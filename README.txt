This is yet another attempt on my part to create an ACTION! cross compiler  
for the 6502.

Compatibility Issues with the real ACTION!
1. Action! used the '*' character to mean "Current Location".
	Unfortunately, this has a conflict with the meaning
	of Multiply, so I had to make the current location
	Symbol be '.*'  :(
	Hopefully, in the future I will find a way to fix
	this conflict.


Cleaned up the AST.


Feb 10, 2025

Added, finally, the binary file.  Please keep in mind this
thing is no where near complete.  But, you still might have
some fun with it.  It most likely will either crash, or hang 
if you do anything that hasn't beeb finished, which covers
pretty much the whole thing.

The only file that I know will compile is "test3.act".


Also, if you want to compile the program, you will need at
least C++ 20.


	 NUMB  CHILD  NEXT
   292      4     -1  +- 'ROOT'
     4      1      7     +- 'INT'
     1     -1      2     |  +- 'IDENT': aa
     2     -1      3     |  +- 'IDENT': bb
     3     -1     -1     |  +- 'IDENT': cc
     7      5      9     +- 'CARD'
     5     -1      6     |  +- 'IDENT': dd
     6     -1     -1     |  +- 'IDENT': ee
     9      8     15     +- 'CHAR'
     8     -1     -1     |  +- 'IDENT': ff
    15     10    291     +- 'BYTE'
    10     -1     11     |  +- 'IDENT': gg
    11     -1     12     |  +- 'IDENT': hh
    12     -1     13     |  +- 'IDENT': ii
    13     -1     14     |  +- 'IDENT': jj
    14     -1     -1     |  +- 'IDENT': kk
   291     23     -1     +- 'MODULE'
    23     22    287        +- 'DEFINE'
    22     21     -1        |  +- 'SECTION'
    21     20     -1        |     +- 'SECTION Name'
    20     17     -1        |        +- 'SECTION Attributes'
    17     16     19        |           +- 'ATRB-START'
    16     -1     -1        |           |  +- 'NUMBER'$0200
    19     18     -1        |           +- 'SIZE'
    18     -1     -1        |              +- 'NUMBER'$0400
   287     24    290        +- 'IRQ-PROC'
    24     25     -1        |  +- 'IDENT': AnInteruption
    25     -1    286        |     +- 'Param LIST'
   286     38     -1        |     +- 'Body'
    38     27    285        |        +- 'LOCAL Vars'
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
   285     44     -1        |        +- 'STATEMENTS'
    44     39     62        |           +- 'CODE-BLOCK'
    39     -1     40        |           |  +- 'NUMBER'$00AA
    40     -1     41        |           |  +- 'NUMBER'$00BB
    41     -1     42        |           |  +- 'NUMBER'$00CE
    42     -1     43        |           |  +- 'NUMBER'$00A1
    43     -1     -1        |           |  +- 'NUMBER'$00C0
    62     54     76        |           +- 'DO'
    54     47     58        |           |  +- 'IF'
    47     46     52        |           |  |  +- 'EQU-TO'
    46     -1     45        |           |  |  |  +- 'IDENT': xx
    45     -1     -1        |           |  |  |  +- 'IDENT': c
    52     49     53        |           |  |  +- 'THEN'
    49     48     51        |           |  |  |  +- 'PUSH'
    48     -1     -1        |           |  |  |  |  +- '.A'
    51     50     -1        |           |  |  |  +- 'POP'
    50     -1     -1        |           |  |  |     +- '.Y'
    53     -1     -1        |           |  |  +- 'FI'
    58     57     60        |           |  +- 'UNTILL'
    57     56     -1        |           |  |  +- 'NEQ'
    56     -1     55        |           |  |     +- 'IDENT': gg
    55     -1     -1        |           |  |     +- 'IDENT': jj
    60     59     61        |           |  +- 'PUSH'
    59     -1     -1        |           |  |  +- '.A'
    61     -1     -1        |           |  +- 'OD'
    76     65     82        |           +- 'IF'
    65     64     68        |           |  +- '>'
    64     -1     63        |           |  |  +- 'IDENT': gg
    63     -1     -1        |           |  |  +- 'IDENT': xxx
    68     66     74        |           |  +- 'THEN'
    66     -1     67        |           |  |  +- 'BREAK'
    67     -1     -1        |           |  |  +- 'EXIT'
    74     70     75        |           |  +- 'ELSE'
    70     69     73        |           |  |  +- '=ADD'
    69     -1     71        |           |  |  |  +- 'IDENT': gg
    71     -1     -1        |           |  |  |  +- 'IDENT': e
    73     72     -1        |           |  |  +- 'PUSH'
    72     -1     -1        |           |  |     +- '.X'
    75     -1     -1        |           |  +- 'FI'
    82     77     91        |           +- 'CODE-BLOCK'
    77     -1     78        |           |  +- 'NUMBER'$00AA
    78     -1     79        |           |  +- 'NUMBER'$00BB
    79     -1     80        |           |  +- 'NUMBER'$00CE
    80     -1     81        |           |  +- 'NUMBER'$00A1
    81     -1     -1        |           |  +- 'NUMBER'$00C0
    91     84     93        |           +- 'DO'
    84     83     90        |           |  +- 'Assign ='
    83     -1     89        |           |  |  +- 'IDENT': c
    89     88     -1        |           |  |  +- 'SUB'
    88     87     85        |           |  |     +- 'MUL'
    87     -1     86        |           |  |     |  +- 'IDENT': xx
    86     -1     -1        |           |  |     |  +- 'IDENT': x
    85     -1     -1        |           |  |     +- 'IDENT': e
    90     -1     -1        |           |  +- 'OD'
    93     92    111        |           +- 'Assign ='
    92     -1    100        |           |  +- 'IDENT': d
   100     99     -1        |           |  +- 'SUB'
    99     -1     98        |           |     +- 'IDENT': xx
    98     97     -1        |           |     +- 'ADD'
    97     96     94        |           |        +- 'MUL'
    96     -1     95        |           |        |  +- 'IDENT': a
    95     -1     -1        |           |        |  +- 'IDENT': xxx
    94     -1     -1        |           |        +- 'IDENT': x
   111    103    132        |           +- 'WHILE'
   103    102    109        |           |  +- '>='
   102     -1    101        |           |  |  +- 'IDENT': d
   101     -1     -1        |           |  |  +- 'IDENT': c
   109    105    110        |           |  +- 'DO'
   105    104     -1        |           |  |  +- 'Assign ='
   104     -1    108        |           |  |     +- 'IDENT': ff
   108    107     -1        |           |  |     +- 'ADD'
   107     -1    106        |           |  |        +- 'NUMBER'$0001
   106     -1     -1        |           |  |        +- 'IDENT': d
   110     -1     -1        |           |  +- 'OD'
   132    112    143        |           +- 'FOR'
   112     -1    116        |           |  +- 'IDENT': a
   116    115    120        |           |  +- 'Assign ='
   115    114     -1        |           |  |  +- 'ADD'
   114     -1    113        |           |  |     +- 'IDENT': xx
   113     -1     -1        |           |  |     +- 'IDENT': x
   120    119    122        |           |  +- 'TO'
   119    118     -1        |           |  |  +- 'SUB'
   118     -1    117        |           |  |     +- 'IDENT': c
   117     -1     -1        |           |  |     +- 'IDENT': c
   122    121    130        |           |  +- 'STEP'
   121     -1     -1        |           |  |  +- 'NUMBER'$0003
   130    124    131        |           |  +- 'DO'
   124    123     -1        |           |  |  +- 'Assign ='
   123     -1    129        |           |  |     +- 'IDENT': dd
   129    128     -1        |           |  |     +- 'DIV'
   128     -1    127        |           |  |        +- 'IDENT': e
   127    126     -1        |           |  |        +- 'MUL'
   126     -1    125        |           |  |           +- 'NUMBER'$0008
   125     -1     -1        |           |  |           +- 'IDENT': d
   131     -1     -1        |           |  +- 'OD'
   143    134    283        |           +- 'DO'
   134    133    141        |           |  +- 'Assign ='
   133     -1    137        |           |  |  +- 'IDENT': gg
   137    136     -1        |           |  |  +- 'SUB'
   136     -1    135        |           |  |     +- 'IDENT': x
   135     -1     -1        |           |  |     +- 'IDENT': x
   141    140    142        |           |  +- 'UNTILL'
   140    139     -1        |           |  |  +- 'EQU-TO'
   139     -1    138        |           |  |     +- 'IDENT': xx
   138     -1     -1        |           |  |     +- 'IDENT': e
   142     -1     -1        |           |  +- 'OD'
   283    145    284        |           +- 'ASM'
   145    144    152        |           |  +- 'PROCESSOR'
   144     -1     -1        |           |  |  +- 'R6502'
   152    151    154        |           |  +- 'SECTION'
   151    150     -1        |           |  |  +- 'SECTION Name'
   150    147     -1        |           |  |     +- 'SECTION Attributes'
   147    146    149        |           |  |        +- 'ATRB-START'
   146     -1     -1        |           |  |        |  +- 'NUMBER'$F000
   149    148     -1        |           |  |        +- 'SIZE'
   148     -1     -1        |           |  |           +- 'NUMBER'$1000
   154    153    155        |           |  +- 'ORG'
   153     -1     -1        |           |  |  +- 'NUMBER'$4000
   155     -1    157        |           |  +- 'LABEL' - 'SomeData'
   157    156    159        |           |  +- 'DB'
   156     -1     -1        |           |  |  +- 'NUMBER'$0005
   159    158    161        |           |  +- 'DW'
   158     -1     -1        |           |  |  +- 'NUMBER'$0257
   161    160    162        |           |  +- 'DL'
   160     -1     -1        |           |  |  +- 'NUMBER'$864B2
   162     -1    164        |           |  +- 'LABEL' - 'Storage'
   164    163    165        |           |  +- 'DS'
   163     -1     -1        |           |  |  +- 'NUMBER'$0A7F
   165     -1    167        |           |  +- 'LABEL' - 'START'
   167    166    169        |           |  +- 'OPCODE' ADC ($71) INDIRECT INDEXED
   166     -1     -1        |           |  |  +- 'NUMBER'$0052
   169    168    171        |           |  +- 'OPCODE' LDA ($A5) ZERO PAGE
   168     -1     -1        |           |  |  +- 'LABEL' - 'xxx'
   171    170    172        |           |  +- 'OPCODE' LDA ($A9) IMMEDIATE
   170     -1     -1        |           |  |  +- 'NUMBER'$0055
   172     -1    173        |           |  +- 'OPCODE' PHA ($48) IMPLIED
   173     -1    174        |           |  +- 'OPCODE' TYA ($98) IMPLIED
   174     -1    175        |           |  +- 'LABEL' - 'HERE'
   175     -1    177        |           |  +- 'OPCODE' PHA ($48) IMPLIED
   177    176    179        |           |  +- 'OPCODE' JMP ($4C) ABSOLUTE
   176     -1     -1        |           |  |  +- 'NUMBER'$BAD1
   179    178    181        |           |  +- 'OPCODE' JMP ($6C) INDIRECT
   178     -1     -1        |           |  |  +- 'NUMBER'$432A
   181    180    182        |           |  +- 'OPCODE' JSR ($20) ABSOLUTE
   180     -1     -1        |           |  |  +- 'NUMBER'$F00D
   182     -1    183        |           |  +- 'OPCODE' PLA ($68) IMPLIED
   183     -1    190        |           |  +- 'OPCODE' TAY ($A8) IMPLIED
   190    184    192        |           |  +- 'Asm PROC'
   184    185     -1        |           |  |  +- 'PROC name' - 'AsmProc'
   185     -1    187        |           |  |     +- 'OPCODE' PLA ($68) IMPLIED
   187    186    188        |           |  |     +- 'OPCODE' STA ($8D) ABSOLUTE
   186     -1     -1        |           |  |     |  +- 'NUMBER'$1000
   188     -1    189        |           |  |     +- 'OPCODE' RTS ($60) IMPLIED
   189     -1     -1        |           |  |     +- 'OPCODE' RTI ($40) IMPLIED
   192    191    194        |           |  +- 'OPCODE' ADC ($61) INDEXED INDIRECT
   191     -1     -1        |           |  |  +- 'NUMBER'$0044
   194    193    197        |           |  +- 'OPCODE' SBC ($E5) ZERO PAGE
   193     -1     -1        |           |  |  +- 'NUMBER'$0012
   197    195    200        |           |  +- 'OPCODE' CMP ($D5) PAGE ZERO,X
   195    196     -1        |           |  |  +- 'NUMBER'$0013
   196     -1     -1        |           |  |     +- '.X'
   200    198    203        |           |  +- 'OPCODE' AND ($3D) ABSOLUTE,X
   198    199     -1        |           |  |  +- 'NUMBER'$0400
   199     -1     -1        |           |  |     +- '.X'
   203    201    205        |           |  +- 'OPCODE' ORA ($19) ABSOLUTE,Y
   201    202     -1        |           |  |  +- 'NUMBER'$0410
   202     -1     -1        |           |  |     +- '.Y'
   205    204    207        |           |  +- 'OPCODE' CPX ($E0) IMMEDIATE
   204     -1     -1        |           |  |  +- 'NUMBER'$0017
   207    206    209        |           |  +- 'OPCODE' CPX ($E4) ZERO PAGE
   206     -1     -1        |           |  |  +- 'NUMBER'$00B0
   209    208    211        |           |  +- 'OPCODE' CPX ($EC) ABSOLUTE
   208     -1     -1        |           |  |  +- 'NUMBER'$A8EE
   211    210    214        |           |  +- 'OPCODE' ASL ($06) ZERO PAGE
   210     -1     -1        |           |  |  +- 'NUMBER'$008C
   214    212    216        |           |  +- 'OPCODE' ASL ($16) PAGE ZERO,X
   212    213     -1        |           |  |  +- 'NUMBER'$008B
   213     -1     -1        |           |  |     +- '.X'
   216    215    219        |           |  +- 'OPCODE' ASL ($0E) ABSOLUTE
   215     -1     -1        |           |  |  +- 'NUMBER'$AACC
   219    217    220        |           |  +- 'OPCODE' ASL ($1E) ABSOLUTE,X
   217    218     -1        |           |  |  +- 'NUMBER'$CCAA
   218     -1     -1        |           |  |     +- '.X'
   220     -1    222        |           |  +- 'OPCODE' ASL ($0A) ACCUMULATOR
   222    221    224        |           |  +- 'OPCODE' LDX ($A2) IMMEDIATE
   221     -1     -1        |           |  |  +- 'NUMBER'$001F
   224    223    227        |           |  +- 'OPCODE' LDX ($A6) ZERO PAGE
   223     -1     -1        |           |  |  +- 'NUMBER'$00C1
   227    225    229        |           |  +- 'OPCODE' LDX ($B6) PAGE ZERO,Y
   225    226     -1        |           |  |  +- 'NUMBER'$00C2
   226     -1     -1        |           |  |     +- '.Y'
   229    228    232        |           |  +- 'OPCODE' LDX ($AE) ABSOLUTE
   228     -1     -1        |           |  |  +- 'NUMBER'$56AB
   232    230    234        |           |  +- 'OPCODE' LDX ($BE) ABSOLUTE,Y
   230    231     -1        |           |  |  +- 'NUMBER'$64BE
   231     -1     -1        |           |  |     +- '.Y'
   234    233    236        |           |  +- 'OPCODE' LDY ($A0) IMMEDIATE
   233     -1     -1        |           |  |  +- 'NUMBER'$001F
   236    235    239        |           |  +- 'OPCODE' LDY ($A4) ZERO PAGE
   235     -1     -1        |           |  |  +- 'NUMBER'$00C1
   239    237    241        |           |  +- 'OPCODE' LDY ($B4) PAGE ZERO,X
   237    238     -1        |           |  |  +- 'NUMBER'$00C2
   238     -1     -1        |           |  |     +- '.X'
   241    240    244        |           |  +- 'OPCODE' LDY ($AC) ABSOLUTE
   240     -1     -1        |           |  |  +- 'NUMBER'$56AB
   244    242    246        |           |  +- 'OPCODE' LDY ($BC) ABSOLUTE,X
   242    243     -1        |           |  |  +- 'NUMBER'$64BE
   243     -1     -1        |           |  |     +- '.X'
   246    245    249        |           |  +- 'OPCODE' STY ($84) ZERO PAGE
   245     -1     -1        |           |  |  +- 'NUMBER'$007C
   249    247    251        |           |  +- 'OPCODE' STY ($94) PAGE ZERO,X
   247    248     -1        |           |  |  +- 'NUMBER'$007D
   248     -1     -1        |           |  |     +- '.X'
   251    250    253        |           |  +- 'OPCODE' STY ($8C) ABSOLUTE
   250     -1     -1        |           |  |  +- 'NUMBER'$D00D
   253    252    256        |           |  +- 'OPCODE' STX ($86) ZERO PAGE
   252     -1     -1        |           |  |  +- 'NUMBER'$0062
   256    254    258        |           |  +- 'OPCODE' STX ($96) PAGE ZERO,Y
   254    255     -1        |           |  |  +- 'NUMBER'$0063
   255     -1     -1        |           |  |     +- '.Y'
   258    257    260        |           |  +- 'OPCODE' STX ($8E) ABSOLUTE
   257     -1     -1        |           |  |  +- 'NUMBER'$B00B
   260    259    263        |           |  +- 'OPCODE' INC ($E6) ZERO PAGE
   259     -1     -1        |           |  |  +- 'NUMBER'$00D0
   263    261    265        |           |  +- 'OPCODE' INC ($F6) PAGE ZERO,X
   261    262     -1        |           |  |  +- 'NUMBER'$0062
   262     -1     -1        |           |  |     +- '.X'
   265    264    268        |           |  +- 'OPCODE' INC ($EE) ABSOLUTE
   264     -1     -1        |           |  |  +- 'NUMBER'$63DA
   268    266    270        |           |  +- 'OPCODE' INC ($FE) ABSOLUTE,X
   266    267     -1        |           |  |  +- 'NUMBER'$B00B
   267     -1     -1        |           |  |     +- '.X'
   270    269    273        |           |  +- 'OPCODE' DEC ($C6) ZERO PAGE
   269     -1     -1        |           |  |  +- 'NUMBER'$00D0
   273    271    275        |           |  +- 'OPCODE' DEC ($D6) PAGE ZERO,X
   271    272     -1        |           |  |  +- 'NUMBER'$0062
   272     -1     -1        |           |  |     +- '.X'
   275    274    278        |           |  +- 'OPCODE' DEC ($CE) ABSOLUTE
   274     -1     -1        |           |  |  +- 'NUMBER'$63DA
   278    276    280        |           |  +- 'OPCODE' DEC ($DE) ABSOLUTE,X
   276    277     -1        |           |  |  +- 'NUMBER'$B00B
   277     -1     -1        |           |  |     +- '.X'
   280    279    282        |           |  +- 'OPCODE' BIT ($24) ZERO PAGE
   279     -1     -1        |           |  |  +- 'NUMBER'$00A8
   282    281     -1        |           |  +- 'OPCODE' BIT ($2C) ABSOLUTE
   281     -1     -1        |           |     +- 'NUMBER'$9AB0
   284     -1     -1        |           +- 'RTI'
   290    288     -1        +- 'INT'
   288     -1    289           +- 'IDENT': qr
   289     -1     -1           +- 'IDENT': st

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
