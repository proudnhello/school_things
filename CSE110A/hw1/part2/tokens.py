from enum import Enum

class Token(Enum):
    ID     = "ID"
    PLUS   = "PLUS"
    HNUM   = "HNUM"
    NUM    = "NUM"
    IGNORE = "IGNORE"
    INCR   = "INCR"
    MULT   = "MULT"
    SEMI   = "SEMI"
    LPAREN = "LPAREN"
    RPAREN = "RPAREN"
    LBRACE = "LBRACE"
    RBRACE = "RBRACE"
    ASSIGN = "ASSIGN"
    IF     = "IF"
    ELSE   = "ELSE"
    WHILE  = "WHILE"
    INT    = "INT"
    FLOAT  = "FLOAT"

class Lexeme:
    def __init__(self, token:Token, value:str) -> None:
        self.token = token
        self.value = value

    def __str__(self):
        return "(" + str(self.token) + "," + "\"" + self.value + "\"" + ")"    

def idy(l:Lexeme) -> Lexeme:
    return l

def keywordify(l:Lexeme) -> Lexeme:
    if l.value == "if":
         l.token = Token.IF
    elif l.value == "else":
         l.token = Token.ELSE
    elif l.value == "while":
         l.token = Token.WHILE
    elif l.value == "int":
         l.token = Token.INT
    elif l.value == "float":
         l.token = Token.FLOAT
    return l


tokens = [
    (Token.ID,     "[a-zA-Z][0-9a-zA-Z]*",  keywordify),
    (Token.HNUM,  "((0x)|(0X))[0-9a-fA-F]+", idy),
    (Token.NUM,    "([0-9]*\.[0-9]+)|([0-9]+)",  idy),
    (Token.INCR,  "\+\+", idy),
    (Token.PLUS,  "\+", idy),
    (Token.MULT,  "\*", idy),
    (Token.SEMI,  ";", idy),
    (Token.LPAREN,  "\(", idy),
    (Token.RPAREN,  "\)", idy),
    (Token.LBRACE,  "{", idy),
    (Token.RBRACE,  "}", idy),
    (Token.ASSIGN,  "=", idy),
    (Token.IGNORE, " |\n",    idy)
]
