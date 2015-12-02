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
#
#  This challenge can be seen as a simple application of the Needleman-Wunsch
#  algorithm in which restrictions are added:
#
#    - Diagonals are not taken into account, which enables the calculation with
#      a single row of data
#
#    - The "score" to take into account is not that of the match of the 2
#      sequences but rather the value of the cell
#
#    - See: https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm
#
###############################################################################
from __future__ import (absolute_import, division, print_function,)
#                        unicode_literals)

import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    MAXINT = sys.maxint
    MININT = -sys.maxint - 1
else:  # >= 3
    MAXINT = sys.maxsize
    MININT = -sys.maxsize - 1


if __name__ == '__main__':
    import sys

    test_cases = open(sys.argv[1], 'r')
    tc = iter(test_cases)

    for line in tc:
        msize = int(line) + 1  # read matrix n x n size
        r0 = [MAXINT] * msize  # create the row to hold calculations

        # loop m rows
        for m in range(1, msize):
            # Initialize 1st column (virtual)
            cache = MAXINT * ((m - 1) != 0)

            # Prepare the values of the matrix (as a generator)
            val = (int(x) for x in next(tc).split(','))

            # loop n cols
            for n in range(1, msize):  # calculate next row
                r0[n] = cache = min(r0[n], cache) + next(val)

        # Result is in the last calculated row, in the last column
        print(r0[msize - 1])

    test_cases.close()
