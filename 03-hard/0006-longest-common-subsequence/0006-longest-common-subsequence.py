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

from bisect import bisect_left
import collections
import itertools
import sys


# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    range = xrange
    map = itertools.imap
else:
    pass


def compute_lcs_with_sets(t1, t2):
    '''
    Computes LCS (Longest Common Sequence) of strings t1, t2

      - Calculates the combinations for each of the strings for a given length

        The length starts at the shortest of the length of both strings

      - Those combinations are put into sets

      - An intersection of the sets is done. If any element is common to both
        the intersection will have at least 1 element (nothing prevents the
        strings from having 2 LCS at the same length)
    '''
    minlen = min(len(t1), len(t2))
    for l in range(minlen, 0, -1):
        # Calculate the combinations
        c1 = itertools.combinations(t1, l)
        c2 = itertools.combinations(t2, l)

        # Turn into sets
        s1 = set(c1)
        # print(s1)
        s2 = set(c2)
        # print(s2)

        # intersection
        lcs = s1 & s2

        if lcs:
            # At least 1 element - return a string
            # return ''.join(lcs.pop())
            return lcs.pop()

    # Out of the loop ... no LCS found
    return ''


def compute_lcs_with_product(t1, t2):
    '''
    Computes LCS (Longest Common Sequence) of strings t1, t2

      - Calculates the combinations for each of the strings for a given length

        The length starts at the shortest of the length of both strings

      - Those combinations are cartesian multiplied

      - If for any of the (x, y) cartesian pais x == y, we have a match
    '''
    minlen = min(len(t1), len(t2))
    for l in range(minlen, 0, -1):
        # Calculate the combinations
        c1 = itertools.combinations(t1, l)
        c2 = itertools.combinations(t2, l)

        p = itertools.product(c1, c2)
        for x, y in p:
            if x == y:
                return ''.join(x)

    # Out of the loop ... no LCS found
    return ''


MakePair = collections.namedtuple('Pair', ['index1', 'index2', 'nextpair'])


def Pairs(matches):
    traces = list()
    threshold = list()

    for index1, match in enumerate(matches):
        for index2 in match:
            index3 = bisect_left(threshold, index2)

            if index3 == len(threshold):
                threshold.append(index2)
                prefix = traces[index3 - 1] if index3 > 0 else None
                traces.append(MakePair(index1, index2, prefix))

            else:
                threshold[index3] = index2
                prefix = traces[index3 - 1] if index3 > 0 else None
                traces[index3] = MakePair(index1, index2, prefix)

    last = traces[-1] if len(traces) > 0 else None
    pair = last
    return len(threshold), pair


def Select(pair, s1, s2):
    string = []
    # pair is tuple (index1, index2, nextpair)
    while pair is not None:
        c = s2[pair.index2]
        string.append(c)
        pair = pair.nextpair

    return string


def Match(s1, s2):
    char2index = collections.defaultdict(collections.deque)
    for index, char in enumerate(s2):
        char2index[char].appendleft(index)

    matches = list(map(lambda c: char2index[c], s1))
    return matches


def Correspondence(s1, s2):
    matches = Match(s1, s2)
    length, last = Pairs(matches)
    return Select(last, s1, s2)


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for test in test_cases:
        tstrip = test.rstrip('\r\n')
        if not tstrip:
            continue

        t1, t2 = test.split(';')
        lcs = Correspondence(t1, t2)

        print(''.join(reversed(lcs)))

    test_cases.close()
