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

import array
import itertools
import sys


# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    map = itertools.imap
    range = xrange
else:  # >= 3
    pass

# The problem is about generating a set of values and then do a sliding window
# operation discounting values from the left and adding from the right with a
# new criterion (minimum non negative integer not yet present in the window)

# By extending the array directly to the size of n, rather than the minimum k,
# the operation can be completed with direct [] accesses instead of having to
# do appends and pops (be it with lists or deques

# Surprisingly this consumes lots of memory, which can be alleviated with
# array.array. This increases computation time 5x but because memory is more
# important in the calculation of CodeEval, the scores increase 10+ with
# regards to the originals by having reduced memory consumption 2x (Python 2.7)
# and over 4x (Python 3)


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        n, k, a, b, c, r = map(int, line.split(','))
        m = a % r  # all other values are mod r -> make it fit

        l = k + 1

        if True:
            ks = [m] * n
            counts = [0] * l  # keep track of frequence of present ints
        else:
            ks = array.array('i', itertools.repeat(m, n))
            counts = array.array('i', itertools.repeat(0, l))

        if m < l:
            counts[m] = 1  # don't forget to init count for start value

        for i in range(1, k):
            ks[i] = m = (b * m + c) % r
            if m < l:
                counts[m] += 1  # update count save 1 loop doing it here

        for i in range(k, n):
            ks[i] = mnni = counts.index(0)  # first missing non-negative int
            counts[mnni] = 1  # add count for incoming element
            oow = ks[i - k]
            if oow < l:
                counts[ks[i - k]] -= 1  # decrease count for removed element

        print(ks[-1])

    test_cases.close()
