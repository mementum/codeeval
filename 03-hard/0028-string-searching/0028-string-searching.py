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

import sys


# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    range = xrange
else:  # >= 3
    pass


def is_sub(first, second, loop=True, escape=False):
    if not second:  # substr consumed ... match
        return True

    if not first:  # mainstr consumed ... match remaining
        return False

    if second[0] == '\\':
        return is_sub(first, second[1:], loop=loop, escape=True)

    if not escape and second[0] == '*':
        return is_sub(first, second[1:])

    for i, f in enumerate(first):
        if f == second[0]:
            if is_sub(first[i + 1:], second[1:], loop=False):
                return True

        if not loop:
            break

    return False


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:

        a, b = line.rstrip('\n').split(',')
        # print('%s,%s - ' % (a, b), end='')
        print('true' if is_sub(a, b) else 'false')

    test_cases.close()
