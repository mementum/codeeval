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

# References
# http://web.mit.edu/dimitrib/www/Orig_Auction.pdf
# http://web.mit.edu/dimitrib/www/Auction_Encycl.pdf
# http://www.mit.edu/~dimitrib/Auction_Survey.pdf

import fractions
import string
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    import itertools

    PY2 = True
    range = xrange
    zip = itertools.izip
else:  # >= 3
    PY2 = False


def suit_score(pname, cname):
    # letters are "alpha" characters
    vowels = ['a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y']

    if PY2:
        pletters = sum(1 for p in pname if p.isalpha())
        cletters = sum(1 for c in cname if c.isalpha())
    else:
        pletters = sum(1 for p in pname if p in string.ascii_letters)
        cletters = sum(1 for c in cname if c in string.ascii_letters)

    if not (pletters % 2):
        sscore = 1.5 * sum(1 for c in cname if c in vowels)
    else:
        if PY2:
            sscore = sum(1 for c in cname
                         if c.isalpha() and c not in vowels)
        else:
            sscore = sum(1 for c in cname
                         if c in string.ascii_letters and c not in vowels)

    if fractions.gcd(pletters, cletters) > 1:
        sscore *= 1.5

    return int(sscore * 100.0)


def auction_round(assigned, notassigned, values, epsilon):
    # Get only one unassigned -> there must be at least one or else no call
    # This is the Gauss-Seidl method theoretically best suited for serial
    # environments and we are actually executing in one
    unassigned = notassigned.pop()  # get the last one
    vua = values[unassigned]  # get the value per produce (s_score)

    # Find the best product
    bestval = max(vua)
    bestidx = vua.index(bestval)

    # Get the 2nd best product ... or self if no product with less value
    bestval2 = max([x for x in vua if x != bestval] or [bestval])
    bidincrement = bestval - bestval2 + epsilon  # calculate bid increment

    # In the auction algorithm the "price" of the object is increased and the
    # actual net value reached by the customer is aij - pj
    # But we have a copy of the aijs (values) and instead of increasing the
    # price we can directly decrease the value. For all occurences of object
    # (once per customer)
    for v in values:
        v[bestidx] -= bidincrement

    # unassign previous customer
    if bestidx in assigned:
        notassigned.append(assigned[bestidx])

    # assign current customer
    assigned[bestidx] = unassigned


def auction(values, epsilon=None):
    # This is a single Epsilon pass which is the main algorithm in Ber papers
    assigned = {}
    notassigned = list(range(len(values)))

    if False and epsilon is None:
        epsilon = 25  # we have "int'ed" the values by 100
    else:  # auto epsilon generation
        diffs = [[abs(i - j) for i, j in zip(v[:-1], v[1:]) if i != j]
                 for v in values]
        mininums = [min(x) for x in diffs]
        epsilon = min(mininums) // 2 + 1

    while notassigned:
        auction_round(assigned, notassigned, values, epsilon)

    return assigned


def max_score(assigned, values):
    # given an auction assignment returns the total value
    return sum(values[assigned[p]][p] for p in assigned) / 100.0


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    count = 0
    linput = []
    for line in test_cases:
        ls = line.rstrip('\n')

        cstr, pstr = ls.split(';')
        # split on an empty string will return [''] ... No go ... extra work
        customers = cstr.split(',') if cstr else []
        products = pstr.split(',') if pstr else []

        linput.append((len(customers), len(products), line))

        # Early break
        if not customers or not products:
            print('%.2f' % 0.0)  # print a null score
            continue

        # generate the values
        values = [[suit_score(p, c) for p in products] for c in customers]

        # if not enough products --> fill up with phantom entries
        extracols = len(customers) - len(products)
        if extracols:
            for v in values:
                v.extend([0] * extracols)

        # Do the thing - send a copy because values will be modified
        assignment = auction([v[:] for v in values])

        # And print the result
        print('%.2f' % max_score(assignment, values))

    test_cases.close()
    for lc, lp, l in linput:
        print('%d / %d /' % (lc, lp), l, end='')
