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
    def viewitems(d): return d.viewitems()
else:  # >= 3
    def viewitems(d): return d.items()


def glue_pieces(kmers):
    # Simplified version of a real (mostly) DNS assembly algorithm using a De
    # Bruijn graph to assemble using a Eulerian path. Simplified because
    #  - A sing solution exists which implies 2 nodes with odd degree exist and
    #    one of them is the head (more outs than ins)
    #
    # The edges are the kmers
    edges = collections.defaultdict(list)
    # The vertex are the kmers - 1 pieces
    vertex = dict()

    # Keep track of the ins / outs (balance) at each vertex
    vertbals = collections.defaultdict(int)

    for kmer in kmers:
        kl, kr = kmer[0:-1], kmer[1:]  # create the kmer-1 -> vertex

        vertbals[kl] -= 1  # keep balance of outpus
        vertbals[kr] += 1  # keep balance of inputs

        edges[kl].append(kr)  # create an edge kl -> kr

    stack = list()  # keep the visited nodes until no further paths out
    path = []  # keep the path

    # Find the source (head) of the DBN graph (inputs < outputs or vbal == -1)
    head = src = next((k for k, vbal in viewitems(vertbals) if vbal == -1))

    # length of content in vertex - also spring factor
    k1len = len(kmers[0]) - 1
    i = 0  # keep track of number of inserted vertex in the path

    # Standard Hierholzer algorithm
    while True:
        if not edges[src]:
            if not (i % k1len):  # jump k1len - 1 nodes (overlapping content)
                path.append(src)
            if not stack:
                break
            i += 1
            src = stack.pop()
        else:
            stack.append(src)
            src = edges[src].pop()

    # The number of elements in the path may not have allowed head to
    # make it ... add the missing text
    missing_text = (len(kmers) + k1len) - (len(path) * k1len)
    if missing_text:
        path.append(head[:missing_text])

    # Join (reversed) and return the gathered pieces
    return ''.join((path[i] for i in range(len(path) - 1, -1, -1)))


if __name__ == '__main__':
    test_cases = open(sys.argv[1], 'r')
    for line in test_cases:
        kmers = line.rstrip('|\r\n').lstrip('|').split('|')
        print(glue_pieces(kmers))

    test_cases.close()
