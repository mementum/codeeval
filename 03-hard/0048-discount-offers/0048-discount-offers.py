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

import copy
import fractions
import string
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    PY2 = True
    range = xrange
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


def auction_round(assigned, notassigned, prods, vals, epsilon):
    # Get only one unassigned -> there must be at least one or else no call
    # This is the Gauss-Seidl method theoretically best suited for serial
    # environments and we are actually executing in one
    unassigned = notassigned.pop()  # get the last one
    vua = vals[unassigned]  # get the value per produce (s_score)

    # Find the best product
    bestval = max(vua)
    bestidx = vua.index(bestval)
    bestprod = prods[bestidx]

    # Get the 2nd best product ... or self if no product with less value
    bestval2 = max([x for x in vua if x != bestval] or [bestval])
    bidincrement = bestval - bestval2 + epsilon  # calculate bid increment

    # In the auction algorithm the "price" of the object is increased and the
    # actual net value reached by the customer is aij - pj
    # But we have a copy of the aijs (values) and instead of increasing the
    # price we can directly decrease the value. For all occurences of object
    # (once per customer)
    for c in vals:
        vals[c][bestidx] -= bidincrement

    # unassign previous customer
    deassigned = assigned.pop(bestprod, None)
    if deassigned is not None:
        notassigned.append(deassigned)

    # assign current customer
    assigned[bestprod] = unassigned


def auction_repetition(custs, prods, vals, epsilon=None, epdec=0.25):
    # Applying the algorithm with changes in Epsilon makes the algorithm
    # polynomial and "ensures" approaching/reaching the optimal solution. But
    # it is not needed in our case
    # papers, but it is not needed for the solution
    if epsilon is None:
        epsilon = 100.0  # Default start value

    while epsilon > 1 / len(custs):
        assigned = {}
        notassigned = list(custs)

        while notassigned:
            auction_round(assigned, notassigned, prods, vals, epsilon)

        epsilon *= epdec  # recommended scaling

    return assigned


def auction(custs, prods, vals, epsilon=None, epdec=0.5):
    # This is a single Epsilon pass which is the main algorithm in Ber papers
    assigned = {}
    notassigned = list(custs)

    if epsilon is None:
        epsilon = 25.0  # we have "int'ed" the values by 100

    while notassigned:
        auction_round(assigned, notassigned, prods, vals, epsilon)

    return assigned


def max_score(assigned, products, values):
    # given an auction assignment returns the total value
    maxscore = 0
    for p in assigned:
        pidx = products.index(p)
        maxscore += values[assigned[p]][pidx]

    return maxscore / 100.0


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    count = 0
    for line in test_cases:
        ls = line.rstrip('\n')

        cstr, pstr = ls.split(';')
        # split on an empty string will return [''] ... No go ... extra work
        customers = cstr.split(',') if cstr else []
        products = pstr.split(',') if pstr else []

        # Early break
        if not customers or not products:
            print('%.2f' % 0.0)  # print a null score
            continue

        values = {c: [suit_score(p, c) for p in products] for c in customers}

        # if not enough products --> fill up with phantom entries
        for i in range(len(customers) - len(products)):
            products.append('ghost{}'.format(i))
            for c in values:
                values[c].append(0.0)  # ghost product with no value

        # Do the thing
        assignment = auction(customers, products, copy.deepcopy(values))

        # And print the result
        print('%.2f' % max_score(assignment, products, values))

    test_cases.close()
