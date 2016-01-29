MEET COMB SORT
==============

CHALLENGE DESCRIPTION:
----------------------

Comb sort is a simplified sorting algorithm, an improvement of a bubble sort
algorithm. It competes in speed with a well-known quick sort algorithm.

The main idea of this sorting algorithm is that a gap between elements can be
more than 1.

Such algorithm is used in this challenge.


INPUT SAMPLE:
-------------

The first argument is a path to a file. Every row includes a test case with
numbers that you need to sort using comb sort algorithm.
::

   3 1 2
   5 4 3 2 1

OUTPUT SAMPLE:
--------------

Count and print number of iterations it will take to sort the test case using
comb sort. Iteration is a pass through the list of numbers using the same range
between the compared elements. When a pass starts from the beginning – it’s a
new iteration. If a range is, for example, 3, then take elements 0 and 3, then
1 and 4 etc.
::

   2
   3

CONSTRAINTS:
------------

  1. Decrease factor for the algorithm is 1.25; round the result to a smaller
     number.

  2. If a range is, for example, 3, then take elements 0 and 3, then 1 and 4
     etc.

  3. One iteration for this algorithm is a pass through the list of numbers to
     the end using the same range between the compared elements.

  4. The number of test cases is 40.
