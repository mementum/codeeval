BRAINF*CK
=========

CHALLENGE DESCRIPTION:
----------------------

Looking for something utterly mind-blowing? Look no further, you hit the right
place!

This challenge is inextricably linked with Brainf*ck, the most famous esoteric
programming language invented by Urban Müller. Is this the first time you hear
about Brainf*ck? Find out more Brainf*ck (wiki)

Brainf*ck consists of only 8 basic commands:
::
  > - move to the next cell;

  < - move to the previous cell;

  + - increment the value in the current cell by 1;

  - - decrement the value of the current cell by 1;

  . - output the value of the current cell;

  , - input the value outside and store it in the current cell;

  [ - if the value of the current cell is zero, move forward on the text to the
    program to] taking into account nesting;

  ] - if the value of the current cell is not zero, go back on the text of the
    program to [considering nesting;

So, you should write a program that will get the code on the Brainf*ck
language, calculate this code, and display the final result of the program. It
can be a simple output of letters or a complex cycle; in any case, your program
should handle it.

INPUT SAMPLE:
-------------

The first argument is a path to a file. Each line includes a test case where
there is a program written on the Brainf*ck language.

For example:
::
  +[--->++<]>+++.[->+++++++<]>.[--->+<]>----.
  ++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.

OUTPUT SAMPLE:
--------------

You should print the result of the program that you will get.

For example:
::
  Yo!
  Hello World!

CONSTRAINTS:
------------

Programs can include the code of any complexity.

The number of test cases is 10.
