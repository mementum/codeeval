SIMPLE OR TRUMP
===============

CHALLENGE DESCRIPTION:
----------------------

First playing cards were invented in Eastern Asia and then spread all over the
world taking different forms and appearance. In India, playing cards were
round, and they were called Ganjifa. In medieval Japan, there was a popular
Uta-garuta game, in which shell mussels were used instead of playing cards.

In our game, we use playing cards that are more familiar nowadays. The rules
are also simple: an ace beats a deuce (2) unless it is a trump deuce.

INPUT SAMPLE:
-------------
The first argument is a path to a file. Each line includes a test case which
contains two playing cards and a trump suit. Cards and a trump suite are
separated by a pipeline (|). The card deck includes thirteen ranks (from a
deuce to an ace) of each of the four suits: clubs (♣), diamonds (♦), hearts
(♥), and spades (♠). There are no Jokers.
::

   AD 2H | H
   KD KH | C
   JH 10S | C

OUTPUT SAMPLE:
--------------

Your task is to print a card that wins. If there is no trump card, then the
higher card wins. If the cards are equal, then print both cards.
::

   2H
   KD KH
   JH

CONSTRAINTS:
------------

  1. The card deck includes ranks from a deuce (2) to an ace, no Jokers.
  2. If the cards are equal, then print both cards.
  3. The number of test cases is 40.
