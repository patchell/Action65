This is yet another attempt on my part to create an ACTION! cross compiler 
for the 6502.

Jan 31, 2025

By Jove, I think I got it.  Had a bit of confusion about how
building an AST went, after looking at code I wrote about 12 years
ago, I realized I was sadly mistaken.  All straitened out now, I hope.
So here are the current state of afairs.

Source:

INT aa,bb,cc
CARD dd,ee
CHAR ff
BYTE gg,hh,ii,jj,kk

MODULE

INTERRUPT AnInteruption()
	CHAR a
	INT x,xx,xxx
	INT c
	CHAR d
	CHAR e

	xyz = a + x - c + d - e
	POP .A
	BREAK
	EXIT
	RTI

INT qr,st

Abstract Syntax Tree

	 NUMB  CHILD  NEXT
    54      4     -1  +- 'ROOT'
     4      1      7  |  +- 'INT'
     1     -1      2  |  |  +- 'IDENT': aa
     2     -1      3  |  |  +- 'IDENT': bb
     3     -1     -1  |  |  +- 'IDENT': cc
     7      5      9  |  +- 'CARD'
     5     -1      6  |  |  +- 'IDENT': dd
     6     -1     -1  |  |  +- 'IDENT': ee
     9      8     15  |  +- 'CHAR'
     8     -1     -1  |  |  +- 'IDENT': ff
    15     10     53  |  +- 'BYTE'
    10     -1     11  |  |  +- 'IDENT': gg
    11     -1     12  |  |  +- 'IDENT': hh
    12     -1     13  |  |  +- 'IDENT': ii
    13     -1     14  |  |  +- 'IDENT': jj
    14     -1     -1  |  |  +- 'IDENT': kk
    53     49     -1  |  +- 'MODULE'
    49     16     52  |  |  +- 'IRQ-PROC'
    16     17     -1  |  |  |  +- 'IDENT': AnInteruption
    17     -1     48  |  |  |  |  +- 'Param LIST'
    48     30     -1  |  |  |  |  +- 'Body'
    30     19     47  |  |  |  |  |  +- 'LOCAL Vars'
    19     18     23  |  |  |  |  |  |  +- 'CHAR'
    18     -1     -1  |  |  |  |  |  |  |  +- 'IDENT': a
    23     20     25  |  |  |  |  |  |  +- 'INT'
    20     -1     21  |  |  |  |  |  |  |  +- 'IDENT': x
    21     -1     22  |  |  |  |  |  |  |  +- 'IDENT': xx
    22     -1     -1  |  |  |  |  |  |  |  +- 'IDENT': xxx
    25     24     27  |  |  |  |  |  |  +- 'INT'
    24     -1     -1  |  |  |  |  |  |  |  +- 'IDENT': c
    27     26     29  |  |  |  |  |  |  +- 'CHAR'
    26     -1     -1  |  |  |  |  |  |  |  +- 'IDENT': d
    29     28     -1  |  |  |  |  |  |  +- 'CHAR'
    28     -1     -1  |  |  |  |  |  |  |  +- 'IDENT': e
    47     41     -1  |  |  |  |  |  +- 'STATEMENTS'
    41     40     43  |  |  |  |  |  |  +- 'Assign ='
    40     39     31  |  |  |  |  |  |  |  +- 'SUB'
    39     -1     38  |  |  |  |  |  |  |  |  +- 'IDENT': e
    38     37     -1  |  |  |  |  |  |  |  |  +- 'ADD'
    37     -1     36  |  |  |  |  |  |  |  |  |  +- 'IDENT': d
    36     35     -1  |  |  |  |  |  |  |  |  |  +- 'SUB'
    35     -1     34  |  |  |  |  |  |  |  |  |  |  +- 'IDENT': c
    34     33     -1  |  |  |  |  |  |  |  |  |  |  +- 'ADD'
    33     -1     32  |  |  |  |  |  |  |  |  |  |  |  +- 'IDENT': x
    32     -1     -1  |  |  |  |  |  |  |  |  |  |  |  +- 'IDENT': a
    31     -1     -1  |  |  |  |  |  |  |  +- 'IDENT': xyz
    43     42     44  |  |  |  |  |  |  +- 'POP'
    42     -1     -1  |  |  |  |  |  |  |  +- '.A'
    44     -1     45  |  |  |  |  |  |  +- 'BREAK'
    45     -1     46  |  |  |  |  |  |  +- 'EXIT'
    46     -1     -1  |  |  |  |  |  |  +- 'RTI'
    52     50     -1  |  |  +- 'INT'
    50     -1     51  |  |  |  +- 'IDENT': qr
    51     -1     -1  |  |  |  +- 'IDENT': st

Jan 26, 2025

A bit more progress.  Starting to get AST generation for proceedures
and Statements, although very soparse.


Jan 18, 2025

Finally, I hope, I have generating AST trees down.  I am,
so far, only generating 6 different nodes, and hopefully
they will start falling in place.  Only about 130 to go.


December 26, 2024.
Things are progressing.  While the intent is to create a
fully compatable ACTION! compiler, And so far, it is looking that
way, that hasn't stopped me from making a few additions.  One
of the things I want to be able to do is produce fully ROM able 
code for my various 6502/Non-Atari projects.  While it is
possible to ROM the original ACITON! code, at best it is
extremely clumbsy, at worst, close to unusable.  So here are the
intended additions:

	1 Parameters:  An options to pass parameters on the stack or
or to have them alocated statically as in the Original.

	2 Local Variables:  An options for these to be allocated
either statically, as in the original, or have them allocated dynamically
on a "stack like" structure.

	3 IFF - THEN - ELSE - FFI Statements.  This is different that the 
IF-FI statement that is in the original ACTION! compiler.  This statement
is from the Rockwell PL/65 compiler, and I thought it was 
useful enough to include.  It is a bill lower level than the 
ACTION! IF-FI statment.  The original ACTION! if statements
is also included.

	4. PUSH/PUSHW: push data or registers onto the stack.  I thought this
would come in handy, also from PL/65.

	5. POP/POPW: pop data from the stack into registers or memrory.

	6. BREAK: Break Instruction (BRK).  Also from PL/65.

	7. BIT/BITS:  Test bits in memory.  A;sp frp, PL/65.

	8. ASM:  Inline Assembly code.  This is real Assembly code,
Not the same as the Code Block.

	9. BOOL:  Boolen data type, like the C/C++ bool type.  Probably
not really needed, but I thought it would make code a
little bit clearer.

	10. INTERRUPT:  Interrupt type proceedure.  Can't take
parameters, and uses RTI to return.

	11. VECTOR defines a verctor locations.  This can be
used so set the RESET/IRQ/NMI vectors, or indirect JMP
vectors.

	12 DECLARE : Used like a C/C++ function prototype/declaration..
This will allow for calling functions that are not yet 
defined in the program.  This will, however, potentially
a dangerous feature as it could be used to implement
recursion in the program.  This is why there will be an
option for dynamic local variables and parameters passed
on the stack.

	13. ROL: operator, just like the ROL asm statment.

	14  ROR: operator, just like the ROR asm statement.

In the original ACTION! compiler, you declared your global
variables, and then your function definitions.  You could not declare any
more global variables unless you used the MODULE keyword.

I am not exactly sure how I managed to cure this problem,
but the current grammar allows you to freely mix global
declarations and function definitions any way you please.
This happend while I was trying to solve the ambiguty
that is caused by declaring global variables and FUNCtions:

------------ Example 1 -------------------
MODULE
INT a,b,c

INT FUNC Afunction()

CHAR x,y,z	;In original ACTION!, this statement is not allowed.

------------- Example 2 -------------------

MODULE
INT a,b,c

INT FUNC Afunction()

MODULE		; If we add another MODULE statement here
CHAR x,y,z	;In original ACTION!, this statement is now allowed.
-----------------------------------------------
In this version, either will just the same, or you can
just eliminate the MODULE statement entrirely.

It sure looks easy enough, and it is if you are using an
LR grammar, but to make the compiler friendly to anybody
who might want to modify the compiler, I made it LL(1) using
Recursive/Descent techniques.  Using an LR (LALR) parser
pretty much requires a parser generator (YACC, Bison, Anagram, etc.).
Of course, you can get parser generators for LL(k) parsers,
most notable ANTLR, but they are easy enough to write, so why
bother. (NOTE:  ANTLR is an LL(k) parser generator, which means
that you can have as many lookaheads as you want.  The parser
for this compiler is strictly LL(1), which means it only has
one lookahead.)


November 1, 2024.
Working the kinks out of the Grammar (action.gram).  There were a few
places where I overthought things just a bit.  "There is just no way
that is correct!", but I was wrong, it was right, so I had to change it to what 
it was supposed to be.
So, if you thought you would turn this in as your compiler design class
project, well, it won't happen this quarter.  Maybe next quarter. ;)
-------
As of Oct 30, 2024 I am in the process of testing the parser.
The parser in an LL(1) grammar that is implemented with a recursive
decent parseer.  Or, at least, I hope so.  I have expanded the language a little
bit, such as adding a BOOL type, adding an INTERRUPT proceedure type,
and in line assembly with syntax checking.

This is all the easy part.  The hard part is code generation.  I purchased
a new book, that I hope, will give me new insights on how to go about
doing this sort of thing.
