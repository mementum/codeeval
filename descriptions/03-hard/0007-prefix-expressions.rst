PREFIX EXPRESSIONS
==================

CHALLENGE DESCRIPTION:
----------------------

You are given a prefix expression. Write a program which evaluates it.

INPUT SAMPLE:
-------------

Your program should accept a file as its first argument. The file contains one
prefix expression per line.

For example:
::
   * + 2 3 4

Your program should read this file and insert it into any data
structure you like. Traverse this data structure and evaluate the prefix
expression. Each token is delimited by a whitespace. You may assume that sum
‘+’, multiplication ‘*’ and division ‘/’ are the only valid operators appearing
in the test data.

OUTPUT SAMPLE:
--------------

Print to stdout the output of the prefix expression, one per line.

For example:
::
   20

CONSTRAINTS:
------------

  1. The evaluation result will always be an integer ≥ 0.

  2. The number of the test cases is ≤ 40.
