#!/usr/bin/env python
# -*- coding: utf-8; py-indent-offset:4 -*-
###############################################################################
#
# Copyright (C) 2016 Daniel Rodriguez
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

import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    pass
else:  # >= 3
    pass

if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')

    rs = ['2', '3', '4', '5', '6', '7', '8', '9', '1', 'J', 'Q', 'K', 'A']
    ranks = {r: i for i, r in enumerate(rs)}
    tval = 100

    for line in test_cases:
        c1, c2, _, t = line.rstrip('\n').split()

        v1 = ranks[c1[0]] + tval * (c1[-1] == t)
        v2 = ranks[c2[0]] + tval * (c2[-1] == t)

        d = v1 - v2
        if not d:
            print(c1, c2)
        else:
            print(c1 if d > 0 else c2)

    test_cases.close()
