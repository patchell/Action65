This is yet another attempt on my part to create an ACTION! cross compiler 
for the 6502.

As of Oct 30, 2024 I am in the process of testing the parser.
The parser in an LL(1) grammar that is implemented with a recursive
decent parseer.  Or, at least, I hope so.  I have expanded the language a little
bit, such as adding a BOOL type, adding an INTERRUPT proceedure type,
and in line assembly with syntax checking.

This is all the easy part.  The hard part is code generation.  I purchased
a new book, that I hope, will give me new insights on how to go about
doing this sort of thing.
