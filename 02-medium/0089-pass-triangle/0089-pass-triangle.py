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
    MAXINT = sys.maxint
    MININT = -sys.maxint - 1

    import itertools
    filter = itertools.ifilter
    map = itertools.imap
    range = xrange
    zip = itertools.izip

else:  # >= 3
    MAXINT = sys.maxsize
    MININT = -sys.maxsize - 1


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')

    INITIAL_SIZE = 100

    v0 = [0] * INITIAL_SIZE
    v1 = [0] * INITIAL_SIZE

    j = 0
    for line in test_cases:
        j += 1
        ls = line.rstrip('\r\n')

        v0, v1 = v1, v0

        if j > INITIAL_SIZE:
            v0.append(0)
            v1.append(0)

        cache = 0
        i = 0
        for lsi in map(int, ls.split()):
            v1[i] = max(v0[i], cache) + lsi
            cache = v0[i]
            i += 1

    # Deliver result
    print(max(v1))

    test_cases.close()
