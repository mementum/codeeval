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
from __future__ import (absolute_import, division, print_function,)
#                        unicode_literals)

import itertools
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    pass
else:  # >= 3
    pass


def is_prime(n):
    if n in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]:
        return True

    if n < 2 or not (n & 1) or not (n % 3):
        return False

    # for numbers below 1373653
    k = 1
    while (36 * k * k) - (12 * k) < n:
        if (n % (6 * k + 1) == 0) or (n % (6 * k - 1) == 0):
            return False

        k += 1

    return True


if __name__ == '__main__':
    n = 1000
    while n > 2:
        n -= 1
        # Check if palindrome ... (faster)
        forw, backw = n, 0
        while forw:
            backw = 10 * backw + forw % 10
            forw //= 10

        if backw != n:
            continue

        if is_prime(n):
            print(n)
            break
