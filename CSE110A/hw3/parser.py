from scanner import Lexeme,Token,Scanner
from typing import Callable,List,Tuple,Optional

# Symbol Table exception, requires a line number and ID
class SymbolTableException(Exception):
    
    def __init__(self, lineno:int, ID:str) -> None:
        message = "Symbol table error on line: " + str(lineno) + "\nUndeclared ID: " + ID
        super().__init__(message)    

#Implement all members of this class for Part 3
class SymbolTable:
    # For the symbol table, scope dict will be our array of dictionaries
    def __init__(self) -> None:
        self.scope_dict = []
        # The way scope dict will work is we prepend new dicts to it as push
        # When we pop, we will remove the 0th entry in the array
        # This will allow us to still iterate over scope dict to find variables in lower scopes, while still having it act as stack
        # b/c newest dict is first, this will let use use standard for x in y for loop
        self.scope_dict.insert(0, dict())

    # Add ID, info to the hash table top of the stack
    def insert(self, ID:str, info) -> None:
        self.scope_dict[0][ID] = info

    # check every dictionary starting at the top for the ID. Return info if it is found, None if it is not
    def lookup(self, ID:str):
        # Iterate over all the dicts, starting at the top of the stack
        for d in self.scope_dict:
            # This should return None if ID is not found
            if d.get(ID, None) != None:
                return d[ID]
        return None

    # make a new dict at the start of scope_dict
    def push_scope(self) -> None:
        self.scope_dict.insert(0, dict())

    # remove the top of the stack
    def pop_scope(self) -> None:
        self.scope_dict.pop(0)

class ParserException(Exception):
    
    # Pass a line number, current lexeme, and what tokens are expected, and print corresponding error message
    def __init__(self, lineno:int, lexeme:Lexeme, tokens:List[Token]) -> None:
        message = "Parser error on line: " + str(lineno) + "\nExpected one of: " + str(tokens) + "\nGot: " + str(lexeme)
        super().__init__(message)

class Parser:
    # Create a new parser, alongside its scannar and symbol table
    def __init__(self, scanner:Scanner, use_symbol_table:bool) -> None:
        self.use_symbol_table = use_symbol_table
        self.table = SymbolTable()
        self.scanner = scanner

    # Pass in the expected token ID to eat
    def eat(self, tokenID):
        # If it does not match, raise an exception 
        if self.getID(self.to_match) != tokenID:
            raise ParserException(self.scanner.lineno, self.to_match, [tokenID])
        # Otherwise, get the next lememe
        self.to_match = self.scanner.token()
    # This function will result on an error being thrown once it encounters the first wrong token
    # So if we had the code "if \n potato" the error would give the line number for the id potato

    # Helper function that converts token IDs into strings
    def getID(self, l:Lexeme):
        if l == None:
            return None
        # Convert the token ID to a str, then remove the Token.
        chop = len("Token.")
        output = str(l.token)[chop:]
        return output


    # Implement one function in this class for every non-terminal in
    # your grammar using the recursive descent recipe from the book
    # and the lectures for part 2
    def parse_init(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # init := statment init {INT, FLOAT, ID, IF, LBRACE, FOR}  
        if id in ["INT", "FLOAT", "ID", "IF", "LBRACE", "FOR"]:
            self.parse_statement()
            self.parse_init()
            return
        
        # | None {None}
        if id in [None]:
            return
        
        raise ParserException(self.scanner.lineno, self.to_match, ["INT", "FLOAT", "ID", "IF", "LBRACE", "FOR", None])

    def parse_statement_list(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # statement_list := statement statement_list {INT, FLOAT, ID, IF, LBRACE, FOR}  
        if id in ["INT", "FLOAT", "ID", "IF", "LBRACE", "FOR"]:
            self.parse_statement()
            self.parse_statement_list()
            return
        
        # | None {None}
        if id in [None]:
            return
        
        # |  "" {RBRACE}
        if id in ["RBRACE"]:
            return
        
        raise ParserException(self.scanner.lineno, self.to_match, ["INT", "FLOAT", "ID", "IF", "LBRACE", "FOR", "RBRACE", None])

    # Parse a statement
    def parse_statement(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # statement := declaration_statement  {INT, FLOAT}
        if id in ["INT", "FLOAT"]:
            self.parse_declaration_statement()
            return
        #   |  assignment_statement   {ID}
        if id in ["ID"]:
            self.parse_assignment_statement()
            return
        #   |  if_else_statement      {IF}
        if id in ["IF"]:
            self.parse_if_else_statement()
            return
        #   |  block_statement        {LBRACE}
        if id in ["LBRACE"]:
            self.parse_block_statement()
            return
        #   |  for_loop_statement     {FOR}
        if id in ["FOR"]:
            self.parse_for_loop_statement()
            return
        
        # If nothing could be matched, raise an exception
        raise ParserException(self.scanner.lineno, self.to_match, ["INT", "FLOAT", "ID", "IF", "LBRACE", "FOR"])

    #parse a declaration statement
    def parse_declaration_statement(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # declaration_statement  := INT ID SEMI   {INT} 
        if id in ["INT"]:
            self.eat("INT")
            # Add variable to the symbol table, now that it is declared
            self.table.insert(self.to_match.value, "INT")
            self.eat("ID")
            self.eat("SEMI")
            return
        #  |  FLOAT ID SEMI {FLOAT} 
        if id in ["FLOAT"]:
            self.eat("FLOAT")
            # Add variable to the symbol table, now that it is declared
            self.table.insert(self.to_match.value, "FLOAT")
            self.eat("ID")
            self.eat("SEMI")
            return

        # If nothing could be matched, raise an exception
        raise ParserException(self.scanner.lineno, self.to_match, ["INT", "FLOAT"])

    # Parse an independant assignment statement
    def parse_assignment_statement(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # assignment_statement := assignment_statement_base SEMI {ID}
        if id in ["ID"]:
            self.parse_assignment_statement_base()
            self.eat("SEMI")
            return

        # If nothing could be matched, raise an exception
        raise ParserException(self.scanner.lineno, self.to_match, ["ID"])
    
    # Parse an assignment statment
    def parse_assignment_statement_base(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # assignment_statement_base := ID ASSIGN expr {ID}
        if id in ["ID"]:
            # check to see if this id is in scope, assuming we are using the symbol table
            if(self.use_symbol_table and self.table.lookup(self.to_match.value) == None):
                raise SymbolTableException(self.scanner.lineno, self.to_match.value)
            self.eat("ID")
            self.eat("ASSIGN")
            self.parse_expr()
            return

        # If nothing could be matched, raise an exception
        raise ParserException(self.scanner.lineno, self.to_match, ["ID"])
    
    # Parse an if else
    def parse_if_else_statement(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # if_else_statement := IF LPAR expr RPAR statement ELSE statement {IF}
        if id in ["IF"]:
            self.eat("IF")
            self.eat("LPAR")
            self.parse_expr()
            self.eat("RPAR")
            self.parse_statement()
            self.eat("ELSE")
            self.parse_statement()
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["IF"])

    # Parse a block
    def parse_block_statement(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # block_statement := LBRACE statement_list RBRACE {LBRACE}
        if id in ["LBRACE"]:
            self.eat("LBRACE")
            # Added in pushing and poping scope
            self.table.push_scope()
            self.parse_statement_list()
            self.table.pop_scope()
            self.eat("RBRACE")
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["LBRACE"])

    # parse a for loop
    def parse_for_loop_statement(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # for_loop_statement := FOR LPAR assignment_statement expr SEMI assignment_statement_base RPAR statement {FOR}
        if id in ["FOR"]:
            self.eat("FOR")
            self.eat("LPAR")
            self.parse_assignment_statement()
            self.parse_expr()
            self.eat("SEMI")
            self.parse_assignment_statement_base()
            self.eat("RPAR")
            self.parse_statement()
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["FOR"])
    
    # Parse an expr
    def parse_expr(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # expr := comp expr2        {NUM, ID, LPAR}
        if id in ["NUM", "ID", "LPAR"]:
            self.parse_comp()
            self.parse_expr2()
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["NUM", "ID", "LPAR"])
    
    # Parse the second part of expr
    def parse_expr2(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # expr2 := EQ comp expr2    {EQ}
        if id in ["EQ"]:
            self.eat("EQ")
            self.parse_comp()
            self.parse_expr2()
            return
        #     | ""                {SEMI, RPAR}
        if id in ["SEMI", "RPAR"]:
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["EQ", "SEMI", "RPAR"])
    
    # Parse a comp
    def parse_comp(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # comp := factor comp2      {NUM, ID, LPAR} 
        if id in ["NUM", "ID", "LPAR"]:
            self.parse_factor()
            self.parse_comp2()
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["NUM", "ID", "LPAR"])
    
    # Parse the second part of comp
    def parse_comp2(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # comp2 := LT factor comp2  {LT}
        if id in ["LT"]:
            self.eat("LT")
            self.parse_factor()
            self.parse_comp2()
            return
        #     | ""                {SEMI, RPAR, EQ}
        if id in ["SEMI", "RPAR", "EQ"]:
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["SEMI", "RPAR", "EQ", "LT"])
    
    # Parse a factor
    def parse_factor(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # factor := term factor2         {NUM, ID, LPAR}
        if id in ["NUM", "ID", "LPAR"]:
            self.parse_term()
            self.parse_factor2()
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["NUM", "ID", "LPAR"])
    
    # Parse the second part of factor
    def parse_factor2(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # factor2 := PLUS term factor2   {PLUS}
        if id in ["PLUS"]:
            self.eat("PLUS")
            self.parse_term()
            self.parse_factor2()
            return
        #         | MINUS  term factor2  {MINUS}
        if id in ["MINUS"]:
            self.eat("MINUS")
            self.parse_term()
            self.parse_factor2()
            return
        #     | ""                {SEMI, RPAR, EQ, LT}
        if id in ["SEMI", "RPAR", "EQ", "LT"]:
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["SEMI", "RPAR", "EQ", "LT", "MINUS", "PLUS"])
    
    # Parse a term
    def parse_term(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # term := unit term2        {NUM, ID, LPAR}
        if id in ["NUM", "ID", "LPAR"]:
            self.parse_unit()
            self.parse_term2()
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["NUM", "ID", "LPAR"])
    
    # Parse the second part of factor
    def parse_term2(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

         # term2 := DIV unit term2   {DIV}
        if id in ["DIV"]:
            self.eat("DIV")
            self.parse_unit()
            self.parse_term2()
            return
        #     | MUL  unit term2   {MUL}
        if id in ["MUL"]:
            self.eat("MUL")
            self.parse_unit()
            self.parse_term2()
            return
        #     | ""                {SEMI, RPAR, EQ, LT, PLUS, MINUS}
        if id in ["SEMI", "RPAR", "EQ", "LT", "PLUS", "MINUS"]:
            return

        raise ParserException(self.scanner.lineno, self.to_match, ["SEMI", "RPAR", "EQ", "LT", "MINUS", "PLUS", "MUL", "DIV"])

    def parse_unit(self):
        # Get the value of the token to match
        id = self.getID(self.to_match)

        # unit := NUM {NUM}
        if id in ["NUM"]:
            self.eat("NUM")
            return
        
        #     |  ID  {ID} 
        if id in ["ID"]:
            # check to see if this id is in scope, assuming we are using the symbol table
            if(self.use_symbol_table and self.table.lookup(self.to_match.value) == None):
                raise SymbolTableException(self.scanner.lineno, self.to_match.value)
            self.eat("ID")
            return
        
        #     |  LPAR expr RPAR {LPAR} 
        if id in ["LPAR"]:
            self.eat("LPAR")
            self.parse_expr()
            self.eat("RPAR")
            return
        
        raise ParserException(self.scanner.lineno, self.to_match, ["NUM", "ID", "LPAR"])

    # Implement me:
    # s is the string to parse
    def parse(self, s:str):
        # Initalize the scannar with the input string
        self.scanner.input_string(s)
        # Set to match to the first lexeme
        self.to_match = self.scanner.token()
        # Then parse the intial non-terminal
        self.parse_init()
