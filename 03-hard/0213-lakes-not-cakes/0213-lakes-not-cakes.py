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
    range = xrange
else:  # >= 3
    pass


def visit_node(forest, r, c, start=False):
    if forest[r][c] <= 0:  # border, no lake or visit start
        return

    forest[r][c] = -1 * start  # mark as start or visited

    for y in (-1, 0, 1):
        for x in (-1, 0, 1):
            visit_node(forest, r + y, c + x)


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:

        # split the forest rows
        rs = line.rstrip('\n').split('|')

        # crate the forest adding extra left/right columns for easy iteration
        forest = [[0] + [int(x == 'o') for x in r.split()] + [0] for r in rs]

        # Add extra top/bottoms rows for easy iteration
        nrows, ncols = len(forest) + 2, len(forest[0])
        forest.insert(0, [0] * ncols)
        forest.append([0] * ncols)

        # visit the lakes
        for r in range(1, nrows - 1):
            for c in range(1, ncols - 1):
                visit_node(forest, r, c, start=True)

        print(-sum((sum(x) for x in forest)))

    test_cases.close()
