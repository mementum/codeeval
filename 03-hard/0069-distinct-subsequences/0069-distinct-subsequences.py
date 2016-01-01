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
    range = xrange
else:  # >= 3
    pass


def distinct_subsequences(sa, sb):
    colsize = len(sb) + 1
    r0 = [0] * colsize

    for j in range(1, len(sa) + 1):
        a = sa[j - 1]
        r0j1 = r0[0] = 1
        for i in range(1, colsize):
            cache = r0[i]
            r0[i] += r0j1 * (a == sb[i - 1])
            r0j1 = cache

    return r0[-1]


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        a, b = line.rstrip('\n').split(',')
        print(distinct_subsequences(a, b))

    test_cases.close()
