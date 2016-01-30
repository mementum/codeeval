#!/usr/bin/env python
# -*- coding: utf-8; py-indent-offset:4 -*-
###############################################################################
#
# Copyright (C) 2015, 2016 Daniel Rodriguez
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
    pass
else:  # >= 3
    pass


def can_be_smiley(msg):
    openbrackets = 0
    opensmileys = 0
    closesmileys = 0

    imsg = iter(msg)  # use iter in loop
    for c in imsg:
        if c == '(':
            openbrackets += 1

        elif c == ')':
            if opensmileys and not openbrackets:
                opensmileys -= 1
            else:
                openbrackets -= 1
        elif c == ':':
            while True:
                c1 = next(imsg, '')  # ret nothing if exhausted
                if c1 != ':':  # consider consecutive :
                    break
            closesmileys += c1 == ')'
            opensmileys += c1 == '('

        if openbrackets < 0:  # quick breakout at any moment
            break

        # invalidate closesmileys if no openbrackets exist
        closesmileys *= openbrackets > 0

    # Compensate openbrackets with closing smileys
    if openbrackets > 0:
        openbrackets *= closesmileys < openbrackets

    return not openbrackets


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        print('YES' if can_be_smiley(line) else 'NO')

    test_cases.close()
