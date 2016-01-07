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


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        n = tuple(line.rstrip('\n'))

        per = itertools.permutations(n, len(n))
        per_no_l0 = filter(lambda x: x[0] != '0', per)
        pjoin = sorted(set(per_no_l0))

        insidx = bisect.bisect_right(pjoin, n)

        if insidx < len(pjoin):
            result = pjoin[insidx]
        else:
            result = list(pjoin[0])
            result.insert(1, '0')

        print(''.join(result))

    test_cases.close()
