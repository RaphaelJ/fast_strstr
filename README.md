Fast strstr()
=============

A fast substitution to the stdlib's `strstr()` sub-string search function.

`fast_strstr()` is significantly faster than most sub-string search algorithm
when searching relatively small sub-strings, such as words. We recommend any
user to benchmark the algorithm on their data as it uses the same interface as
`strstr()` before discarding other algorithms.

Its worst case complexity (`O(n × m)` where `n` is the length of the string and
`m` the length of the searched sub-string) is the same as the naive brute-force
algorithm but it mostly runs with a linear complexity (`O(n)`) on most strings.

Algorithm
---------

Unlike other *efficient* sub-string search algorithms, it doesn't try to skip
characters of the string but reads *every character* of the string.
However, the amount of work done for each character is substantially smaller.

This algorithm is thus faster when the searched sub-string has a small number of
characters as other algorithms are not able to skip a large number of
characters.

Its starts by computing the sum of the character values of the sub-string.

It then reads the string by shifting a reading window the size of the
sub-string. It knows the sum of the character values inside this window and is
able to update this sum when shifting it.

When the sum of the iterating window equals the sum of the sub-string, it
compares the two sub-strings, potentially terminating the search. This trick
enable the algorithm to skip a large number of comparisons.

Benchmarks
----------

The algorithm has been benchmarked together with three other algorithms by
searching all occurrences of 100 random Latin words in portions of *Commentarii
de Bello Gallico* by *Julius Caesar*. Each test was run 100 times on an
*AMD Phenom II X4 965* and the best time was taken.

`strstr()` is from the *GNU C Library* (version 2.19) and uses the *Two Ways
Algorithm*, `naive strstr()` is a naive brute-force implementation and
`Volnitsky's strstr()` is
[an algorithm by Leonid Volnitsky](http://volnitsky.com/project/str_search/)
whereas `fast_strstr()` is our implementation.
Scores where normalized to `strstr()`.

Section of 10, 100, 500, 1000, 2000, 5000, 10000, 50000 and 100000 characters of
*Bello Gallico* along with the full text (147277 characters) have been used.

| Algorithm \ Size | 10              | 100              | 500                |
| ---------------- |:---------------:|:----------------:|:------------------:|
| strstr()         | **3.6 µs (1×)** | **12.4 µs (1×)** | 78.3 µs (1×)       |
| naive strstr()   | 9.7 µs (2.7×)   | 89.8 µs (7.2×)   | 479 µs (6.1×)      |
| Volnitsky        | 138 µs (39×)    | 146 µs (11.8×)   | 186 µs (2.4×)      |
| fast_strstr()    | 4.1 µs (1.1×)   | 14.7 µs (1.2×)   | **64.6 µs (0.8×)** |