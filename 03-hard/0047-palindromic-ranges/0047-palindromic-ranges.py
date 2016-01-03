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
    map = itertools.imap
    range = xrange
else:  # >= 3
    pass


def is_palindrome(number):
    forward, backwards = number, 0
    while forward:
        backwards = 10 * backwards + forward % 10
        forward //= 10

    return number == backwards


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        first, last = map(int, line.rstrip('\n').split())
        palins = list(map(is_palindrome, range(first, last + 1)))

        size = len(palins)
        iranges = 0
        for rangesize in range(0, size):
            for col in range(0, size - rangesize):
                s = sum(palins[col:col + rangesize + 1])
                iranges += not (s % 2)

        print(iranges)

    test_cases.close()
