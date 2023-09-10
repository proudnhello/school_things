from enum import Enum
from typing import Callable,List,Tuple,Optional
import argparse
import re

class ScannerException(Exception):
    
    # this time, the scanner exception takes a line number
    def __init__(self, lineno:int) -> None:
        message = "Scanner error on line: " + str(lineno)
        super().__init__(message)

class Token(Enum):
    MUL     ="MUL"
    PLUS    ="PLUS"
    MINUS   ="MINUS"
    DIV     ="DIV"
    EQ      ="EQU"
    LT      ="LT"
    LBRACE  ="LBRACE"
    RBRACE  ="RBRACE"
    LPAR    ="LPAR"
    RPAR    ="RPAR"
    SEMI    ="SEMI"
    ID      ="ID"
    NUM     ="NUM"
    IF      ="IF"
    ELSE    ="ELSE"
    FOR     ="FOR"
    INT     ="INT"
    FLOAT   ="FLOAT"
    IGNORE  ="IGNORE"
    ASSIGN  ="ASSIGN"

class Lexeme:
    def __init__(self, token:Token, value:str) -> None:
        self.token = token
        self.value = value

    def __str__(self) -> str:
        return "(" + str(self.token) + "," + "\"" + self.value + "\"" + ")"    


class Scanner:
    def __init__(self, tokens: List[Tuple[Token,str,Callable[[Lexeme],Lexeme]]]) -> None:
        self.tokens = tokens
        self.lineno = 1

    def input_string(self, input_string:str) -> None:
        self.istring = input_string

    # Get the scanner line number, needed for the parser exception
    def get_lineno(self)->int:
        return self.lineno

    # SOSscanner taken from hw1
    def token(self) -> Optional[Lexeme]:
        while True:
            if len(self.istring) == 0:
                return None

            # For the input
            matches = []

                # Check each token
            for t in tokens:
                # Create a tuple for each token:
                # * first element is the token type
                # * second is the possible match
                # * third is the token action
                matches.append((t[0],re.match(t[1],self.istring), t[2]))

                # Remove all non matches from the list of matches
                matches = [m for m in matches if m[1] is not None]
            
            # If there were no matches, raise an exception
            if len(matches) == 0:
                raise ScannerException(self.lineno)
            
            # Find the longest string matched by one of the tokens           
            longest = matches[0]
            for m in matches:
                if len(m[1][0]) > len(longest[1][0]):
                    longest = m

            # apply the token action
            ret = longest[2](Lexeme(longest[0],longest[1][0]), self)

            # figure how much we need to chop from our input string

            chop = len(ret.value)
            self.istring = self.istring[chop:]

            # if we did not match an IGNORE token, then we can
            # return the lexeme
            if ret.token != Token.IGNORE:
                return ret

# Do nothing
def idy(l:Lexeme, s:Scanner) -> Lexeme:
    return l

# If we discovered an ID, then check to see if it is a keyword. If it is, change the token type to that keyword
def keywordify(l:Lexeme, s:Scanner) -> Lexeme:
    if l.value == "if":
         l.token = Token.IF
    elif l.value == "else":
         l.token = Token.ELSE
    elif l.value == "for":
         l.token = Token.FOR
    elif l.value == "int":
         l.token = Token.INT
    elif l.value == "float":
         l.token = Token.FLOAT
    return l

# If we find a newline, increment the line counter. B/c the line counter is in the scanner, this and all other token actions
# need to have an instance of the scanner passed to them
def lineCounter(l:Lexeme, s:Scanner) -> Lexeme:
    if l.value == "\n":
        s.lineno += 1
    return l

# Tokens for C simple
tokens = [(Token.ID,     "[a-zA-Z][0-9a-zA-Z]*",  keywordify),
    (Token.NUM,    "([0-9]*\.[0-9]+)|([0-9]+)",  idy),
    (Token.PLUS,  "\+", idy),
    (Token.MINUS, "-", idy),
    (Token.LT, "<", idy),
    (Token.MUL,  "\*", idy),
    (Token.DIV, "/", idy),
    (Token.SEMI,  ";", idy),
    (Token.LPAR,  "\(", idy),
    (Token.RPAR,  "\)", idy),
    (Token.LBRACE,  "{", idy),
    (Token.RBRACE,  "}", idy),
    (Token.EQ,  "==", idy),
    (Token.ASSIGN,  "=", idy),
    (Token.IGNORE, " |\n|\t",    lineCounter)]

# Main from hw1 used for testing

# if __name__ == "__main__":

#     parser = argparse.ArgumentParser()
#     parser.add_argument('file_name', type=str)
#     parser.add_argument('--verbose', '-v', action='store_true')
#     args = parser.parse_args()
    
#     f = open(args.file_name)    
#     f_contents = f.read()
#     f.close()

#     verbose = args.verbose

#     s = Scanner(tokens)
#     s.input_string(f_contents)

#     while True:
#         t = s.token()
#         if t is None:
#             break
#         if (verbose):
#             print(t)