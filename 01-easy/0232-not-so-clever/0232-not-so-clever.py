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

import itertools
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    range = xrange
else:  # >= 3
    pass


def stupid_sort(tosort, iterations):

    while(iterations):
        iterations -= 1

        dirty = False
        for i in range(len(tosort) - 1):
            a, b = tosort[i: i + 2]
            if b < a:
                tosort[i], tosort[i + 1] = b, a
                dirty = True
                break

        if not dirty:
            break

    return tosort


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        tosort, iterations = line.split('|')
        tosort = [int(x) for x in tosort.split()]

        tsorted = stupid_sort(tosort, int(iterations))
        print(' '.join((str(x) for x in tsorted)))

    test_cases.close()
