NOT SO CLEVER
=============

CHALLENGE DESCRIPTION:
----------------------

Imagine that you have to arrange items in a certain order: pencils from black
to white in a color palette, photographs by the date taken, banknotes from the
highest to the lowest, etc. To do this, you definitely don’t need to use the
Stupid sort algorithm.

After each action, you need to come back to the beginning and start all over
again. Not so clever, is it? But, you need to know about this algorithm, that’s
why it is used in this challenge.

INPUT SAMPLE:
-------------

The first argument is a path to a file. Each line includes a test case which
contains numbers that you need to sort using the Stupid sort algorithm. There
is also a number of iterations for an algorithm to carry out. The numbers
themselves and the number of iterations are separated by a pipeline '|'.
::

   4 3 2 1 | 1
   5 4 3 2 1 | 2

OUTPUT SAMPLE:
--------------

Print sorted numbers after they pass the required number of iterations. One
iteration of this sort is a pass to the moment of making changes. Once changing
the order of the digits, passing starts from the very beginning. Hence, this is
another iteration.
::

   3 4 2 1
   4 3 5 2 1

CONSTRAINTS:
------------

  1. The number of iterations can be from 1 to 8.

  2. One iteration of this sort is a pass to the moment of making changes.

  3. The number of test cases is 40.
