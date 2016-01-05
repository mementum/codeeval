DECODE NUMBERS
==============

CHALLENGE DESCRIPTION:
----------------------

You are given an encoded message containing only numbers. You are also provided
with the following mapping:
::
   A : 1
   B : 2
   C : 3
   ...
   Z : 26

Given an encoded message, count the number of ways it can be decoded.

INPUT SAMPLE:
-------------

Your program should accept as its first argument a path to a filename. Each
line in this file is a test-case and contains an encoded message of
numbers. E.g.
::
   12
   123

You may assume that the test cases contain only numbers.

OUTPUT SAMPLE:
--------------

Print out the different number of ways it can be decoded. E.g.
::
   2
   3

NOTE: 12 could be decoded as AB(1 2) or L(12). Hence the number of ways to
decode 12 is 2.
