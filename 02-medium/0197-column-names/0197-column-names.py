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
    pass

else:  # >= 3
    pass


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')

    OFFSET = ord('A') - 1
    ALPHALEN = 26

    for line in test_cases:
        colnum = int(line.rstrip('\r\n'))

        colnames = list()
        while colnum:
            div, rem = divmod(colnum, ALPHALEN)
            # map char ... taking boundary (Z) into account
            colnames.append(chr(OFFSET + (rem or ALPHALEN)))
            colnum = div - 1 * (not rem)  # remove 1 int if landed on boundary

        print(''.join(reversed(colnames)))

    test_cases.close()
