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

import collections
import heapq
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    pass
else:  # >= 3
    pass


def huffman(node, result, code=''):
    # Descend the tree recursively creating the codes
    if len(node[2]) > 1:
        huffman(node[3][0], result, code + '0')
        huffman(node[3][1], result, code + '1')
    else:
        result[node[2]] = code


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        ls = line.rstrip('\n')

        f = collections.Counter(ls)
        h = [(f[s], -1, s, (None, None)) for s in f]
        heapq.heapify(h)

        while(len(h) > 1):
            l, r = heapq.heappop(h), heapq.heappop(h)
            heapq.heappush(h, (l[0] + r[0], l[1] + r[1], l[2] + r[2], (l, r)))

        res = dict()
        huffman(h.pop(), res)

        result = ' '.join(('%s: %s;' % (k, res[k]) for k in sorted(res)))
        print(result)

    test_cases.close()
