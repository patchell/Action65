This is yet another attempt on my part to create an ACTION! cross compiler 
for the 6502.

Compatibility Issues with the real ACTION!
1. Action! used the '*' character to mean "Current Location".
	Unfortunately, this has a conflict with the meaning
	of Multiply, so I had to make the current location
	Symbol be '.*'  :(
	Hopefully, in the future I will find a way to fix
	this conflict.

I do not know why the AST is all messed up.  In the Github
editor it looks just fine, but not when displayed.
Go figure

Cleaned up the AST.


Feb 10, 2025

Added, finally, the binary file.  Please keep in mind this
thing is no where near complete.  But, you still might have
some fun with it.  It most likely will either crash, or hang 
if you do anything that hasn't beeb finished, which covers
pretty much the whole thing.

The only file that I know will compile is "test3.act".

	 NUMB  CHILD  NEXT
   207      4     -1  +- 'ROOT'
     4      1      7     +- 'INT'
     1     -1      2     |  +- 'IDENT': aa
     2     -1      3     |  +- 'IDENT': bb
     3     -1     -1     |  +- 'IDENT': cc
     7      5      9     +- 'CARD'
     5     -1      6     |  +- 'IDENT': dd
     6     -1     -1     |  +- 'IDENT': ee
     9      8     15     +- 'CHAR'
     8     -1     -1     |  +- 'IDENT': ff
    15     10    206     +- 'BYTE'
    10     -1     11     |  +- 'IDENT': gg
    11     -1     12     |  +- 'IDENT': hh
    12     -1     13     |  +- 'IDENT': ii
    13     -1     14     |  +- 'IDENT': jj
    14     -1     -1     |  +- 'IDENT': kk
   206     23     -1     +- 'MODULE'
    23     22    202        +- 'DEFINE'
    22     21     -1        |  +- 'SECTION'
    21     20     -1        |     +- 'SECTION Name'
    20     17     -1        |        +- 'SECTION Attributes'
    17     16     19        |           +- 'ATRB-START'
    16     -1     -1        |           |  +- 'NUMBER'$0200
    19     18     -1        |           +- 'SIZE'
    18     -1     -1        |              +- 'NUMBER'$0400
   202     24    205        +- 'IRQ-PROC'
    24     25     -1        |  +- 'IDENT': AnInteruption
    25     -1    201        |     +- 'Param LIST'
   201     38     -1        |     +- 'Body'
    38     27    200        |        +- 'LOCAL Vars'
    27     26     31        |        |  +- 'CHAR'
    26     -1     -1        |        |  |  +- 'IDENT': a
    31     28     33        |        |  +- 'INT'
    28     -1     29        |        |  |  +- 'IDENT': x
    29     -1     30        |        |  |  +- 'IDENT': xx
    30     -1     -1        |        |  |  +- 'IDENT': xxx
    33     32     35        |        |  +- 'INT'
    32     -1     -1        |        |  |  +- 'IDENT': c
    35     34     37        |        |  +- 'CHAR'
    34     -1     -1        |        |  |  +- 'IDENT': d
    37     36     -1        |        |  +- 'CHAR'
    36     -1     -1        |        |     +- 'IDENT': e
   200     39     -1        |        +- 'STATEMENTS'
    39     47     58        |           +- 'IDENT': e
    47     46     -1        |           |  +- 'Assign ='
    46     45     -1        |           |     +- 'SUB'
    45     -1     44        |           |        +- 'IDENT': xx
    44     43     -1        |           |        +- 'ADD'
    43     42     40        |           |           +- 'MUL'
    42     -1     41        |           |           |  +- 'IDENT': a
    41     -1     -1        |           |           |  +- 'IDENT': xxx
    40     -1     -1        |           |           +- 'IDENT': x
    58     48    198        |           +- 'DO'
    48     52     56        |           |  +- 'IDENT': e
    52     51     -1        |           |  |  +- 'Assign ='
    51     50     -1        |           |  |     +- 'SUB'
    50     -1     49        |           |  |        +- 'IDENT': x
    49     -1     -1        |           |  |        +- 'IDENT': x
    56     55     57        |           |  +- 'UNTILL'
    55     54     -1        |           |  |  +- 'EQU-TO'
    54     -1     53        |           |  |     +- 'IDENT': xx
    53     -1     -1        |           |  |     +- 'IDENT': e
    57     -1     -1        |           |  +- 'OD'
   198     60    199        |           +- 'ASM'
    60     59     67        |           |  +- 'PROCESSOR'
    59     -1     -1        |           |  |  +- 'R6502'
    67     66     69        |           |  +- 'SECTION'
    66     65     -1        |           |  |  +- 'SECTION Name'
    65     62     -1        |           |  |     +- 'SECTION Attributes'
    62     61     64        |           |  |        +- 'ATRB-START'
    61     -1     -1        |           |  |        |  +- 'NUMBER'$F000
    64     63     -1        |           |  |        +- 'SIZE'
    63     -1     -1        |           |  |           +- 'NUMBER'$1000
    69     68     70        |           |  +- 'ORG'
    68     -1     -1        |           |  |  +- 'NUMBER'$4000
    70     -1     72        |           |  +- 'LABEL'
    72     71     74        |           |  +- 'DB'
    71     -1     -1        |           |  |  +- 'NUMBER'$0005
    74     73     76        |           |  +- 'DW'
    73     -1     -1        |           |  |  +- 'NUMBER'$0257
    76     75     77        |           |  +- 'DL'
    75     -1     -1        |           |  |  +- 'NUMBER'$864B2
    77     -1     79        |           |  +- 'LABEL'
    79     78     80        |           |  +- 'DS'
    78     -1     -1        |           |  |  +- 'NUMBER'$0A7F
    80     -1     82        |           |  +- 'LABEL'
    82     81     84        |           |  +- 'OPCODE' ADC ($71) INDIRECT INDEXED
    81     -1     -1        |           |  |  +- 'NUMBER'$0052
    84     83     86        |           |  +- 'OPCODE' LDA ($A5) ZERO PAGE
    83     -1     -1        |           |  |  +- 'LABEL'
    86     85     87        |           |  +- 'OPCODE' LDA ($A9) IMMEDIATE
    85     -1     -1        |           |  |  +- 'NUMBER'$0055
    87     -1     88        |           |  +- 'OPCODE' PHA ($48) IMPLIED
    88     -1     89        |           |  +- 'OPCODE' TYA ($98) IMPLIED
    89     -1     90        |           |  +- 'LABEL'
    90     -1     92        |           |  +- 'OPCODE' PHA ($48) IMPLIED
    92     91     94        |           |  +- 'OPCODE' JMP ($4C) ABSOLUTE
    91     -1     -1        |           |  |  +- 'NUMBER'$BAD1
    94     93     96        |           |  +- 'OPCODE' JMP ($6C) INDIRECT
    93     -1     -1        |           |  |  +- 'NUMBER'$432A
    96     95     97        |           |  +- 'OPCODE' JSR ($20) ABSOLUTE
    95     -1     -1        |           |  |  +- 'NUMBER'$F00D
    97     -1     98        |           |  +- 'OPCODE' PLA ($68) IMPLIED
    98     -1    105        |           |  +- 'OPCODE' TAY ($A8) IMPLIED
   105     99    107        |           |  +- 'Asm PROC'
    99    100     -1        |           |  |  +- 'PROC name' - 'AsmProc'
   100     -1    102        |           |  |     +- 'OPCODE' PLA ($68) IMPLIED
   102    101    103        |           |  |     +- 'OPCODE' STA ($8D) ABSOLUTE
   101     -1     -1        |           |  |     |  +- 'NUMBER'$1000
   103     -1    104        |           |  |     +- 'OPCODE' RTS ($60) IMPLIED
   104     -1     -1        |           |  |     +- 'OPCODE' RTI ($40) IMPLIED
   107    106    109        |           |  +- 'OPCODE' ADC ($61) INDEXED INDIRECT
   106     -1     -1        |           |  |  +- 'NUMBER'$0044
   109    108    112        |           |  +- 'OPCODE' SBC ($E5) ZERO PAGE
   108     -1     -1        |           |  |  +- 'NUMBER'$0012
   112    110    115        |           |  +- 'OPCODE' CMP ($D5) PAGE ZERO,X
   110    111     -1        |           |  |  +- 'NUMBER'$0013
   111     -1     -1        |           |  |     +- '.X'
   115    113    118        |           |  +- 'OPCODE' AND ($3D) ABSOLUTE,X
   113    114     -1        |           |  |  +- 'NUMBER'$0400
   114     -1     -1        |           |  |     +- '.X'
   118    116    120        |           |  +- 'OPCODE' ORA ($19) ABSOLUTE,Y
   116    117     -1        |           |  |  +- 'NUMBER'$0410
   117     -1     -1        |           |  |     +- '.Y'
   120    119    122        |           |  +- 'OPCODE' CPX ($E0) IMMEDIATE
   119     -1     -1        |           |  |  +- 'NUMBER'$0017
   122    121    124        |           |  +- 'OPCODE' CPX ($E4) ZERO PAGE
   121     -1     -1        |           |  |  +- 'NUMBER'$00B0
   124    123    126        |           |  +- 'OPCODE' CPX ($EC) ABSOLUTE
   123     -1     -1        |           |  |  +- 'NUMBER'$A8EE
   126    125    129        |           |  +- 'OPCODE' ASL ($06) ZERO PAGE
   125     -1     -1        |           |  |  +- 'NUMBER'$008C
   129    127    131        |           |  +- 'OPCODE' ASL ($16) PAGE ZERO,X
   127    128     -1        |           |  |  +- 'NUMBER'$008B
   128     -1     -1        |           |  |     +- '.X'
   131    130    134        |           |  +- 'OPCODE' ASL ($0E) ABSOLUTE
   130     -1     -1        |           |  |  +- 'NUMBER'$AACC
   134    132    135        |           |  +- 'OPCODE' ASL ($1E) ABSOLUTE,X
   132    133     -1        |           |  |  +- 'NUMBER'$CCAA
   133     -1     -1        |           |  |     +- '.X'
   135     -1    137        |           |  +- 'OPCODE' ASL ($0A) ACCUMULATOR
   137    136    139        |           |  +- 'OPCODE' LDX ($A2) IMMEDIATE
   136     -1     -1        |           |  |  +- 'NUMBER'$001F
   139    138    142        |           |  +- 'OPCODE' LDX ($A6) ZERO PAGE
   138     -1     -1        |           |  |  +- 'NUMBER'$00C1
   142    140    144        |           |  +- 'OPCODE' LDX ($B6) PAGE ZERO,Y
   140    141     -1        |           |  |  +- 'NUMBER'$00C2
   141     -1     -1        |           |  |     +- '.Y'
   144    143    147        |           |  +- 'OPCODE' LDX ($AE) ABSOLUTE
   143     -1     -1        |           |  |  +- 'NUMBER'$56AB
   147    145    149        |           |  +- 'OPCODE' LDX ($BE) ABSOLUTE,Y
   145    146     -1        |           |  |  +- 'NUMBER'$64BE
   146     -1     -1        |           |  |     +- '.Y'
   149    148    151        |           |  +- 'OPCODE' LDY ($A0) IMMEDIATE
   148     -1     -1        |           |  |  +- 'NUMBER'$001F
   151    150    154        |           |  +- 'OPCODE' LDY ($A4) ZERO PAGE
   150     -1     -1        |           |  |  +- 'NUMBER'$00C1
   154    152    156        |           |  +- 'OPCODE' LDY ($B4) PAGE ZERO,X
   152    153     -1        |           |  |  +- 'NUMBER'$00C2
   153     -1     -1        |           |  |     +- '.X'
   156    155    159        |           |  +- 'OPCODE' LDY ($AC) ABSOLUTE
   155     -1     -1        |           |  |  +- 'NUMBER'$56AB
   159    157    161        |           |  +- 'OPCODE' LDY ($BC) ABSOLUTE,X
   157    158     -1        |           |  |  +- 'NUMBER'$64BE
   158     -1     -1        |           |  |     +- '.X'
   161    160    164        |           |  +- 'OPCODE' STY ($84) ZERO PAGE
   160     -1     -1        |           |  |  +- 'NUMBER'$007C
   164    162    166        |           |  +- 'OPCODE' STY ($94) PAGE ZERO,X
   162    163     -1        |           |  |  +- 'NUMBER'$007D
   163     -1     -1        |           |  |     +- '.X'
   166    165    168        |           |  +- 'OPCODE' STY ($8C) ABSOLUTE
   165     -1     -1        |           |  |  +- 'NUMBER'$D00D
   168    167    171        |           |  +- 'OPCODE' STX ($86) ZERO PAGE
   167     -1     -1        |           |  |  +- 'NUMBER'$0062
   171    169    173        |           |  +- 'OPCODE' STX ($96) PAGE ZERO,Y
   169    170     -1        |           |  |  +- 'NUMBER'$0063
   170     -1     -1        |           |  |     +- '.Y'
   173    172    175        |           |  +- 'OPCODE' STX ($8E) ABSOLUTE
   172     -1     -1        |           |  |  +- 'NUMBER'$B00B
   175    174    178        |           |  +- 'OPCODE' INC ($E6) ZERO PAGE
   174     -1     -1        |           |  |  +- 'NUMBER'$00D0
   178    176    180        |           |  +- 'OPCODE' INC ($F6) PAGE ZERO,X
   176    177     -1        |           |  |  +- 'NUMBER'$0062
   177     -1     -1        |           |  |     +- '.X'
   180    179    183        |           |  +- 'OPCODE' INC ($EE) ABSOLUTE
   179     -1     -1        |           |  |  +- 'NUMBER'$63DA
   183    181    185        |           |  +- 'OPCODE' INC ($FE) ABSOLUTE,X
   181    182     -1        |           |  |  +- 'NUMBER'$B00B
   182     -1     -1        |           |  |     +- '.X'
   185    184    188        |           |  +- 'OPCODE' DEC ($C6) ZERO PAGE
   184     -1     -1        |           |  |  +- 'NUMBER'$00D0
   188    186    190        |           |  +- 'OPCODE' DEC ($D6) PAGE ZERO,X
   186    187     -1        |           |  |  +- 'NUMBER'$0062
   187     -1     -1        |           |  |     +- '.X'
   190    189    193        |           |  +- 'OPCODE' DEC ($CE) ABSOLUTE
   189     -1     -1        |           |  |  +- 'NUMBER'$63DA
   193    191    195        |           |  +- 'OPCODE' DEC ($DE) ABSOLUTE,X
   191    192     -1        |           |  |  +- 'NUMBER'$B00B
   192     -1     -1        |           |  |     +- '.X'
   195    194    197        |           |  +- 'OPCODE' BIT ($24) ZERO PAGE
   194     -1     -1        |           |  |  +- 'NUMBER'$00A8
   197    196     -1        |           |  +- 'OPCODE' BIT ($2C) ABSOLUTE
   196     -1     -1        |           |     +- 'NUMBER'$9AB0
   199     -1     -1        |           +- 'RTI'
   205    203     -1        +- 'INT'
   203     -1    204           +- 'IDENT': qr
   204     -1     -1           +- 'IDENT': st

Also, if you want to compile the program, you will need at
least C++ 20


Feb 8, 2025

Making loads of progress.  The inline assembler AST geration is
about 1/2 done.  Still need to test the branch instructions and
all of the directives.  The AST for my tests is really starting to
get quite long.


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
