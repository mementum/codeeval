PRISONER OR CITIZEN
===================

CHALLENGE DESCRIPTION:
---------------------

No matter where you are, there are always good and bad people everywhere, those
who have never broken the law, and those who are constantly abusing it.

The goal of our challenge is not to persuade you to obey the laws, become
better, or vice versa. Everyone has a right to make his own life choice.

Your task is to find out where a person is—in jail or at large—depending on the
coordinates.

INPUT SAMPLE:
-------------

The first argument is a path to a file. Each line includes a test case with the
coordinates of jail and a person, separated by a pipeline '|'. All coordinates
are in the (x, y) format. Coordinates of a jail, which form a jail square, are
separated by comma ','.

Coordinates of a prison should be united in the provided sequence (as shown on
the picture).
::

  1 1, 1 4, 3 4, 3 2 | 2 3
  1 1, 3 2, 1 4, 3 4 | 3 3
  1 1, 1 3, 3 3, 3 1 | 1 2

OUTPUT SAMPLE:
--------------

Find out where a person is—in jail or at large—depending on the coordinates. If
in jail, print Prisoner; otherwise, print Citizen.
::

  Prisoner
  Citizen
  Prisoner

CONSTRAINTS:
------------

All coordinates are from 0 to 10 and cannot be negative.

Coordinates of a prison should be united in the provided sequence (as shown on
the picture).

The number of coordinates of a prison can be from 3 to 12.

If coordinates of a person fall into the prison line, print Prisoner (see input
sample # 3).  The number of test cases is 20.
