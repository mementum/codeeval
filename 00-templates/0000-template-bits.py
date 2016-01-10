#!/usr/bin/env python
# -*- coding: utf-8; py-indent-offset:4 -*-
###############################################################################
#
# Copyright (C) 2015 Daniel Rodriguez
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################


def reverse(n):
    forw, backw = n, 0
    while forw:
        backw = 10 * backw + forw % 10
        forw //= 10

    return backw


def is_palindrome(n):
    forw, backw = n, 0
    while forw:
        backw = 10 * backw + forw % 10
        forw //= 10

    return n == backw


def aks(p):
    # Agrawal–Kayal–Saxena: https://en.wikipedia.org/wiki/AKS_primality_test

    def expand_x_1(n):
        # This version uses a generator and thus less computations
        c = 1
        for i in range(n // 2 + 1):
            c = c * (n - i) // (i + 1)
            yield c

    if p == 2:
        return True

    for i in expand_x_1(p):
        if i % p:
            # we stop without computing all possible solutions
            return False
    return True


def is_prime(n):
    # Rabin-Miller from Rosetta Code
    # The test sets are possibly improbable (according to C++ implemenation and
    # own statement in the code

    def is_composite(a, d, n, s):
        if pow(a, d, n) == 1:
            return False
        for i in range(s):
            if pow(a, 2**i * d, n) == n - 1:
                return False
        return True  # n is composite

    if n in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]:
        return True

    if n < 2 or not (n & 1) or not (n % 3):
        return False

    d, s = n - 1, 0
    while not d % 2:
        d, s = d >> 1, s + 1

    #
    # Check if the sets can be changed to
    #
    # if n < 1,373,653, enough to test a = 2 and 3;
    # if n < 9,080,191, enough to test a = 31 and 73;
    # if n < 4,759,123,141, enough to test a = 2, 7, and 61;
    # if n < 1,122,004,669,633, enough to test a = 2, 13, 23, 1662803;
    # if n < 2,152,302,898,747, enough to test a = 2, 3, 5, 7, 11;
    # if n < 3,474,749,660,383, enough to test a = 2, 3, 5, 7, 11, 13;
    # if n < 341,550,071,728,321, enough to test a = 2, 3, 5, 7, 11, 13, 17

    if n < 1373653:
        return not any(is_composite(a, d, n, s) for a in (2, 3))
    if n < 25326001:
        return not any(is_composite(a, d, n, s) for a in (2, 3, 5))
    if n < 118670087467:
        if n == 3215031751:
            return False
        return not any(is_composite(a, d, n, s) for a in (2, 3, 5, 7))
    if n < 2152302898747:
        return not any(is_composite(a, d, n, s) for a in (2, 3, 5, 7, 11))
    if n < 3474749660383:
        return not any(is_composite(a, d, n, s) for a in (2, 3, 5, 7, 11, 13))
    if n < 341550071728321:
        return not any(is_composite(a, d, n, s)
                       for a in (2, 3, 5, 7, 11, 13, 17))


def is_prime(n):
    # for numbers below 1373653
    if n in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        return True

    if n < 2 or not (n & 1) or not (n % 3):
        return False

    k = 1
    while (36 * k * k) - (12 * k) < n:
        if (n % (6 * k + 1) == 0) or (n % (6 * k - 1) == 0):
            return False

        k += 1

    return True


def is_prime(n):
    # for any number ... but the slowest
    # checks diviibility up to the square root of the input
    if n < 2:  # (0, 1)
        return False

    # some known primes
    if n in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        return True

    if not (n & 1):  # divisible by 2
        return False

    # Check if divisible by > 3
    r = range(3, int(pow(n, 0.5)) + 1, 2)
    if any((not (n % x) for x in r)):
        return False

    return True
