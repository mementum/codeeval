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
    pass
else:  # >= 3
    def cmp(a, b): return (a > b) - (a < b)


SCORE_STRAIGHT_FLUSH = 8000000
SCORE_FOUR_OF_A_KIND = 7000000
SCORE_FULL_HOUSE = 6000000
SCORE_FLUSH = 5000000
SCORE_STRAIGHT = 4000000
SCORE_THREE_OF_A_KIND = 3000000
SCORE_TWO_PAIR = 2000000
SCORE_PAIR = 1000000
SCORE_HIGH_CARD = 0

STDECK = '2345A23456789TJQKA'
DECK = '23456789TJQKA'


def hand_eval(hand):
    vals, suits = ''.join(sorted(hand[0::3], key=DECK.index)), hand[1::3]

    flush = suits.count(suits[0]) == len(suits)
    straight = STDECK.find(vals)

    if straight >= 0:
        score = SCORE_STRAIGHT_FLUSH * flush or SCORE_STRAIGHT
        score += straight

    else:
        rank = sorted(map(lambda v: (vals.count(v), DECK.index(v)), set(vals)))
        lowrank, lidx = rank[-2]
        highrank, hidx = rank[-1]

        if highrank == 4:
            score = SCORE_FOUR_OF_A_KIND
            score += hidx

        elif highrank == 3:
            score = SCORE_FULL_HOUSE * (lowrank == 2) or SCORE_THREE_OF_A_KIND
            score += hidx

        elif highrank == 2:
            score = SCORE_TWO_PAIR * (lowrank == 2) or SCORE_PAIR
            score += sum([pow(13, i) * kc[1] for i, kc in enumerate(rank)])

        else:
            score = SCORE_FLUSH * flush or SCORE_HIGH_CARD
            score += sum([pow(13, i) * kc[1] for i, kc in enumerate(rank)])

    return score


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')

    results = {1: 'left', -1: 'right', 0: 'none'}

    for line in test_cases:
        hands = line.rstrip('\n')
        nhands = ((len(hands) + 1) // 3) // 2
        lh = hands[0:nhands * 2 + nhands - 1]
        rh = hands[nhands * 2 + nhands:]

        retcmp = cmp(hand_eval(lh), hand_eval(rh))
        print(results[retcmp])

    test_cases.close()
