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

import collections
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


class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return '%d %d' % (self.x, self.y)


def is_in_poly(vs, p):
    # Returns True if "on line" or winding number is not 0 (in polygon)
    wn = 0  # winding number

    for i in range(len(vs) - 1):
        v0, v1 = vs[i], vs[i + 1]

        # the 2 following functions operate with closures for direct access to
        # v0, v1 and p
        #
        # Return if point is on-line
        def on_line_x():
            if v0.x <= v1.x:
                return v0.x <= p.x and p.x <= v1.x

            return v0.x >= p.x and p.x >= v1.x

        # return if point is left or right of v0->v1
        # The result is interpreted with regards to v0->v1 vertical direction
        def left_or_right():
            return ((v1.x - v0.x) * (p.y - v0.y)) - ((v1.y - v0.y) * (p.x - v0.x))

        if v1.y >= p.y and p.y > v0.y:  # v0 -> v1 upwards
            lor = left_or_right()
            if lor > 0:
                wn += 1
            elif lor == 0:
                return True
        elif v0.y >= p.y and p.y > v1.y:  # v1 -> v0 downwards
            lor = left_or_right()
            if lor < 0:
                wn -= 1
            elif lor == 0:
                return True
        elif v0.y == p.y and on_line_x():  # v0/v1 on same vertical as point
            return True

    return wn != 0


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        ls = line.rstrip('\r\n')

        # Parse the coordinates - separate vertex from point
        vxys, _, pxy = ls.rpartition('|')
        # Parse the vertex
        vcoords = list()
        for vxy in vxys.split(','):
            x, y = vxy.split()
            vcoords.append(Point(int(x), int(y)))

        # Append 1st as last to close the polygon
        vcoords.append(vcoords[0])  # close the polygon

        # Parse the point
        x, y = pxy.split()
        pt = Point(int(x), int(y))

        print("Prisoner" if is_in_poly(vcoords, pt) else "Citizen")

    test_cases.close()
