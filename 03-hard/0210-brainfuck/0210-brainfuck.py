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


def bfck(code):
    '''
    CodeEval seems to be about the lowest memory/speed and 0 about elegance,
    extensibility, maint...

    Test 100% passed with my pybrainfuck configurable package

    Now for raw speed and lowest memory

    array.array would consume less but would use more cpu ... to be tested
    '''

    cells = [0]
    cellptr = 0
    codeptr = 0
    loopskip = 0
    codeptr = -1
    loops = list()

    lcode = len(code)

    while True:
        codeptr += 1
        if codeptr >= lcode:
            break

        codechar = code[codeptr]

        if loopskip:
            if codechar == '[':
                loopskip += 1
            elif codechar == ']':
                loopskip -= 1

        elif codechar == '+':
            cells[cellptr] = (cells[cellptr] + 1) % 256

        elif codechar == '-':
            cells[cellptr] = (cells[cellptr] - 1 + 256) % 256

        elif codechar == '>':
            cellptr += 1
            if cellptr >= len(cells):
                cells.append(0)

        elif codechar == '<':
            cellptr -= 1
            if cellptr < 0:
                cellptr = 0
                cells.insert(0, 0)

        elif codechar == '[':
            if cells[cellptr]:
                loops.append(codeptr)
            else:
                loopskip += 1

        elif codechar == ']':
            if cells[cellptr]:
                codeptr = loops[-1]
            else:
                loops.pop()

        elif codechar == '.':
            print(chr(cells[cellptr]), end='')


if __name__ == '__main__':
    import sys

    test_cases = open(sys.argv[1], 'r')
    for test in test_cases:
        tstrip = test.rstrip('\r\n')
        if not tstrip:
            continue

        bfck(tstrip)
        print('')

    test_cases.close()
