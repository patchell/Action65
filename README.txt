This is yet another attempt on my part to create an ACTION! cross compiler  
for the 6502.

Compatibility Issues with the real ACTION!
1. Action! used the '*' character to mean "Current Location".
	Unfortunately, this has a conflict with the meaning
	of Multiply, so I had to make the current location
	Symbol be '.*'  :(
	Hopefully, in the future I will find a way to fix
	this conflict.

March 6, 2025
Well, it seems I ran into a bit of a problem.
When I was testing the declarations of TYPE, I
started getting random crashes, i.e. , sometimes
a crash, sometimes not.  about 1 crash for every
4 runs or so.  Not a good sign.  I was lucky in that
when it did creash, it crashed in the same two places,
and the two together pointed to the fact that I was
probably doing a "delete" two time on the same object.
However, I was unable to come up with a method of
actually finding where it was happening and how.
In my efforts to find this problem I came to the sad
realization that what I thought was a real cool way to
implement a Recursive Desent Parcser turned out to be
a bad idea instead.  So the last couple of weeks I have
been doing heavy modifications to the parser.  Fixed
the crash, I hope, and I am starting over verifying the
parser and AST generation.

Feb 18, 2025
Fixing all sorts of things that were typed in just
completely wrong, or just not thought through.
Getting there.


	 NUMB  CHILD  NEXT
   388      4     -1  +- 'ROOT'
     4      1      7     +- 'INT'
     1     -1      2     |  +- 'IDENT': aa
     2     -1      3     |  +- 'IDENT': bb
     3     -1     -1     |  +- 'IDENT': cc
     7      5      9     +- 'CARD'
     5     -1      6     |  +- 'IDENT': dd
     6     -1     -1     |  +- 'IDENT': ee
     9      8     15     +- 'CHAR'
     8     -1     -1     |  +- 'IDENT': ff
    15     10    387     +- 'BYTE'
    10     -1     11     |  +- 'IDENT': gg
    11     -1     12     |  +- 'IDENT': hh
    12     -1     13     |  +- 'IDENT': ii
    13     -1     14     |  +- 'IDENT': jj
    14     -1     -1     |  +- 'IDENT': kk
   387     23     -1     +- 'MODULE'
    23     22    383        +- 'DEFINE'
    22     21     -1        |  +- 'SECTION'
    21     20     -1        |     +- 'SECTION Name'
    20     17     -1        |        +- 'SECTION Attributes'
    17     16     19        |           +- 'ATRB-START'
    16     -1     -1        |           |  +- 'NUMBER'$0200
    19     18     -1        |           +- 'SIZE'
    18     -1     -1        |              +- 'NUMBER'$0400
   383     24    386        +- 'IRQ-PROC'
    24     25     -1        |  +- 'IDENT': AnInteruption
    25     -1    382        |     +- 'Param LIST'
   382     38     -1        |     +- 'Body'
    38     27     39        |        +- 'LOCAL Vars'
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
    39     45     -1        |        +- 'STATEMENTS'
    45     40     55        |           +- 'CODE-BLOCK'
    40     -1     41        |           |  +- 'NUMBER'$00AA
    41     -1     42        |           |  +- 'NUMBER'$00BB
    42     -1     43        |           |  +- 'NUMBER'$00CE
    43     -1     44        |           |  +- 'NUMBER'$00A1
    44     -1     -1        |           |  +- 'NUMBER'$00C0
    55     48     63        |           +- 'IFF'
    48     47     53        |           |  +- 'EQU-TO'
    47     -1     46        |           |  |  +- 'NUMBER'$0000
    46     -1     -1        |           |  |  +- '.A'
    53     49     54        |           |  +- 'THEN'
    49     51     -1        |           |  |  +- 'STATEMENTS'
    51     52     -1        |           |  |     +- '=SUB'
    52     -1     50        |           |  |        +- 'NUMBER'$0002
    50     -1     -1        |           |  |        +- 'IDENT': jj
    54     -1     -1        |           |  +- 'FFI'
    63     56     65        |           +- 'IFF'
    56     -1     61        |           |  +- '.N'
    61     57     62        |           |  +- 'THEN'
    57     59     -1        |           |  |  +- 'STATEMENTS'
    59     60     -1        |           |  |     +- '=MOD'
    60     -1     58        |           |  |        +- 'NUMBER'$0005
    58     -1     -1        |           |  |        +- 'IDENT': kk
    62     -1     -1        |           |  +- 'FFI'
    65     68     70        |           +- 'Assign ='
    68     67     64        |           |  +- 'MUL'
    67     -1     66        |           |  |  +- 'IDENT': d
    66     -1     -1        |           |  |  +- 'IDENT': c
    64     -1     -1        |           |  +- 'IDENT': a
    70     74     76        |           +- 'Assign ='
    74     73     69        |           |  +- 'ADD'
    73     72     -1        |           |  |  +- 'MUL'
    72     -1     71        |           |  |     +- 'IDENT': d
    71     -1     -1        |           |  |     +- 'IDENT': c
    69     -1     -1        |           |  +- 'IDENT': a
    76     79     81        |           +- '=SUB'
    79     78     75        |           |  +- 'MUL'
    78     -1     77        |           |  |  +- 'IDENT': d
    77     -1     -1        |           |  |  +- 'IDENT': c
    75     -1     -1        |           |  +- 'IDENT': a
    81     84     86        |           +- '=SUB'
    84     83     80        |           |  +- 'MUL'
    83     -1     82        |           |  |  +- 'IDENT': d
    82     -1     -1        |           |  |  +- 'IDENT': c
    80     -1     -1        |           |  +- 'IDENT': a
    86     89     91        |           +- '=DIV'
    89     88     85        |           |  +- 'MUL'
    88     -1     87        |           |  |  +- 'IDENT': d
    87     -1     -1        |           |  |  +- 'IDENT': c
    85     -1     -1        |           |  +- 'IDENT': a
    91     94     96        |           +- '=MOD'
    94     93     90        |           |  +- 'MUL'
    93     -1     92        |           |  |  +- 'IDENT': d
    92     -1     -1        |           |  |  +- 'IDENT': c
    90     -1     -1        |           |  +- 'IDENT': a
    96     99    101        |           +- '=XOR'
    99     98     95        |           |  +- 'MUL'
    98     -1     97        |           |  |  +- 'IDENT': d
    97     -1     -1        |           |  |  +- 'IDENT': c
    95     -1     -1        |           |  +- 'IDENT': a
   101    104    106        |           +- '=AND'
   104    103    100        |           |  +- 'MUL'
   103     -1    102        |           |  |  +- 'IDENT': d
   102     -1     -1        |           |  |  +- 'IDENT': c
   100     -1     -1        |           |  +- 'IDENT': a
   106    109    111        |           +- '=OR'
   109    108    105        |           |  +- 'MUL'
   108     -1    107        |           |  |  +- 'IDENT': d
   107     -1     -1        |           |  |  +- 'IDENT': c
   105     -1     -1        |           |  +- 'IDENT': a
   111    114    116        |           +- '=LSH'
   114    113    110        |           |  +- 'MUL'
   113     -1    112        |           |  |  +- 'IDENT': d
   112     -1     -1        |           |  |  +- 'IDENT': c
   110     -1     -1        |           |  +- 'IDENT': a
   116    119    121        |           +- '=RSH'
   119    118    115        |           |  +- 'MUL'
   118     -1    117        |           |  |  +- 'IDENT': d
   117     -1     -1        |           |  |  +- 'IDENT': c
   115     -1     -1        |           |  +- 'IDENT': a
   121    124    133        |           +- '=ADD'
   124    123    120        |           |  +- 'MUL'
   123     -1    122        |           |  |  +- 'IDENT': d
   122     -1     -1        |           |  |  +- 'IDENT': c
   120     -1     -1        |           |  +- 'IDENT': a
   133    126    153        |           +- 'IFF'
   126    125    131        |           |  +- 'PL/65 BITS'
   125     -1     -1        |           |  |  +- 'NUMBER'$0005
   131    127    132        |           |  +- 'THEN'
   127    129     -1        |           |  |  +- 'STATEMENTS'
   129    130     -1        |           |  |     +- '=LSH'
   130     -1    128        |           |  |        +- 'NUMBER'$0002
   128     -1     -1        |           |  |        +- 'IDENT': ii
   132     -1     -1        |           |  +- 'FFI'
   153    134    169        |           +- 'DO'
   134    145    152        |           |  +- 'STATEMENTS'
   145    137    149        |           |  |  +- 'IF'
   137    136    143        |           |  |  |  +- 'EQU-TO'
   136     -1    135        |           |  |  |  |  +- 'IDENT': xx
   135     -1     -1        |           |  |  |  |  +- 'IDENT': c
   143    138    144        |           |  |  |  +- 'THEN'
   138    140     -1        |           |  |  |  |  +- 'STATEMENTS'
   140    139    142        |           |  |  |  |     +- 'PUSH'
   139     -1     -1        |           |  |  |  |     |  +- '.A'
   142    141     -1        |           |  |  |  |     +- 'POP'
   141     -1     -1        |           |  |  |  |        +- '.Y'
   144     -1     -1        |           |  |  |  +- 'FI'
   149    148    151        |           |  |  +- 'UNTILL'
   148    147     -1        |           |  |  |  +- 'NEQ'
   147     -1    146        |           |  |  |     +- 'IDENT': gg
   146     -1     -1        |           |  |  |     +- 'IDENT': jj
   151    150     -1        |           |  |  +- 'PUSH'
   150     -1     -1        |           |  |     +- '.A'
   152     -1     -1        |           |  +- 'OD'
   169    156    175        |           +- 'IF'
   156    155    160        |           |  +- '>'
   155     -1    154        |           |  |  +- 'IDENT': gg
   154     -1     -1        |           |  |  +- 'IDENT': xxx
   160    157    167        |           |  +- 'THEN'
   157    158     -1        |           |  |  +- 'STATEMENTS'
   158     -1    159        |           |  |     +- 'BREAK'
   159     -1     -1        |           |  |     +- 'EXIT'
   167    161    168        |           |  +- 'ELSE'
   161    163     -1        |           |  |  +- 'STATEMENTS'
   163    164    166        |           |  |     +- '=ADD'
   164     -1    162        |           |  |     |  +- 'IDENT': e
   162     -1     -1        |           |  |     |  +- 'IDENT': gg
   166    165     -1        |           |  |     +- 'PUSH'
   165     -1     -1        |           |  |        +- '.X'
   168     -1     -1        |           |  +- 'FI'
   175    170    185        |           +- 'CODE-BLOCK'
   170     -1    171        |           |  +- 'NUMBER'$00AA
   171     -1    172        |           |  +- 'NUMBER'$00BB
   172     -1    173        |           |  +- 'NUMBER'$00CE
   173     -1    174        |           |  +- 'NUMBER'$00A1
   174     -1     -1        |           |  +- 'NUMBER'$00C0
   185    176    187        |           +- 'DO'
   176    178    184        |           |  +- 'STATEMENTS'
   178    183     -1        |           |  |  +- 'Assign ='
   183    182    177        |           |  |     +- 'SUB'
   182    181    179        |           |  |     |  +- 'MUL'
   181     -1    180        |           |  |     |  |  +- 'IDENT': xx
   180     -1     -1        |           |  |     |  |  +- 'IDENT': x
   179     -1     -1        |           |  |     |  +- 'IDENT': e
   177     -1     -1        |           |  |     +- 'IDENT': c
   184     -1     -1        |           |  +- 'OD'
   187    194    206        |           +- 'Assign ='
   194    193    186        |           |  +- 'SUB'
   193     -1    192        |           |  |  +- 'IDENT': xx
   192    191     -1        |           |  |  +- 'ADD'
   191    190    188        |           |  |     +- 'MUL'
   190     -1    189        |           |  |     |  +- 'IDENT': a
   189     -1     -1        |           |  |     |  +- 'IDENT': xxx
   188     -1     -1        |           |  |     +- 'IDENT': x
   186     -1     -1        |           |  +- 'IDENT': d
   206    197    228        |           +- 'WHILE'
   197    196    204        |           |  +- '>='
   196     -1    195        |           |  |  +- 'IDENT': d
   195     -1     -1        |           |  |  +- 'IDENT': c
   204    198    205        |           |  +- 'DO'
   198    200     -1        |           |  |  +- 'STATEMENTS'
   200    203     -1        |           |  |     +- 'Assign ='
   203    202    199        |           |  |        +- 'ADD'
   202     -1    201        |           |  |        |  +- 'NUMBER'$0001
   201     -1     -1        |           |  |        |  +- 'IDENT': d
   199     -1     -1        |           |  |        +- 'IDENT': ff
   205     -1     -1        |           |  +- 'OD'
   228    207    240        |           +- 'FOR'
   207     -1    211        |           |  +- 'IDENT': a
   211    210    215        |           |  +- 'ASSIGN'
   210    209     -1        |           |  |  +- 'ADD'
   209     -1    208        |           |  |     +- 'IDENT': xx
   208     -1     -1        |           |  |     +- 'IDENT': x
   215    214    217        |           |  +- 'TO'
   214    213     -1        |           |  |  +- 'SUB'
   213     -1    212        |           |  |     +- 'IDENT': c
   212     -1     -1        |           |  |     +- 'IDENT': c
   217    216    226        |           |  +- 'STEP'
   216     -1     -1        |           |  |  +- 'NUMBER'$0003
   226    218    227        |           |  +- 'DO'
   218    220     -1        |           |  |  +- 'STATEMENTS'
   220    225     -1        |           |  |     +- 'Assign ='
   225    224    219        |           |  |        +- 'DIV'
   224     -1    223        |           |  |        |  +- 'IDENT': e
   223    222     -1        |           |  |        |  +- 'MUL'
   222     -1    221        |           |  |        |     +- 'NUMBER'$0008
   221     -1     -1        |           |  |        |     +- 'IDENT': d
   219     -1     -1        |           |  |        +- 'IDENT': dd
   227     -1     -1        |           |  +- 'OD'
   240    229    380        |           +- 'DO'
   229    231    239        |           |  +- 'STATEMENTS'
   231    234    238        |           |  |  +- 'Assign ='
   234    233    230        |           |  |  |  +- 'SUB'
   233     -1    232        |           |  |  |  |  +- 'IDENT': x
   232     -1     -1        |           |  |  |  |  +- 'IDENT': x
   230     -1     -1        |           |  |  |  +- 'IDENT': gg
   238    237     -1        |           |  |  +- 'UNTILL'
   237    236     -1        |           |  |     +- 'EQU-TO'
   236     -1    235        |           |  |        +- 'IDENT': xx
   235     -1     -1        |           |  |        +- 'IDENT': e
   239     -1     -1        |           |  +- 'OD'
   380    242    381        |           +- 'ASM'
   242    241    249        |           |  +- 'PROCESSOR'
   241     -1     -1        |           |  |  +- 'R6502'
   249    248    251        |           |  +- 'SECTION'
   248    247     -1        |           |  |  +- 'SECTION Name'
   247    244     -1        |           |  |     +- 'SECTION Attributes'
   244    243    246        |           |  |        +- 'ATRB-START'
   243     -1     -1        |           |  |        |  +- 'NUMBER'$F000
   246    245     -1        |           |  |        +- 'SIZE'
   245     -1     -1        |           |  |           +- 'NUMBER'$1000
   251    250    252        |           |  +- 'ORG'
   250     -1     -1        |           |  |  +- 'NUMBER'$4000
   252     -1    254        |           |  +- 'LABEL' - 'SomeData'
   254    253    256        |           |  +- 'DB'
   253     -1     -1        |           |  |  +- 'NUMBER'$0005
   256    255    258        |           |  +- 'DW'
   255     -1     -1        |           |  |  +- 'NUMBER'$0257
   258    257    259        |           |  +- 'DL'
   257     -1     -1        |           |  |  +- 'NUMBER'$864B2
   259     -1    261        |           |  +- 'LABEL' - 'Storage'
   261    260    262        |           |  +- 'DS'
   260     -1     -1        |           |  |  +- 'NUMBER'$0A7F
   262     -1    264        |           |  +- 'LABEL' - 'START'
   264    263    266        |           |  +- 'OPCODE' ADC ($71) INDIRECT INDEXED
   263     -1     -1        |           |  |  +- 'NUMBER'$0052
   266    265    268        |           |  +- 'OPCODE' LDA ($A5) ZERO PAGE
   265     -1     -1        |           |  |  +- 'LABEL' - 'xxx'
   268    267    269        |           |  +- 'OPCODE' LDA ($A9) IMMEDIATE
   267     -1     -1        |           |  |  +- 'NUMBER'$0055
   269     -1    270        |           |  +- 'OPCODE' PHA ($48) IMPLIED
   270     -1    271        |           |  +- 'OPCODE' TYA ($98) IMPLIED
   271     -1    272        |           |  +- 'LABEL' - 'HERE'
   272     -1    274        |           |  +- 'OPCODE' PHA ($48) IMPLIED
   274    273    276        |           |  +- 'OPCODE' JMP ($4C) ABSOLUTE
   273     -1     -1        |           |  |  +- 'NUMBER'$BAD1
   276    275    278        |           |  +- 'OPCODE' JMP ($6C) INDIRECT
   275     -1     -1        |           |  |  +- 'NUMBER'$432A
   278    277    279        |           |  +- 'OPCODE' JSR ($20) ABSOLUTE
   277     -1     -1        |           |  |  +- 'NUMBER'$F00D
   279     -1    280        |           |  +- 'OPCODE' PLA ($68) IMPLIED
   280     -1    287        |           |  +- 'OPCODE' TAY ($A8) IMPLIED
   287    281    289        |           |  +- 'Asm PROC'
   281    282     -1        |           |  |  +- 'PROC name' - 'AsmProc'
   282     -1    284        |           |  |     +- 'OPCODE' PLA ($68) IMPLIED
   284    283    285        |           |  |     +- 'OPCODE' STA ($8D) ABSOLUTE
   283     -1     -1        |           |  |     |  +- 'NUMBER'$1000
   285     -1    286        |           |  |     +- 'OPCODE' RTS ($60) IMPLIED
   286     -1     -1        |           |  |     +- 'OPCODE' RTI ($40) IMPLIED
   289    288    291        |           |  +- 'OPCODE' ADC ($61) INDEXED INDIRECT
   288     -1     -1        |           |  |  +- 'NUMBER'$0044
   291    290    294        |           |  +- 'OPCODE' SBC ($E5) ZERO PAGE
   290     -1     -1        |           |  |  +- 'NUMBER'$0012
   294    292    297        |           |  +- 'OPCODE' CMP ($D5) PAGE ZERO,X
   292    293     -1        |           |  |  +- 'NUMBER'$0013
   293     -1     -1        |           |  |     +- '.X'
   297    295    300        |           |  +- 'OPCODE' AND ($3D) ABSOLUTE,X
   295    296     -1        |           |  |  +- 'NUMBER'$0400
   296     -1     -1        |           |  |     +- '.X'
   300    298    302        |           |  +- 'OPCODE' ORA ($19) ABSOLUTE,Y
   298    299     -1        |           |  |  +- 'NUMBER'$0410
   299     -1     -1        |           |  |     +- '.Y'
   302    301    304        |           |  +- 'OPCODE' CPX ($E0) IMMEDIATE
   301     -1     -1        |           |  |  +- 'NUMBER'$0017
   304    303    306        |           |  +- 'OPCODE' CPX ($E4) ZERO PAGE
   303     -1     -1        |           |  |  +- 'NUMBER'$00B0
   306    305    308        |           |  +- 'OPCODE' CPX ($EC) ABSOLUTE
   305     -1     -1        |           |  |  +- 'NUMBER'$A8EE
   308    307    311        |           |  +- 'OPCODE' ASL ($06) ZERO PAGE
   307     -1     -1        |           |  |  +- 'NUMBER'$008C
   311    309    313        |           |  +- 'OPCODE' ASL ($16) PAGE ZERO,X
   309    310     -1        |           |  |  +- 'NUMBER'$008B
   310     -1     -1        |           |  |     +- '.X'
   313    312    316        |           |  +- 'OPCODE' ASL ($0E) ABSOLUTE
   312     -1     -1        |           |  |  +- 'NUMBER'$AACC
   316    314    317        |           |  +- 'OPCODE' ASL ($1E) ABSOLUTE,X
   314    315     -1        |           |  |  +- 'NUMBER'$CCAA
   315     -1     -1        |           |  |     +- '.X'
   317     -1    319        |           |  +- 'OPCODE' ASL ($0A) ACCUMULATOR
   319    318    321        |           |  +- 'OPCODE' LDX ($A2) IMMEDIATE
   318     -1     -1        |           |  |  +- 'NUMBER'$001F
   321    320    324        |           |  +- 'OPCODE' LDX ($A6) ZERO PAGE
   320     -1     -1        |           |  |  +- 'NUMBER'$00C1
   324    322    326        |           |  +- 'OPCODE' LDX ($B6) PAGE ZERO,Y
   322    323     -1        |           |  |  +- 'NUMBER'$00C2
   323     -1     -1        |           |  |     +- '.Y'
   326    325    329        |           |  +- 'OPCODE' LDX ($AE) ABSOLUTE
   325     -1     -1        |           |  |  +- 'NUMBER'$56AB
   329    327    331        |           |  +- 'OPCODE' LDX ($BE) ABSOLUTE,Y
   327    328     -1        |           |  |  +- 'NUMBER'$64BE
   328     -1     -1        |           |  |     +- '.Y'
   331    330    333        |           |  +- 'OPCODE' LDY ($A0) IMMEDIATE
   330     -1     -1        |           |  |  +- 'NUMBER'$001F
   333    332    336        |           |  +- 'OPCODE' LDY ($A4) ZERO PAGE
   332     -1     -1        |           |  |  +- 'NUMBER'$00C1
   336    334    338        |           |  +- 'OPCODE' LDY ($B4) PAGE ZERO,X
   334    335     -1        |           |  |  +- 'NUMBER'$00C2
   335     -1     -1        |           |  |     +- '.X'
   338    337    341        |           |  +- 'OPCODE' LDY ($AC) ABSOLUTE
   337     -1     -1        |           |  |  +- 'NUMBER'$56AB
   341    339    343        |           |  +- 'OPCODE' LDY ($BC) ABSOLUTE,X
   339    340     -1        |           |  |  +- 'NUMBER'$64BE
   340     -1     -1        |           |  |     +- '.X'
   343    342    346        |           |  +- 'OPCODE' STY ($84) ZERO PAGE
   342     -1     -1        |           |  |  +- 'NUMBER'$007C
   346    344    348        |           |  +- 'OPCODE' STY ($94) PAGE ZERO,X
   344    345     -1        |           |  |  +- 'NUMBER'$007D
   345     -1     -1        |           |  |     +- '.X'
   348    347    350        |           |  +- 'OPCODE' STY ($8C) ABSOLUTE
   347     -1     -1        |           |  |  +- 'NUMBER'$D00D
   350    349    353        |           |  +- 'OPCODE' STX ($86) ZERO PAGE
   349     -1     -1        |           |  |  +- 'NUMBER'$0062
   353    351    355        |           |  +- 'OPCODE' STX ($96) PAGE ZERO,Y
   351    352     -1        |           |  |  +- 'NUMBER'$0063
   352     -1     -1        |           |  |     +- '.Y'
   355    354    357        |           |  +- 'OPCODE' STX ($8E) ABSOLUTE
   354     -1     -1        |           |  |  +- 'NUMBER'$B00B
   357    356    360        |           |  +- 'OPCODE' INC ($E6) ZERO PAGE
   356     -1     -1        |           |  |  +- 'NUMBER'$00D0
   360    358    362        |           |  +- 'OPCODE' INC ($F6) PAGE ZERO,X
   358    359     -1        |           |  |  +- 'NUMBER'$0062
   359     -1     -1        |           |  |     +- '.X'
   362    361    365        |           |  +- 'OPCODE' INC ($EE) ABSOLUTE
   361     -1     -1        |           |  |  +- 'NUMBER'$63DA
   365    363    367        |           |  +- 'OPCODE' INC ($FE) ABSOLUTE,X
   363    364     -1        |           |  |  +- 'NUMBER'$B00B
   364     -1     -1        |           |  |     +- '.X'
   367    366    370        |           |  +- 'OPCODE' DEC ($C6) ZERO PAGE
   366     -1     -1        |           |  |  +- 'NUMBER'$00D0
   370    368    372        |           |  +- 'OPCODE' DEC ($D6) PAGE ZERO,X
   368    369     -1        |           |  |  +- 'NUMBER'$0062
   369     -1     -1        |           |  |     +- '.X'
   372    371    375        |           |  +- 'OPCODE' DEC ($CE) ABSOLUTE
   371     -1     -1        |           |  |  +- 'NUMBER'$63DA
   375    373    377        |           |  +- 'OPCODE' DEC ($DE) ABSOLUTE,X
   373    374     -1        |           |  |  +- 'NUMBER'$B00B
   374     -1     -1        |           |  |     +- '.X'
   377    376    379        |           |  +- 'OPCODE' BIT ($24) ZERO PAGE
   376     -1     -1        |           |  |  +- 'NUMBER'$00A8
   379    378     -1        |           |  +- 'OPCODE' BIT ($2C) ABSOLUTE
   378     -1     -1        |           |     +- 'NUMBER'$9AB0
   381     -1     -1        |           +- 'RTI'
   386    384     -1        +- 'INT'
   384     -1    385           +- 'IDENT': qr
   385     -1     -1           +- 'IDENT': st
Lines Compiled:147

Feb 15, 2025
To a large extent, but still not complete, all of the major
language features are now working...I think.  So I will
continue to write ACTION screpts to cover the rest.
Is this fun or what?  ;)

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
