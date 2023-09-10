# hw2

## Part 1.1 - Write a straightforward grammar for C-simple.

#### Your first task is to write a straightforward grammar for C-simple. First, write down tokens for C-simple, which includes a token ID, a regular expression, and any token actions you might need (e.g., for keywords). You can use your tokens from Homework 1 as a starting point. You will not need all of the tokens; you may need some new ones.

#### Second, write down the production rules for C-simple in BNF form using tokens as terminal symbols. You may define as many non-terminals as you need. Please use the convention that terminals are given in all CAPS and non-terminals are either in lower-case. Write down the production rules in the most straightforward way you can from the English description above.

#### You should use the precedence and associativity rules defined here: https://en.cppreference.com/w/cpp/language/operator_precedence

Markdown is interfearing with my formatting, look directly at the text file for best results.
```
Tokens, in the form (ID, regex, token action):  
(ID, "[a-zA-Z][0-9a-zA-Z]*",  keywordify),  
(NUM, "([0-9]*\.[0-9]+)|([0-9]+)",  idy),  
(INCR, "\+\+", idy),  
(PLUS, "\+", idy),  
(MULT, "\*", idy),  
(SUB, "-", idy),  
(DIV, "/", idy),  
(LESS, "<", idy),  
(EMI,  ";", idy),  
(LPAREN, "\(", idy),  
(RPAREN, "\)", idy),  
(LBRACE, "\{", idy),  
(RBRACE, "\}", idy),  
(EQU, "==", idy),  
(ASSIGN, "=", idy),  
(IGNORE, " |\n", idy)  
```
Token actions:  
idy simply returns the token with no changes.  
keywordify will change the token id from ID to IF, ELSE, FOR, INT, or FLOAT, if the token exactly matches "if", "else", "for", "int", or "float" respectively.  
```
1 : lstate   ::= state lstate  
2 :          |   state  
3 : state    ::= type ID SEMI                                                  // Declares a variable  
5 :          |   ID ASSIGN expr SEMI                                           // Independant assignment statement. Not to be confused with internal assignment statements  
6 :          |   IF LPAREN expr RPAREN state ELSE state                        // If statement  
7 :          |   FOR LPAREN inassign SEMI expr SEMI inassign RPAREN state      // For loop  
8 :          |   LBRACE lstate RBRACE                                          // Block 
9 : type     ::= FLOAT  
10:          |   INT  
11: inassign ::= ID ASSIGN expr                                                // Assignment statements internal to for/if statements, where being followed by a SEMI is wrong  
12: expr     ::= expr EQU comp                                                 // Equality statements  
13:          |   comp  
14: comp     ::= comp LESS sum                                                 // Comparison statements  
15:          |   sum  
16: sum      ::= sum PLUS term                                                 // Addition statements  
17:          |   sum SUB term                                                  // Subtraction statements  
18:          |   term  
19: term     ::= term DIV block                                                // Divsion statements  
20:          |   term MULT block                                               // Multiplication statements  
21:          |   block  
22: block    ::= LPAREN expr RPAREN  
23:          |   NUM  
24:          |   ID
```
## Part 1.2 - Removing left recursion.

#### Identify all points in your grammar with left recursion. Re-write any non-terminals with left recursion in a way that removes the left recursion. Be sure to check for both direct and indirect left recursion.
```
1 : lstate   ::= state lstate  
2 :          |   state  
3 : state    ::= type ID SEMI                                                  // Declares variable  
4 :          |   ID ASSIGN expr SEMI                                           // Independant assignment statement. Not to be confused with internal assignment statements  
5 :          |   IF LPAREN expr RPAREN state ELSE state                        // If statement  
6 :          |   FOR LPAREN inassign SEMI expr SEMI inassign RPAREN state      // For loop  
7 :          |   LBRACE lstate RBRACE                                          // Block  
8 : type     ::= FLOAT  
9 :          |   INT  
10: inassign ::= ID ASSIGN expr                                                // Assignment statements internal to for/if statements, where being followed by a SEMI is wrong  
11: expr     ::= comp expr2                                                    // Equality statements  
12: expr2    ::= EQU comp expr2  
13:          |   ""  
14: comp     ::= sum comp2                                                     // Comparison statements  
15: comp2    ::= LESS sum comp2  
16:          |   ""  
17: sum      ::= term sum2  
18: sum2     ::= PLUS term comp2  
19:          |   SUB term comp2  
20:          |   ""  
21: term     ::= block term2  
22: term2    ::= DIV block comp2  
23:          |   MULT block comp2  
24:          |   ""  
25: block    ::= LPAREN expr RPAREN  
26:          |   NUM  
27:          |   ID  
```
## Part 1.3 - Finding the First+ sets.

#### For each production choice, identify the First+ sets as discussed in class. Ensure that your grammar is predictive, i.e., that for each non-terminal, each production rule has a disjoint First+ set.
```
Grammer rewritten again, because rules 1 and 2 above had identical First+ sets
1 : lstate   ::= state lstate2                                                 // Used to ensure at least one statement
2 : lstate2  ::= state lstate2                                                 // We can either have multiple statements
3 :          |   ""                                                            // Or that could have been the last one
4 : state    ::= type ID SEMI                                                  // Declares variable  
5 :          |   ID ASSIGN expr SEMI                                           // Independant assignment statement. Not to be confused with internal assignment statements  
6 :          |   IF LPAREN expr RPAREN state ELSE state                        // If statement  
7 :          |   FOR LPAREN inassign SEMI expr SEMI inassign RPAREN state      // For loop  
8 :          |   LBRACE lstate RBRACE                                          // Block  
9 : type     ::= FLOAT  
10:          |   INT  
11: inassign ::= ID ASSIGN expr                                                // Assignment statements internal to for/if statements, where being followed by a SEMI is wrong  
12: expr     ::= comp expr2                                                    // Equality statements  
13: expr2    ::= EQU comp expr2  
14:          |   ""  
15: comp     ::= sum comp2                                                     // Comparison statements  
16: comp2    ::= LESS sum comp2  
17:          |   ""  
18: sum      ::= term sum2  
19: sum2     ::= PLUS term sum2  
20:          |   SUB term sum2  
21:          |   ""  
22: term     ::= block term2  
23: term2    ::= DIV block term2  
24:          |   MULT block term2  
25:          |   ""  
26: block    ::= LPAREN expr RPAREN  
27:          |   NUM  
28:          |   ID  

1 : {FLOAT, INT, ID, IF, FOR, LBRACE}
2 : {FLOAT, INT, ID, IF, FOR, LBRACE}
3 : {None, RBRACE}                                                             // If this is from the inital lstate, it should be followed by nothing. Otherwise, a } b/c end of block
4 : {FLOAT, INT}
5 : {ID}
6 : {IF}
7 : {FOR}
8 : {LBRACE}
9 : {FLOAT}
10: {INT}
11: {ID}
12: {LPAREN, NUM, ID}
13: {EQU}
14: {SEMI, RPAREN, PLUS, SUB, MULT, DIV, LESS, LPAREN}                        // expr should be followed by a SEMI if in assignment or for loop, or a RPAREN if in a for loop, if, or block. It could also be followed by a sum, comp, block, or term
15: {LPAREN, NUM, ID}
16: {LESS}
17: {SEMI, RPAREN, PLUS, SUB, MULT, DIV, EQU, LPAREN} 
18: {LPAREN, NUM, ID}
19: {PLUS}
20: {SUB}
21: {SEMI, RPAREN, MULT, DIV, LESS, EQU, LPAREN} 
22: {LPAREN, NUM, ID}
23: {DIV}
24: {MULT}
25: {SEMI, RPAREN, PLUS, SUB, LESS, EQU, LPAREN}                             // A term could be followed by a sum, comp, block, or expr
26: {LPAREN}
27: {NUM}
28: {ID}
```