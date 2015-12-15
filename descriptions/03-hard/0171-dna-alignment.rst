DNA ALIGNMENT
=============

CHALLENGE DESCRIPTION:
----------------------

In this challenge, you have to deal with biological code: DNA sequences. DNA
sequences are made up of four bases: A, C, T, and G. New DNA sequences are
formed by copying the existing ones. However, sometimes, this process is
accompanied by errors: one base may be replaced with another; fragments may be
deleted or inserted.

Your task is to compare two related sequences that parted from one another
million years ago and changed during this time.

To do this, bioinformaticians use sequence alignment: they present them in the
form of a matrix, indicating gaps with a minus sign (-), so that the maximum
number of identical bases (symbols) are under each other.

For example, for GAAAAAAT and GAAT sequences, one of the possible alignments
will be the following:
::
  GAAAAAAT
  G--A–A–T

Scoring systems are used to assess the alignment. These are rules, according to
which the alignment receives:

  - positive points — for each match (when the symbols in two lines match)
  - negative points — for mismatch (different symbols in two lines)
  - negative points — for indel (an insertion or deletion, i.e. a gap in one of
    the lines)

In some scoring systems, indel start “costs“ more than indel extension, because
one big gap is biologically more likely to appear than two or more smaller
gaps.

In this challenge, we use the following scoring system:
::
  Match: +3
  Mismatch: -3
  Indel start: -8
  Indel extension: -1

Thus, the score for the above sequence is calculated as follows:
::
   G  A  A  A  A  A  A  T
   G  -  -  A  –  A  –  T
  +3 -8 -1 +3 -8 +3 -8 +3 = -13

Another version of the same sequences alignment:
::
   G  A  A  A  A  A  A  T
   G  A  A  -  -  -  -  T
  +3 +3 +3 -8 -1 -1 -1 +3 = 1

For most pairs of sequences, there is more than one optimal alignment. Your
task is to find the highest possible score of such alignments.

INPUT SAMPLE:
-------------

Your program should accept a path to a file as its first argument. Each line of
the file is a pair of DNA sequences separated by vertical bar (|).
::

  GAAAAAAT | GAAT
  GCATGCT | GATTACA

OUTPUT SAMPLE:
--------------

Print to stdout the score of the best possible alignment taking into account
the above scoring system.
::
   1
  -3

CONSTRAINTS:
------------

The length of each sequence does not exceed 400 symbols.
