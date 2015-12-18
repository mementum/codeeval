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


COINS = [50, 25, 10, 5]
NUMCOINS = 4


def alternative_reality(cents, coinidx):
    if not cents or coinidx >= NUMCOINS:
        return 1

    coin = COINS[coinidx]
    coincount = cents // coin
    changes = 0
    for i in range(coincount + 1):
        changes += alternative_reality(cents - (coin * i), coinidx + 1)

    return changes


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        ls = line.rstrip('\r\n')

        print(alternative_reality(int(ls), 0))

    test_cases.close()
