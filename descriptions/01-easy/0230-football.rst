FOOTBALL
========

CHALLENGE DESCRIPTION:
----------------------

People around the world watch football matches and root for different football
teams. Some people are fans of Real Madrid, some like Barcelona, some pull for
Atletico Madrid, while others do not miss a single match with FC Bayern Munich.

The teams would like to know people in which countries cheer for them. So,
let’s help them!

INPUT SAMPLE:
-------------

The first argument is a path to a file. Each row includes a test case with
lists of countries. Lists are separated by pipelines '|'. Each list includes
football teams that people in these countries root for.

For example:
::

   1 2 3 4 | 3 1 | 4 1
   19 11 | 19 21 23 | 31 39 29

OUTPUT SAMPLE:
--------------

For each football team, print a list of countries where people root for
them. Separate each team by a semicolon ';' and a space. All output should be
sorted.

For example:
::

   1:1,2,3; 2:1; 3:1,2; 4:1,3;
   11:1; 19:1,2; 21:2; 23:2; 29:3; 31:3; 39:3;

CONSTRAINTS:
------------

  1. The number of countries lists can be from 3 to 20.

  2. Each list contains a different number of football teams: from 1 to 7.

  3. The number of test cases is 40.
