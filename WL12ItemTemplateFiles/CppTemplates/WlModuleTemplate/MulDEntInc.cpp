$if$ (0) /*Copyright (c) 2025 by Wayne E. Wright, W5XD
This template was converted from the version used for the old vsz template technology.
The old style used [!if ]. The new one uses the "if" that commands the processor to skip this very commentary.
The difference between the two is (a) that the old tech supported nested "if" clauses while the new one does not,
and (b) the old style supported && and || operators and the new one only supports string compare for "if" clauses.
The way I made this work was to extract all the compounded conditional expressions that in the old tech were
evaluated by the template processor, and make the wizard here evaluate the expressions and write a separate
for each expression.*/$endif$#include "pch.h"
#include "MultDEnt.cpp"
