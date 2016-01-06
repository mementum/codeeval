STRING SEARCHING
================

CHALLENGE DESCRIPTION:
----------------------

You are given two strings. Determine if the second string is a substring of the
first (Do NOT use any substr type library function). The second string may
contain an asterisk(*) which should be treated as a regular expression
i.e. matches zero or more characters. The asterisk can be escaped by a \ char
in which case it should be interpreted as a regular '*' character. To
summarize: the strings can contain alphabets, numbers, * and \ characters.

INPUT SAMPLE:
-------------

Your program should accept as its first argument a path to a filename. The
input file contains two comma delimited strings per line. E.g.
::
   Hello,ell
   This is good, is
   CodeEval,C*Eval
   Old,Young

OUTPUT SAMPLE:
--------------

If the second string is indeed a substring of the first, print out a
'true'(lowercase), else print out a 'false'(lowercase), one per line. E.g.
::
   true
   true
   true
   false
