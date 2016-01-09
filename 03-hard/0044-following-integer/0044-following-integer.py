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

import bisect
import itertools
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    MAXINT = sys.maxint
    MININT = -sys.maxint - 1

    filter = itertools.ifilter
    map = itertools.imap
    range = xrange
    zip = itertools.izip

else:  # >= 3
    MAXINT = sys.maxsize
    MININT = -sys.maxsize - 1

# This solution avoids using the built-in itertools.permutations and set and
# ... which can be memory/cpu intensive if the amount of digits starts growing


def following_integer(n, ZERO='0'):
    # find out if moving one digit forward gives us the next
    # 115 -> 151, 2048 -> 2084, 20005, 20050
    ln = len(n)
    # loop backwards to compare tail to predecessor
    for i in range(ln - 1, 0, -1):
        ni1 = n[i - 1]
        # find any number greater in range greater than predecessor
        gti = [(x, j) for j, x in enumerate(n[i:]) if x > ni1]
        if not gti:
            continue

        # swap predecessor, against the minimum of the greater
        gtidx = min(gti)[1] + i  # compensate enumerate 0 based j
        n[i - 1], n[gtidx] = n[gtidx], n[i - 1]

        # return up to and including predecessor position + minimum of next
        # range (sorted)
        return n[0:i] + sorted(n[i:])

    # Need to grow digit length. Zero inserted as 2nd leftmost digit
    # After pushing the lowest greater than 0 digit to the left
    # 842 -> 248 -> 2048, 8420 -> 0248 -> 20048
    sn = sorted(n)
    # get first which is not zero
    ifirst = next(i for i in range(ln) if sn[i] != ZERO)
    # return the new string with a 2nd leftmost zero
    n = [sn.pop(ifirst), ZERO] + sn

    return n


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        n = list(line.rstrip('\n'))
        o = following_integer(n)
        print(''.join(o))

    test_cases.close()
