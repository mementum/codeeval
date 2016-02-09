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


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')

    BDIM = 256
    BSIZE = BDIM * BDIM
    board = [0] * BSIZE

    for line in test_cases:
        ls = line.rstrip('\n')

        if ls.startswith('Set'):
            cmd, coord, val = ls.split()
            coord, val = int(coord), int(val)

            if cmd[3:] == 'Col':
                r = range(coord, BSIZE, BDIM)
            else:
                r = range(coord * BDIM, coord * BDIM + BDIM)

            for x in r:
                board[x] = val
        else:
            cmd, coord = ls.split()
            coord = int(coord)
            if cmd[5:] == 'Col':
                r = range(coord, BSIZE, BDIM)
            else:
                r = range(coord * BDIM, coord * BDIM + BDIM)

            result = sum(board[x] for x in r)
            print(result)

    test_cases.close()
