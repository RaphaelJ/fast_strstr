Fast strstr()
=============

A fast substitution to the stdlib's `strstr()` sub-string search function.

`fast_strstr()` is significantly faster than most sub-string search algorithms
when searching relatively small sub-strings, such as words. We recommend any
user to benchmark the algorithm on their data as it uses the same interface as
`strstr()` before discarding other algorithms.

Its worst case complexity (`O(n × m)` where `n` is the length of the string and
`m` the length of the searched sub-string) is the same as the naive brute-force
algorithm but it mostly runs with a linear complexity (`O(n)`) on most strings.

Algorithm
---------

Unlike other efficient sub-string search algorithms, it doesn't try to skip
characters of the string but reads *every character* of the string.
However, the amount of work done for each character is substantially smaller.

This algorithm is thus faster when the searched sub-string has a small number of
characters as other algorithms are not able to skip a large number of
characters.

Its starts by computing the sum of the character values of the sub-string.

It then reads the string by shifting a reading window the size of the
sub-string. It knows the sum of the character values inside this window and is
able to update this sum when shifting it.

Comparing the two sub-string (the one inside the iterating window and the
searched one) only occurs when the both sums are equal. This trick enable the
algorithm to skip a large number of comparisons.

A reference implementation of the algorithm is freely available
[here](fast_strstr.c).

This algorithm is a special case of the
[Rabin-Karp algorithm](http://en.wikipedia.org/wiki/Rabin-Karp_algorithm) for
sub-string search which uses a simple sum as rolling hash function.

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
Scores where compared to `strstr()`.

Section of 10, 100, 500, 1000, 5000, 10000, 50000 characters of *Bello Gallico*
along with the full text (147277 characters) have been used.

| Algorithm \ Size  | 10                | 100              | 500                | 1000              |
| ----------------- |:-----------------:|:----------------:|:------------------:|:-----------------:|
| strstr()          | 3.6 µs (1×)       | 12.4 µs (1×)     | 78.3 µs (1×)       | 162 µs (1×)       |
| naive strstr()    | 9.7 µs (2.7×)     | 89.8 µs (7.2×)   | 479 µs (6.1×)      | 943 µs (5.8×)     |
| Volnitsky         | 138 µs (39×)      | 146 µs (11.8×)   | 186 µs (2.4×)      | 239 µs (1.5×)     |
| **fast_strstr()** | **2.3 µs (0.6×)** | **7 µs (0.6×)**  | **50.9 µs (0.6×)** | **108 µs (0.7×)** |

| Algorithm \ Size  | 5000              | 10000             | 50000              | Full text (147277) |
| ----------------- |:-----------------:|:-----------------:|:------------------:|:------------------:|
| strstr()          | 841 µs (1×)       | 1.7 ms (1×)       | 8.5 ms (1×)        | 25.2 ms (1×)       |
| naive strstr()    | 4.8 ms (5.6×)     | 9.6 ms (5.6×)     | 48 ms (5.6×)       | 140 ms (5.52×)     |
| Volnitsky         | 635 µs (0.7×)     | 1.3 ms (0.8×)     | 10 ms (1.2×)       | 58.6 ms (2.3×)     |
| **fast_strstr()** | **570 µs (0.7×)** | **1.1 ms (0.7×)** | **6.2 ms (0.7×)**  | **17.2 ms (0.7×)** |

Notice that the algorithm doesn't require to pre-process the sub-string while
other algorithms such as Volnitsky's do, it is also fast on short strings.

Benchmark was also tried on newer *Intel's Haswell* and *Sandy Bridge*
processors, with less impressive results (`strstr()` defeated `fast_strstr()` in
for large texts).
We are working on what could give such poorer results. As said in the
introduction, we suggest you to try the algorithm on your data before discarding
`strstr()`.

Benchmark scripts are available [here](benchmark).

License
-------

This algorithm is licensed under the open-source
[BSD3 license](http://opensource.org/licenses/BSD-3-Clause) :

Copyright (c) 2014, Raphael Javaux
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
