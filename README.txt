This is yet another attempt on my part to create an ACTION! cross compiler  
for the 6502.

Compatibility Issues with the real ACTION!
1. Action! used the '*' character to mean "Current Location".
	Unfortunately, this has a conflict with the meaning
	of Multiply, so I had to make the current location
	Symbol be '.*'  :(
	Hopefully, in the future I will find a way to fix
	this conflict.


Feb 15, 2025
To a large extent, but still not complete, all of the major
language features are now working...I think.  So I will
continue to write ACTION screpts to cover the rest.
Is this fun or what?  ;)

OH heck, just realized I haven't done TYPE and paramer lists.

	 NUMB  CHILD  NEXT
   316      4     -1  +- 'ROOT'
     4      1      7     +- 'INT'
     1     -1      2     |  +- 'IDENT': aa
     2     -1      3     |  +- 'IDENT': bb
     3     -1     -1     |  +- 'IDENT': cc
     7      5      9     +- 'CARD'
     5     -1      6     |  +- 'IDENT': dd
     6     -1     -1     |  +- 'IDENT': ee
     9      8     15     +- 'CHAR'
     8     -1     -1     |  +- 'IDENT': ff
    15     10    315     +- 'BYTE'
    10     -1     11     |  +- 'IDENT': gg
    11     -1     12     |  +- 'IDENT': hh
    12     -1     13     |  +- 'IDENT': ii
    13     -1     14     |  +- 'IDENT': jj
    14     -1     -1     |  +- 'IDENT': kk
   315     23     -1     +- 'MODULE'
    23     22    311        +- 'DEFINE'
    22     21     -1        |  +- 'SECTION'
    21     20     -1        |     +- 'SECTION Name'
    20     17     -1        |        +- 'SECTION Attributes'
    17     16     19        |           +- 'ATRB-START'
    16     -1     -1        |           |  +- 'NUMBER'$0200
    19     18     -1        |           +- 'SIZE'
    18     -1     -1        |              +- 'NUMBER'$0400
   311     24    314        +- 'IRQ-PROC'
    24     25     -1        |  +- 'IDENT': AnInteruption
    25     -1    310        |     +- 'Param LIST'
   310     38     -1        |     +- 'Body'
    38     27    309        |        +- 'LOCAL Vars'
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
   309     44     -1        |        +- 'STATEMENTS'
    44     39     53        |           +- 'CODE-BLOCK'
    39     -1     40        |           |  +- 'NUMBER'$00AA
    40     -1     41        |           |  +- 'NUMBER'$00BB
    41     -1     42        |           |  +- 'NUMBER'$00CE
    42     -1     43        |           |  +- 'NUMBER'$00A1
    43     -1     -1        |           |  +- 'NUMBER'$00C0
    53     47     60        |           +- 'IFF'
    47     46     51        |           |  +- 'EQU-TO'
    46     -1     45        |           |  |  +- 'NUMBER'$0000
    45     -1     -1        |           |  |  +- '.A'
    51     49     52        |           |  +- 'THEN'
    49     48     -1        |           |  |  +- '=SUB'
    48     -1     50        |           |  |     +- 'IDENT': jj
    50     -1     -1        |           |  |     +- 'NUMBER'$0002
    52     -1     -1        |           |  +- 'FFI'
    60     54     68        |           +- 'IFF'
    54     -1     58        |           |  +- '.N'
    58     56     59        |           |  +- 'THEN'
    56     55     -1        |           |  |  +- '=MOD'
    55     -1     57        |           |  |     +- 'IDENT': kk
    57     -1     -1        |           |  |     +- 'NUMBER'$0005
    59     -1     -1        |           |  +- 'FFI'
    68     62     86        |           +- 'IFF'
    62     61     66        |           |  +- 'PL/65 BITS'
    61     -1     -1        |           |  |  +- 'NUMBER'$0005
    66     64     67        |           |  +- 'THEN'
    64     63     -1        |           |  |  +- '=LSH'
    63     -1     65        |           |  |     +- 'IDENT': ii
    65     -1     -1        |           |  |     +- 'NUMBER'$0002
    67     -1     -1        |           |  +- 'FFI'
    86     78    100        |           +- 'DO'
    78     71     82        |           |  +- 'IF'
    71     70     76        |           |  |  +- 'EQU-TO'
    70     -1     69        |           |  |  |  +- 'IDENT': xx
    69     -1     -1        |           |  |  |  +- 'IDENT': c
    76     73     77        |           |  |  +- 'THEN'
    73     72     75        |           |  |  |  +- 'PUSH'
    72     -1     -1        |           |  |  |  |  +- '.A'
    75     74     -1        |           |  |  |  +- 'POP'
    74     -1     -1        |           |  |  |     +- '.Y'
    77     -1     -1        |           |  |  +- 'FI'
    82     81     84        |           |  +- 'UNTILL'
    81     80     -1        |           |  |  +- 'NEQ'
    80     -1     79        |           |  |     +- 'IDENT': gg
    79     -1     -1        |           |  |     +- 'IDENT': jj
    84     83     85        |           |  +- 'PUSH'
    83     -1     -1        |           |  |  +- '.A'
    85     -1     -1        |           |  +- 'OD'
   100     89    106        |           +- 'IF'
    89     88     92        |           |  +- '>'
    88     -1     87        |           |  |  +- 'IDENT': gg
    87     -1     -1        |           |  |  +- 'IDENT': xxx
    92     90     98        |           |  +- 'THEN'
    90     -1     91        |           |  |  +- 'BREAK'
    91     -1     -1        |           |  |  +- 'EXIT'
    98     94     99        |           |  +- 'ELSE'
    94     93     97        |           |  |  +- '=ADD'
    93     -1     95        |           |  |  |  +- 'IDENT': gg
    95     -1     -1        |           |  |  |  +- 'IDENT': e
    97     96     -1        |           |  |  +- 'PUSH'
    96     -1     -1        |           |  |     +- '.X'
    99     -1     -1        |           |  +- 'FI'
   106    101    115        |           +- 'CODE-BLOCK'
   101     -1    102        |           |  +- 'NUMBER'$00AA
   102     -1    103        |           |  +- 'NUMBER'$00BB
   103     -1    104        |           |  +- 'NUMBER'$00CE
   104     -1    105        |           |  +- 'NUMBER'$00A1
   105     -1     -1        |           |  +- 'NUMBER'$00C0
   115    108    117        |           +- 'DO'
   108    107    114        |           |  +- 'Assign ='
   107     -1    113        |           |  |  +- 'IDENT': c
   113    112     -1        |           |  |  +- 'SUB'
   112    111    109        |           |  |     +- 'MUL'
   111     -1    110        |           |  |     |  +- 'IDENT': xx
   110     -1     -1        |           |  |     |  +- 'IDENT': x
   109     -1     -1        |           |  |     +- 'IDENT': e
   114     -1     -1        |           |  +- 'OD'
   117    116    135        |           +- 'Assign ='
   116     -1    124        |           |  +- 'IDENT': d
   124    123     -1        |           |  +- 'SUB'
   123     -1    122        |           |     +- 'IDENT': xx
   122    121     -1        |           |     +- 'ADD'
   121    120    118        |           |        +- 'MUL'
   120     -1    119        |           |        |  +- 'IDENT': a
   119     -1     -1        |           |        |  +- 'IDENT': xxx
   118     -1     -1        |           |        +- 'IDENT': x
   135    127    156        |           +- 'WHILE'
   127    126    133        |           |  +- '>='
   126     -1    125        |           |  |  +- 'IDENT': d
   125     -1     -1        |           |  |  +- 'IDENT': c
   133    129    134        |           |  +- 'DO'
   129    128     -1        |           |  |  +- 'Assign ='
   128     -1    132        |           |  |     +- 'IDENT': ff
   132    131     -1        |           |  |     +- 'ADD'
   131     -1    130        |           |  |        +- 'NUMBER'$0001
   130     -1     -1        |           |  |        +- 'IDENT': d
   134     -1     -1        |           |  +- 'OD'
   156    136    167        |           +- 'FOR'
   136     -1    140        |           |  +- 'IDENT': a
   140    139    144        |           |  +- 'Assign ='
   139    138     -1        |           |  |  +- 'ADD'
   138     -1    137        |           |  |     +- 'IDENT': xx
   137     -1     -1        |           |  |     +- 'IDENT': x
   144    143    146        |           |  +- 'TO'
   143    142     -1        |           |  |  +- 'SUB'
   142     -1    141        |           |  |     +- 'IDENT': c
   141     -1     -1        |           |  |     +- 'IDENT': c
   146    145    154        |           |  +- 'STEP'
   145     -1     -1        |           |  |  +- 'NUMBER'$0003
   154    148    155        |           |  +- 'DO'
   148    147     -1        |           |  |  +- 'Assign ='
   147     -1    153        |           |  |     +- 'IDENT': dd
   153    152     -1        |           |  |     +- 'DIV'
   152     -1    151        |           |  |        +- 'IDENT': e
   151    150     -1        |           |  |        +- 'MUL'
   150     -1    149        |           |  |           +- 'NUMBER'$0008
   149     -1     -1        |           |  |           +- 'IDENT': d
   155     -1     -1        |           |  +- 'OD'
   167    158    307        |           +- 'DO'
   158    157    165        |           |  +- 'Assign ='
   157     -1    161        |           |  |  +- 'IDENT': gg
   161    160     -1        |           |  |  +- 'SUB'
   160     -1    159        |           |  |     +- 'IDENT': x
   159     -1     -1        |           |  |     +- 'IDENT': x
   165    164    166        |           |  +- 'UNTILL'
   164    163     -1        |           |  |  +- 'EQU-TO'
   163     -1    162        |           |  |     +- 'IDENT': xx
   162     -1     -1        |           |  |     +- 'IDENT': e
   166     -1     -1        |           |  +- 'OD'
   307    169    308        |           +- 'ASM'
   169    168    176        |           |  +- 'PROCESSOR'
   168     -1     -1        |           |  |  +- 'R6502'
   176    175    178        |           |  +- 'SECTION'
   175    174     -1        |           |  |  +- 'SECTION Name'
   174    171     -1        |           |  |     +- 'SECTION Attributes'
   171    170    173        |           |  |        +- 'ATRB-START'
   170     -1     -1        |           |  |        |  +- 'NUMBER'$F000
   173    172     -1        |           |  |        +- 'SIZE'
   172     -1     -1        |           |  |           +- 'NUMBER'$1000
   178    177    179        |           |  +- 'ORG'
   177     -1     -1        |           |  |  +- 'NUMBER'$4000
   179     -1    181        |           |  +- 'LABEL' - 'SomeData'
   181    180    183        |           |  +- 'DB'
   180     -1     -1        |           |  |  +- 'NUMBER'$0005
   183    182    185        |           |  +- 'DW'
   182     -1     -1        |           |  |  +- 'NUMBER'$0257
   185    184    186        |           |  +- 'DL'
   184     -1     -1        |           |  |  +- 'NUMBER'$864B2
   186     -1    188        |           |  +- 'LABEL' - 'Storage'
   188    187    189        |           |  +- 'DS'
   187     -1     -1        |           |  |  +- 'NUMBER'$0A7F
   189     -1    191        |           |  +- 'LABEL' - 'START'
   191    190    193        |           |  +- 'OPCODE' ADC ($71) INDIRECT INDEXED
   190     -1     -1        |           |  |  +- 'NUMBER'$0052
   193    192    195        |           |  +- 'OPCODE' LDA ($A5) ZERO PAGE
   192     -1     -1        |           |  |  +- 'LABEL' - 'xxx'
   195    194    196        |           |  +- 'OPCODE' LDA ($A9) IMMEDIATE
   194     -1     -1        |           |  |  +- 'NUMBER'$0055
   196     -1    197        |           |  +- 'OPCODE' PHA ($48) IMPLIED
   197     -1    198        |           |  +- 'OPCODE' TYA ($98) IMPLIED
   198     -1    199        |           |  +- 'LABEL' - 'HERE'
   199     -1    201        |           |  +- 'OPCODE' PHA ($48) IMPLIED
   201    200    203        |           |  +- 'OPCODE' JMP ($4C) ABSOLUTE
   200     -1     -1        |           |  |  +- 'NUMBER'$BAD1
   203    202    205        |           |  +- 'OPCODE' JMP ($6C) INDIRECT
   202     -1     -1        |           |  |  +- 'NUMBER'$432A
   205    204    206        |           |  +- 'OPCODE' JSR ($20) ABSOLUTE
   204     -1     -1        |           |  |  +- 'NUMBER'$F00D
   206     -1    207        |           |  +- 'OPCODE' PLA ($68) IMPLIED
   207     -1    214        |           |  +- 'OPCODE' TAY ($A8) IMPLIED
   214    208    216        |           |  +- 'Asm PROC'
   208    209     -1        |           |  |  +- 'PROC name' - 'AsmProc'
   209     -1    211        |           |  |     +- 'OPCODE' PLA ($68) IMPLIED
   211    210    212        |           |  |     +- 'OPCODE' STA ($8D) ABSOLUTE
   210     -1     -1        |           |  |     |  +- 'NUMBER'$1000
   212     -1    213        |           |  |     +- 'OPCODE' RTS ($60) IMPLIED
   213     -1     -1        |           |  |     +- 'OPCODE' RTI ($40) IMPLIED
   216    215    218        |           |  +- 'OPCODE' ADC ($61) INDEXED INDIRECT
   215     -1     -1        |           |  |  +- 'NUMBER'$0044
   218    217    221        |           |  +- 'OPCODE' SBC ($E5) ZERO PAGE
   217     -1     -1        |           |  |  +- 'NUMBER'$0012
   221    219    224        |           |  +- 'OPCODE' CMP ($D5) PAGE ZERO,X
   219    220     -1        |           |  |  +- 'NUMBER'$0013
   220     -1     -1        |           |  |     +- '.X'
   224    222    227        |           |  +- 'OPCODE' AND ($3D) ABSOLUTE,X
   222    223     -1        |           |  |  +- 'NUMBER'$0400
   223     -1     -1        |           |  |     +- '.X'
   227    225    229        |           |  +- 'OPCODE' ORA ($19) ABSOLUTE,Y
   225    226     -1        |           |  |  +- 'NUMBER'$0410
   226     -1     -1        |           |  |     +- '.Y'
   229    228    231        |           |  +- 'OPCODE' CPX ($E0) IMMEDIATE
   228     -1     -1        |           |  |  +- 'NUMBER'$0017
   231    230    233        |           |  +- 'OPCODE' CPX ($E4) ZERO PAGE
   230     -1     -1        |           |  |  +- 'NUMBER'$00B0
   233    232    235        |           |  +- 'OPCODE' CPX ($EC) ABSOLUTE
   232     -1     -1        |           |  |  +- 'NUMBER'$A8EE
   235    234    238        |           |  +- 'OPCODE' ASL ($06) ZERO PAGE
   234     -1     -1        |           |  |  +- 'NUMBER'$008C
   238    236    240        |           |  +- 'OPCODE' ASL ($16) PAGE ZERO,X
   236    237     -1        |           |  |  +- 'NUMBER'$008B
   237     -1     -1        |           |  |     +- '.X'
   240    239    243        |           |  +- 'OPCODE' ASL ($0E) ABSOLUTE
   239     -1     -1        |           |  |  +- 'NUMBER'$AACC
   243    241    244        |           |  +- 'OPCODE' ASL ($1E) ABSOLUTE,X
   241    242     -1        |           |  |  +- 'NUMBER'$CCAA
   242     -1     -1        |           |  |     +- '.X'
   244     -1    246        |           |  +- 'OPCODE' ASL ($0A) ACCUMULATOR
   246    245    248        |           |  +- 'OPCODE' LDX ($A2) IMMEDIATE
   245     -1     -1        |           |  |  +- 'NUMBER'$001F
   248    247    251        |           |  +- 'OPCODE' LDX ($A6) ZERO PAGE
   247     -1     -1        |           |  |  +- 'NUMBER'$00C1
   251    249    253        |           |  +- 'OPCODE' LDX ($B6) PAGE ZERO,Y
   249    250     -1        |           |  |  +- 'NUMBER'$00C2
   250     -1     -1        |           |  |     +- '.Y'
   253    252    256        |           |  +- 'OPCODE' LDX ($AE) ABSOLUTE
   252     -1     -1        |           |  |  +- 'NUMBER'$56AB
   256    254    258        |           |  +- 'OPCODE' LDX ($BE) ABSOLUTE,Y
   254    255     -1        |           |  |  +- 'NUMBER'$64BE
   255     -1     -1        |           |  |     +- '.Y'
   258    257    260        |           |  +- 'OPCODE' LDY ($A0) IMMEDIATE
   257     -1     -1        |           |  |  +- 'NUMBER'$001F
   260    259    263        |           |  +- 'OPCODE' LDY ($A4) ZERO PAGE
   259     -1     -1        |           |  |  +- 'NUMBER'$00C1
   263    261    265        |           |  +- 'OPCODE' LDY ($B4) PAGE ZERO,X
   261    262     -1        |           |  |  +- 'NUMBER'$00C2
   262     -1     -1        |           |  |     +- '.X'
   265    264    268        |           |  +- 'OPCODE' LDY ($AC) ABSOLUTE
   264     -1     -1        |           |  |  +- 'NUMBER'$56AB
   268    266    270        |           |  +- 'OPCODE' LDY ($BC) ABSOLUTE,X
   266    267     -1        |           |  |  +- 'NUMBER'$64BE
   267     -1     -1        |           |  |     +- '.X'
   270    269    273        |           |  +- 'OPCODE' STY ($84) ZERO PAGE
   269     -1     -1        |           |  |  +- 'NUMBER'$007C
   273    271    275        |           |  +- 'OPCODE' STY ($94) PAGE ZERO,X
   271    272     -1        |           |  |  +- 'NUMBER'$007D
   272     -1     -1        |           |  |     +- '.X'
   275    274    277        |           |  +- 'OPCODE' STY ($8C) ABSOLUTE
   274     -1     -1        |           |  |  +- 'NUMBER'$D00D
   277    276    280        |           |  +- 'OPCODE' STX ($86) ZERO PAGE
   276     -1     -1        |           |  |  +- 'NUMBER'$0062
   280    278    282        |           |  +- 'OPCODE' STX ($96) PAGE ZERO,Y
   278    279     -1        |           |  |  +- 'NUMBER'$0063
   279     -1     -1        |           |  |     +- '.Y'
   282    281    284        |           |  +- 'OPCODE' STX ($8E) ABSOLUTE
   281     -1     -1        |           |  |  +- 'NUMBER'$B00B
   284    283    287        |           |  +- 'OPCODE' INC ($E6) ZERO PAGE
   283     -1     -1        |           |  |  +- 'NUMBER'$00D0
   287    285    289        |           |  +- 'OPCODE' INC ($F6) PAGE ZERO,X
   285    286     -1        |           |  |  +- 'NUMBER'$0062
   286     -1     -1        |           |  |     +- '.X'
   289    288    292        |           |  +- 'OPCODE' INC ($EE) ABSOLUTE
   288     -1     -1        |           |  |  +- 'NUMBER'$63DA
   292    290    294        |           |  +- 'OPCODE' INC ($FE) ABSOLUTE,X
   290    291     -1        |           |  |  +- 'NUMBER'$B00B
   291     -1     -1        |           |  |     +- '.X'
   294    293    297        |           |  +- 'OPCODE' DEC ($C6) ZERO PAGE
   293     -1     -1        |           |  |  +- 'NUMBER'$00D0
   297    295    299        |           |  +- 'OPCODE' DEC ($D6) PAGE ZERO,X
   295    296     -1        |           |  |  +- 'NUMBER'$0062
   296     -1     -1        |           |  |     +- '.X'
   299    298    302        |           |  +- 'OPCODE' DEC ($CE) ABSOLUTE
   298     -1     -1        |           |  |  +- 'NUMBER'$63DA
   302    300    304        |           |  +- 'OPCODE' DEC ($DE) ABSOLUTE,X
   300    301     -1        |           |  |  +- 'NUMBER'$B00B
   301     -1     -1        |           |  |     +- '.X'
   304    303    306        |           |  +- 'OPCODE' BIT ($24) ZERO PAGE
   303     -1     -1        |           |  |  +- 'NUMBER'$00A8
   306    305     -1        |           |  +- 'OPCODE' BIT ($2C) ABSOLUTE
   305     -1     -1        |           |     +- 'NUMBER'$9AB0
   308     -1     -1        |           +- 'RTI'
   314    312     -1        +- 'INT'
   312     -1    313           +- 'IDENT': qr
   313     -1     -1           +- 'IDENT': st


Feb 10, 2025

Added, finally, the binary file.  Please keep in mind this
thing is no where near complete.  But, you still might have
some fun with it.  It most likely will either crash, or hang 
if you do anything that hasn't beeb finished, which covers
pretty much the whole thing.

The only file that I know will compile is "test3.act".


Also, if you want to compile the program, you will need at
least C++ 20.



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
