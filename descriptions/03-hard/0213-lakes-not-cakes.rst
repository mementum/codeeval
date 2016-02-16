LAKES, NOT CAKES
================

CHALLENGE DESCRIPTION:
----------------------

It was a dark and stormy night when Alice was wandering in the black
forest. The rain fell in torrents into deep lakes scattered all over the area…
Wait! Lakes… forest…? Really? Well, yeah, it’s not a true horror story, but it
is fits our challenge perfectly. So, you have a map of the forest. You are sure
there are some lakes in there, but you do not know their number.

Write a program that will count how many lakes are in the forest. We count all
adjacent o symbols as one lake (by adjacent we mean symbols that are located
one cell up, down, left, right, or diagonally from the needed symbol).

INPUT SAMPLE:
-------------

The first argument is a path to a file. Each line includes a test case, which
contains a map of the forest of different size. Forest areas are separated by a
vertical bar |.

  # - forest
  o - lake

For example:
::

   o # o | # # # | o # o
   o # o | # o # | o # o

OUTPUT SAMPLE:
--------------

Print the number of lakes for each test case.

For example:
::

   4
   1

CONSTRAINTS:
------------

  1. A forest may be of different width and height from 3 to 30 symbols.

  2. Diagonal symbols are also counted as adjacent.

  3. The number of test cases is 40.
