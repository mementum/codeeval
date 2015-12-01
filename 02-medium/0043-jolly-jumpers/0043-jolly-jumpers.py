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

import itertools
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    map = itertools.imap
else:  # >= Py3
    pass


JOLLY_TEXT = {
    True: 'Jolly',
    False: 'Not jolly',
}


def is_jolly_jumper(n, lst):
    jolly = n - 1
    prev = next(lst)
    s = set()
    for cur in lst:
        diff = abs(prev - cur)
        if not (jolly >= diff > 0):
            return False  # short circuit for long sequences
        s.add(diff)
        prev = cur

    return len(s) == jolly


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for test in test_cases:
        tst = test.rstrip('\r\n')
        if not tst:
            continue

        tokens = tst.split()
        n, lst = int(tokens[0]), map(int, tokens[1:])
        isjolly = is_jolly_jumper(n, lst)
        print(JOLLY_TEXT[isjolly])

    test_cases.close()
