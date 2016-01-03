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
    filter = itertools.ifilter
    map = itertools.imap
    range = xrange
    zip = itertools.izip

else:  # >= 3
    def cmp(a, b): return (a > b) - (a < b)


class PokerHand(object):
    CARDS = '23456789TJQKA'
    HIGHVAL = len(CARDS)

    SCORE_STRAIGHT_FLUSH = 8000000
    SCORE_FOUR_OF_A_KIND = 7000000
    SCORE_FULL_HOUSE = 6000000
    SCORE_FLUSH = 5000000
    SCORE_STRAIGHT = 4000000
    SCORE_THREE_OF_A_KIND = 3000000
    SCORE_TWO_PAIRS = 2000000
    SCORE_PAIR = 1000000
    SCORE_HIGH_CARD = 0

    def __init__(self, cards):
        kinds, suits = zip(*cards)  # separate face values from suits

        # flush check
        flush = suits.count(suits[0]) == len(suits)

        # count number of same face values
        kcounts = [kinds.count(x) for x in self.CARDS]

        # group the kinds of counts (zeros, ones, twos, threes, fours)
        kgroups = [kcounts.count(x) for x in range(len(cards))]

        # To value high cards within individual hands - if needed be
        ones = [i for i, x in enumerate(kcounts) if x == 1]

        # Assign scoring
        if kgroups[1] == 5:  # straight, flush, high card
            # check straights and a2345 straight
            if ones[4] - ones[0] == 4:
                score = (self.SCORE_STRAIGHT_FLUSH * flush or
                         self.SCORE_STRAIGHT)

                score += ones[4]

            elif ones[3] == 3 and kcounts[-1]:  # a2345
                score = (self.SCORE_STRAIGHT_FLUSH * flush or
                         self.SCORE_STRAIGHT)

                score += ones[3]

            else:
                score = self.SCORE_FLUSH * flush or self.SCORE_HIGH_CARD
                score += self.value_cards(ones)

        elif kgroups[4]:  # four of kind
            score = self.SCORE_FOUR_OF_A_KIND
            score += kcounts.index(4)

        elif kgroups[3]:  # full house, three of kind
            score = (self.SCORE_FULL_HOUSE * kgroups[2] or
                     self.SCORE_THREE_OF_A_KIND)
            score += kcounts.index(3)

        else:  # one or two pairs
            score = (self.SCORE_TWO_PAIRS * (kgroups[2] == 2) or
                     self.SCORE_PAIR)
            twos = [i for i, x in enumerate(kcounts) if x == 2]
            for t, two in enumerate(twos):
                score += pow(self.HIGHVAL, t + 3) * two
            score += self.value_cards(ones)

        self.score = score

    def value_cards(self, cards):
        # Complete valuation with position of ones
        return sum([pow(self.HIGHVAL, i) * card
                    for i, card in enumerate(cards)])


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        hands = line.rstrip('\n').split()
        retcmp = cmp(PokerHand(hands[:5]).score, PokerHand(hands[5:]).score)
        if retcmp < 0:
            print('right')
        elif retcmp > 0:
            print('left')
        else:
            print('none')

    test_cases.close()
