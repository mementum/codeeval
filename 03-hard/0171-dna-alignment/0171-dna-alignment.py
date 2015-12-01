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
from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import array
import itertools
import sys

# Small Py2/3 compatibility layer
if sys.version_info.major == 2:
    range = xrange
    map = itertools.imap
else:  # >= Py3
    pass


MATCH = 3
MISMATCH = -3
INDEL_START = 8
INDEL_EXTENSION = 1

ARRAYCODE = 'i'  # 2 bytes per code
MAXINT = 2**15
MININT = -MAXINT + 1

GACT_CODING = dict(
    X=0,
    G=1,
    A=2,
    C=4,
    T=8)


def nw_affine_gap_opt(seqa, seqb,
                      match=MATCH, mmatch=MISMATCH,
                      opening=INDEL_START, extension=INDEL_EXTENSION,):

    # This version optimizes memory usage by keeping only 2 rows
    # of each matrix active. Backtracking is not needed because
    # only the score is sought

    ROWSIZE, COLSIZE = len(seqa) + 1, len(seqb) + 1
    MSIZE = ROWSIZE * COLSIZE

    # Matrices Creation
    M0 = [MININT] * COLSIZE
    M1 = [0] * COLSIZE

    Ix0 = [MININT] * COLSIZE
    Ix1 = [0] * COLSIZE

    Iy0 = [MININT] * COLSIZE
    Iy1 = [0] * COLSIZE

    # Spot 0 initialization
    M0[0] = 0

    # Initialization of spot 0 of M and row 0 of Iy
    for j in range(1, COLSIZE):
        Iy0[j] = -opening - (j - 1) * extension

    for i in range(1, ROWSIZE):
        # Initialize col 0
        Ix1[0] = -opening - (i - 1) * extension
        M1[0] = Iy1[0] = MININT

        for j in range(1, COLSIZE):
            # Precompute coordinates
            j1 = j - 1
            # Gaps on X
            m = M0[j] - opening
            ix = Ix0[j] - extension
            Ix1[j] = max(m, ix)

            # Gaps on Y
            m = M1[j1] - opening
            iy = Iy1[j1] - extension
            Iy1[j] = max(m, iy)

            # Matches/Mismatches
            # - 1 for seqa/seqb because sequences start at 0
            mscore = match if (seqa[i - 1] == seqb[j - 1]) else mmatch
            m = M0[j1] + mscore
            ix = Ix0[j1] + mscore
            iy = Iy0[j1] + mscore

            M1[j] = max(m, ix, iy)

        # Swap rows, no clean-up needed
        M0, M1 = M1, M0
        Ix0, Ix1 = Ix1, Ix0
        Iy0, Iy1 = Iy1, Iy0

    # Last Matrix field has the best score - M0/M1 were swapped
    return M0[COLSIZE - 1]


def nw_affine_gap(seqa, seqb,
                  match=MATCH, mmatch=MISMATCH,
                  opening=INDEL_START, extension=INDEL_EXTENSION,
                  usearray=False):

    if usearray:
        sa = array.array(str('B'),
                         map(lambda x: GACT_CODING[str(x)], 'X' + seqa))
        sb = array.array(str('B'),
                         map(lambda x: GACT_CODING[str(x)], 'X' + seqb))
    else:
        sa = list(map(lambda x: GACT_CODING[str(x)], 'X' + seqa))
        sb = list(map(lambda x: GACT_CODING[str(x)], 'X' + seqb))

    ROWSIZE, COLSIZE = len(seqa) + 1, len(seqb) + 1
    MSIZE = ROWSIZE * COLSIZE

    # Matrices Creation
    if usearray:
        M = array.array(str('i'), itertools.repeat(0, MSIZE))
        Ix = array.array(str('i'), itertools.repeat(0, MSIZE))
        Iy = array.array(str('i'), itertools.repeat(0, MSIZE))
    else:
        M = list(itertools.repeat(0, MSIZE))
        Ix = list(itertools.repeat(0, MSIZE))
        Iy = list(itertools.repeat(0, MSIZE))

    # Matrices Initialization
    # Spot 0
    r, c = 0, 0
    rc = r + c

    M[rc] = 0
    Ix[rc] = MININT
    Iy[rc] = MININT

    # Col 0
    for i in range(1, ROWSIZE):
        r, c = i * COLSIZE, 0
        rc = r + c

        Ix[rc] = -opening - (i - 1) * extension
        M[rc] = Iy[rc] = MININT

    # Row 0
    for j in range(1, COLSIZE):
        r, c = 0, j
        rc = r + c

        Iy[rc] = -opening - (j - 1) * extension
        M[rc] = Ix[rc] = MININT

    for i in range(1, ROWSIZE):
        for j in range(1, COLSIZE):
            # Precompute coordinates
            r, c = i * COLSIZE, j
            r1, c1 = (i - 1) * COLSIZE, j - 1

            rc = r + c
            r1c = r1 + c
            rc1 = r + c1
            r1c1 = r1 + c1

            # Gaps on X
            m = M[r1c] - opening
            ix = Ix[r1c] - extension
            Ix[rc] = max(m, ix)

            # Gaps on Y
            m = M[rc1] - opening
            iy = Iy[rc1] - extension
            Iy[rc] = max(m, iy)

            # Matches/Mismatches
            # - 1 for seqa/seqb because sequences start at 0
            mscore = match if (sa[i] & sb[j]) else mmatch
            m = M[r1c1] + mscore
            ix = Ix[r1c1] + mscore
            iy = Iy[r1c1] + mscore

            M[rc] = max(m, ix, iy)

    # Last Matrix field has the best score
    return M[MSIZE - 1]


def nw_align(seqa, seqb):
    lena = len(seqa)
    lenb = len(seqb)

    f = []
    for i in range(0, lena + 1):
        f.append([0] * (lenb + 1))

    f[0][0] = 0
    for i in range(1, lena + 1):
        f[i][0] = INDEL_EXT * i

    for j in range(1, lenb + 1):
        f[0][j] = INDEL_EXT * i

    for i in range(1, lena + 1):
        for j in range(1, lenb + 1):
            m = MATCH if seqa[i - 1] == seqb[j - 1] else MISMATCH
            match = f[i - 1][j - 1] + m
            delete = f[i-1][j] + INDEL_EXT
            insert = f[i][j - 1] + INDEL_EXT
            f[i][j] = max(match, delete, insert)

    print(f)
    score = f[lena][lenb]
    return score


if __name__ == '__main__':
    import sys

    test_cases = open(sys.argv[1], 'r')
    for test in test_cases:
        tstrip = test.rstrip('\r\n')
        if not tstrip:
            continue

        seqa, seqb = tstrip.split('|')
        score = nw_affine_gap_opt(seqa.strip(), seqb.strip())
        print(score)

    test_cases.close()
