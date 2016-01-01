DISTINCT SUBSEQUENCES
=====================

CHALLENGE DESCRIPTION:
----------------------

A subsequence of a given sequence S consists of S with zero or more elements
deleted. Formally, a sequence Z = z1z2..zk is a subsequence of X = x1x2...xm,
if there exists a strictly increasing sequence of indicies of X such that for
all j=1,2,...k we have Xij = Zj. E.g. Z=bcdb is a subsequence of X=abcbdab with
corresponding index sequence <2,3,5,7>

INPUT SAMPLE:
-------------

Your program should accept as its first argument a path to a filename. Each
line in this file contains two comma separated strings. The first is the
sequence X and the second is the subsequence Z. E.g.
::
   babgbag,bag
   rabbbit,rabbit

OUTPUT SAMPLE:
--------------

Print out the number of distinct occurrences of Z in X as a subsequence E.g.
::
   5
   3
