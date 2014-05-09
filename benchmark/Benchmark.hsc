{-# LANGUAGE BangPatterns, ForeignFunctionInterface #-}
import Control.Monad
import Criterion.Main
import Data.Function
import Data.List
import Foreign.C.String
import Foreign.Ptr
import System.Random

path :: FilePath
path = "de_bello_gallico.txt"

nWords :: Int
nWords = 100

tests :: [Int]
tests = [10, 100, 500, 1000, 2000, 5000, 10000, 50000, 100000, 147277]

type StrStrFun = CString -> CString -> IO CString

foreign import ccall unsafe "strstr"           strstr           :: StrStrFun
foreign import ccall unsafe "fast_strstr"      fast_strstr      :: StrStrFun
foreign import ccall unsafe "naive_strstr"     naive_strstr     :: StrStrFun
foreign import ccall unsafe "volnitsky_strstr" volnitsky_strstr :: StrStrFun

-- | Unsorts the elements of a list (complexity: O(n * log² n)).
shuffle :: RandomGen g => g -> [a] -> [a]
shuffle gen = map snd . sortBy (compare `on` fst) . zip (randoms gen :: [Int])

main :: IO ()
main = do
    content <- readFile path
    ws <- mapM newCString $ take nWords $ shuffle (mkStdGen 0) $ words content
    contents <- mapM newCString [ take nChars content | nChars <- tests ]

    defaultMain [ testWith ws c nChars | (nChars, c) <- zip tests contents ]
  where
    testWith ws content nChars =
        bgroup (show nChars) [
              testFun ws content strstr           "strstr"
            , testFun ws content fast_strstr      "fast_strstr"
            , testFun ws content naive_strstr     "naive_strstr"
            , testFun ws content volnitsky_strstr "volnitsky_strstr"
            ]

    testFun ws content fun funName =
        bench funName $
            forM_ ws $ \w ->
                testWord w content fun

    testWord :: CString -> CString -> StrStrFun -> IO ()
    testWord w content fun = do
        res <- fun content w
        when (res /= nullPtr) $
            testWord w (res `plusPtr` 1) fun
