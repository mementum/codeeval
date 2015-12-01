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
from __future__ import (absolute_import, division, print_function)
#                        unicode_literals)

import sys
import itertools

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    pass
else:  # >= Py3
    pass


def reverse_and_add(initial):

    # Start the count at 0, to filter out input which is already a palindrome
    for i in itertools.count(0):
        # inlined reversal for speed
        forward, backwards = initial, 0
        while forward:
            backwards = 10 * backwards + forward % 10
            forward //= 10

        if initial == backwards:
            return i, backwards

        initial += backwards


if __name__ == '__main__':
    import sys

    test_cases = open(sys.argv[1], 'r')
    for test in test_cases:
        tst = test.rstrip('\r\n')
        if not tst:
            continue

        iterations, palindrome = reverse_and_add(int(tst))
        print('%d %d' % (iterations, palindrome))

    test_cases.close()
