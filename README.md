This is yet another attempt on my part to create an ACTION! cross compiler 
for the 6502.

Compatability Issues with the real ACTION!
1. Action! used the '*' character to mean "Current Location".
	Unfortunately, this has a conflict with the meaning
	of Multiply, so I had to make the current location
	Symbol be '.*'  :(
	Hopefully, in the future I will find a way to fix
	this conflict.



Feb 10, 2025

Added, finally, the binary file.  Please keep in mind this
thing is no where near complete.  But, you still might have
some fun with it.  It most likely will either crash, or hang 
if you do anything that hasn't beeb finished, which covers
pretty much the whole thing.

The only file that I know will compile is "test3.act"

Also, if you want to compile the program, you will need at
least C++ 20

Feb 8, 2025

Making loads of progress.  The inline assembler AST geration is
about 1/2 done.  Still need to test the branch instructions and
all of the directives.  The AST for my tests is really starting to
get quite long.

	 NUMB  CHILD  NEXT
   150      4     -1  +- 'ROOT'
     4      1      7  |  +- 'INT'
     1     -1      2  |  |  +- 'IDENT': aa
     2     -1      3  |  |  +- 'IDENT': bb
     3     -1     -1  |  |  +- 'IDENT': cc
     7      5      9  |  +- 'CARD'
     5     -1      6  |  |  +- 'IDENT': dd
     6     -1     -1  |  |  +- 'IDENT': ee
     9      8     15  |  +- 'CHAR'
     8     -1     -1  |  |  +- 'IDENT': ff
    15     10    149  |  +- 'BYTE'
    10     -1     11  |  |  +- 'IDENT': gg
    11     -1     12  |  |  +- 'IDENT': hh
    12     -1     13  |  |  +- 'IDENT': ii
    13     -1     14  |  |  +- 'IDENT': jj
    14     -1     -1  |  |  +- 'IDENT': kk
   149    145     -1  |  +- 'MODULE'
   145     16    148  |  |  +- 'IRQ-PROC'
    16     17     -1  |  |  |  +- 'IDENT': AnInteruption
    17     -1    144  |  |  |  |  +- 'Param LIST'
   144     30     -1  |  |  |  |  +- 'Body'
    30     19    143  |  |  |  |  |  +- 'LOCAL Vars'
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
   143    141     -1  |  |  |  |  |  +- 'STATEMENTS'
   141     31    142  |  |  |  |  |  |  +- 'ASM'
    31     -1     33  |  |  |  |  |  |  |  +- 'LABEL'+- 'START' ($0000)
    33     32     35  |  |  |  |  |  |  |  +- 'OPCODE' ADC ($71) INDIRECT INDEXED
    32     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0052
    35     34     36  |  |  |  |  |  |  |  +- 'OPCODE' LDA ($A9) IMMEDIATE
    34     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0055
    36     -1     37  |  |  |  |  |  |  |  +- 'OPCODE' PHA ($48) IMPLIED
    37     -1     38  |  |  |  |  |  |  |  +- 'OPCODE' TYA ($98) IMPLIED
    38     -1     39  |  |  |  |  |  |  |  +- 'LABEL'+- 'HERE' ($0000)
    39     -1     41  |  |  |  |  |  |  |  +- 'OPCODE' PHA ($48) IMPLIED
    41     40     43  |  |  |  |  |  |  |  +- 'OPCODE' JMP ($4C) ABSOLUTE
    40     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$BAD1
    43     42     45  |  |  |  |  |  |  |  +- 'OPCODE' JMP ($6C) INDIRECT
    42     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$432A
    45     44     46  |  |  |  |  |  |  |  +- 'OPCODE' JSR ($20) ABSOLUTE
    44     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$F00D
    46     -1     47  |  |  |  |  |  |  |  +- 'OPCODE' PLA ($68) IMPLIED
    47     -1     48  |  |  |  |  |  |  |  +- 'OPCODE' TAY ($A8) IMPLIED
    48     -1     50  |  |  |  |  |  |  |  +- 'OPCODE' PLA ($68) IMPLIED
    50     49     51  |  |  |  |  |  |  |  +- 'OPCODE' STA ($8D) ABSOLUTE
    49     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$1000
    51     -1     52  |  |  |  |  |  |  |  +- 'OPCODE' RTS ($60) IMPLIED
    52     -1     54  |  |  |  |  |  |  |  +- 'OPCODE' RTI ($40) IMPLIED
    54     53     56  |  |  |  |  |  |  |  +- 'OPCODE' ADC ($61) INDEXED INDIRECT
    53     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0044
    56     55     59  |  |  |  |  |  |  |  +- 'OPCODE' SBC ($E5) ZERO PAGE
    55     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0012
    59     57     62  |  |  |  |  |  |  |  +- 'OPCODE' CMP ($D5) PAGE ZERO,X
    57     58     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0013
    58     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
    62     60     65  |  |  |  |  |  |  |  +- 'OPCODE' AND ($3D) ABSOLUTE,X
    60     61     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0400
    61     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
    65     63     67  |  |  |  |  |  |  |  +- 'OPCODE' ORA ($19) ABSOLUTE,Y
    63     64     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0410
    64     -1     -1  |  |  |  |  |  |  |  |  |  +- '.Y'
    67     66     69  |  |  |  |  |  |  |  +- 'OPCODE' CPX ($E0) IMMEDIATE
    66     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0017
    69     68     71  |  |  |  |  |  |  |  +- 'OPCODE' CPX ($E4) ZERO PAGE
    68     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00B0
    71     70     73  |  |  |  |  |  |  |  +- 'OPCODE' CPX ($EC) ABSOLUTE
    70     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$A8EE
    73     72     76  |  |  |  |  |  |  |  +- 'OPCODE' ASL ($06) ZERO PAGE
    72     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$008C
    76     74     78  |  |  |  |  |  |  |  +- 'OPCODE' ASL ($16) PAGE ZERO,X
    74     75     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$008B
    75     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
    78     77     81  |  |  |  |  |  |  |  +- 'OPCODE' ASL ($0E) ABSOLUTE
    77     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$AACC
    81     79     82  |  |  |  |  |  |  |  +- 'OPCODE' ASL ($1E) ABSOLUTE,X
    79     80     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$CCAA
    80     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
    82     -1     84  |  |  |  |  |  |  |  +- 'OPCODE' ASL ($0A) ACCUMULATOR
    84     83     86  |  |  |  |  |  |  |  +- 'OPCODE' LDX ($A2) IMMEDIATE
    83     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$001F
    86     85     89  |  |  |  |  |  |  |  +- 'OPCODE' LDX ($A6) ZERO PAGE
    85     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00C1
    89     87     91  |  |  |  |  |  |  |  +- 'OPCODE' LDX ($B6) PAGE ZERO,Y
    87     88     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00C2
    88     -1     -1  |  |  |  |  |  |  |  |  |  +- '.Y'
    91     90     94  |  |  |  |  |  |  |  +- 'OPCODE' LDX ($AE) ABSOLUTE
    90     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$56AB
    94     92     96  |  |  |  |  |  |  |  +- 'OPCODE' LDX ($BE) ABSOLUTE,Y
    92     93     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$64BE
    93     -1     -1  |  |  |  |  |  |  |  |  |  +- '.Y'
    96     95     98  |  |  |  |  |  |  |  +- 'OPCODE' LDY ($A0) IMMEDIATE
    95     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$001F
    98     97    101  |  |  |  |  |  |  |  +- 'OPCODE' LDY ($A4) ZERO PAGE
    97     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00C1
   101     99    103  |  |  |  |  |  |  |  +- 'OPCODE' LDY ($B4) PAGE ZERO,X
    99    100     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00C2
   100     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   103    102    106  |  |  |  |  |  |  |  +- 'OPCODE' LDY ($AC) ABSOLUTE
   102     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$56AB
   106    104    108  |  |  |  |  |  |  |  +- 'OPCODE' LDY ($BC) ABSOLUTE,X
   104    105     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$64BE
   105     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   108    107    111  |  |  |  |  |  |  |  +- 'OPCODE' STY ($84) ZERO PAGE
   107     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$007C
   111    109    113  |  |  |  |  |  |  |  +- 'OPCODE' STY ($94) PAGE ZERO,X
   109    110     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$007D
   110     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   113    112    115  |  |  |  |  |  |  |  +- 'OPCODE' STY ($8C) ABSOLUTE
   112     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$D00D
   115    114    118  |  |  |  |  |  |  |  +- 'OPCODE' STX ($86) ZERO PAGE
   114     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0062
   118    116    120  |  |  |  |  |  |  |  +- 'OPCODE' STX ($96) PAGE ZERO,Y
   116    117     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0063
   117     -1     -1  |  |  |  |  |  |  |  |  |  +- '.Y'
   120    119    122  |  |  |  |  |  |  |  +- 'OPCODE' STX ($8E) ABSOLUTE
   119     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$B00B
   122    121    125  |  |  |  |  |  |  |  +- 'OPCODE' INC ($E6) ZERO PAGE
   121     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00D0
   125    123    127  |  |  |  |  |  |  |  +- 'OPCODE' INC ($F6) PAGE ZERO,X
   123    124     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0062
   124     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   127    126    130  |  |  |  |  |  |  |  +- 'OPCODE' INC ($EE) ABSOLUTE
   126     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$63DA
   130    128    132  |  |  |  |  |  |  |  +- 'OPCODE' INC ($FE) ABSOLUTE,X
   128    129     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$B00B
   129     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   132    131    135  |  |  |  |  |  |  |  +- 'OPCODE' DEC ($C6) ZERO PAGE
   131     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$00D0
   135    133    137  |  |  |  |  |  |  |  +- 'OPCODE' DEC ($D6) PAGE ZERO,X
   133    134     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$0062
   134     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   137    136    140  |  |  |  |  |  |  |  +- 'OPCODE' DEC ($CE) ABSOLUTE
   136     -1     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$63DA
   140    138     -1  |  |  |  |  |  |  |  +- 'OPCODE' DEC ($DE) ABSOLUTE,X
   138    139     -1  |  |  |  |  |  |  |  |  +- 'NUMBER'$B00B
   139     -1     -1  |  |  |  |  |  |  |  |  |  +- '.X'
   142     -1     -1  |  |  |  |  |  |  +- 'RTI'
   148    146     -1  |  |  +- 'INT'
   146     -1    147  |  |  |  +- 'IDENT': qr
   147     -1     -1  |  |  |  +- 'IDENT': st

Jan 31, 2025

By Jove, I think I got it.  Had a bit of confusion about how
building an AST went, after looking at code I wrote about 12 years
ago, I realized I was sadly mistaken.  All straitened out now, I hope.
So here are the current state of afairs.


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
