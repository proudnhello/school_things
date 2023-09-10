# change types to type ~ change IDtypes to IDtype
import pdb
from cse110A_ast import *
from typing import Callable,List,Tuple,Optional
from scanner import Lexeme,Token,Scanner
from enum import Enum

# Extra classes:

# Keeps track of the type of an ID,
# i.e. whether it is a program variable
# or an IO variable
class IDType(Enum):
    IO = 1
    VAR = 2

# The data to be stored for each ID in the symbol table
class SymbolTableData:
    def __init__(self, id_type: IDType, data_type: Type, new_name: str) -> None:
        self.id_type = id_type      # if the variable is input/output
                                    # or variable
                                    
        self.data_type = data_type  # if the variable is an int or
                                    # float
                                    
        self.new_name = new_name    # a new name to resolve collisions
                                    # in scoping

    # Getters for each of the elements
    def get_id_type(self) -> IDType:
        return self.id_type

    def get_data_type(self) -> Type:
        return self.data_type

    def get_new_name(self) -> str:
        return self.new_name

# Symbol Table exception, requires a line number and ID
class SymbolTableException(Exception):
    def __init__(self, lineno: int, ID: str) -> None:
        message = "Symbol table error on line: " + str(lineno) + "\nUndeclared ID: " + str(ID)
        super().__init__(message)

# Generates a new label when needed
class NewLabelGenerator():
    def __init__(self) -> None:
        self.counter = 0
        
    def mk_new_label(self) -> str:
        new_label = "label" + str(self.counter)
        self.counter += 1
        return new_label

# Generates a new name (e.g. for program variables)
# when needed
class NewNameGenerator():
    def __init__(self) -> None:
        self.counter = 0
        self.new_names = []

    # You may want to make a better renaming scheme
    def mk_new_name(self, original_name: str) -> str:
        new_name = original_name + str(self.counter)
        self.counter += 1
        self.new_names.append(new_name)
        return new_name
    
# Allocates virtual registers
class VRAllocator():
    def __init__(self) -> None:
        self.counter = 0
        
    def mk_new_vr(self) -> str:
        vr = "vr" + str(self.counter)
        self.counter += 1
        return vr

    # get variable declarations (needed for the C++ wrapper)
    def declare_variables(self) -> List[str]:
        ret = []
        for i in range(self.counter):
            ret.append("virtual_reg vr%d;" % i)

        return ret

# Symbol table class
class SymbolTable:
    def __init__(self) -> None:
        # stack of hashtables
        self.ht_stack = [dict()]

    # This should be the ID gotten from the program, the new ID assigned to it, then the types
    def insert(self, ID: str, new_ID: str, id_type: IDType, data_type: Type) -> None:
        
        # Create the data to store for the ID
        
        # HOMEWORK: make sure this is storing the
        # right information! You may need to use
        # the new name generator to make new names
        # for some ids
        info = SymbolTableData(id_type, data_type, new_ID)
        self.ht_stack[-1][ID] = info        

    # Lookup the symbol. If it is there, return the
    # info, otherwise return None
    def lookup(self, ID: str) -> SymbolTableData:
        for ht in reversed(self.ht_stack):
            if ID in ht:
                return ht[ID]
        return None

    def push_scope(self) -> None:
        self.ht_stack.append(dict())

    def pop_scope(self) -> None:
        self.ht_stack.pop()

# Parser Exception
class ParserException(Exception):
    
    # Pass a line number, current lexeme, and what tokens are expected
    def __init__(self, lineno: int, lexeme: Lexeme, tokens: List[Token]) -> None:
        message = "Parser error on line: " + str(lineno) + "\nExpected one of: " + str(tokens) + "\nGot: " + str(lexeme)
        super().__init__(message)

# Parser class
class Parser:

    # Creating the parser requires a scanner
    def __init__(self, scanner: Scanner) -> None:
        
        self.scanner = scanner

        # Create a symbol table
        self.symbol_table = SymbolTable()

        # objects to create virtual registers,
        # labels, and new names
        self.vra = VRAllocator()
        self.nlg = NewLabelGenerator()
        self.nng = NewNameGenerator()

        # needed to create the C++ wrapper
        # You do not need to modify these for the
        # homework
        self.function_name = None
        self.function_args = []

    # HOMEWORK: top level function:
    # This needs to return a list of 3 address instructions
    def parse(self, s: str) -> List[str]:

        # Set the scanner and get the first token
        self.scanner.input_string(s)
        self.to_match = self.scanner.token()

        # start parsing. In your solution, p must contain a list of
        # three address instructions
        p = self.parse_function()
        self.eat(None)
        
        return p

    # Helper fuction: get the token ID
    def get_token_id(self, l: Lexeme) ->Token:
        if l is None:
            return None
        return l.token

    # Helper fuction: eat a token ID and advance
    # to the next token
    def eat(self, check: Token) -> None:
        token_id = self.get_token_id(self.to_match)
        if token_id != check:
            raise ParserException(self.scanner.get_lineno(),
                                  self.to_match,
                                  [check])      
        self.to_match = self.scanner.token()

    # The top level parse_function
    def parse_function(self) -> List[str]:

        # I am parsing the function header for you
        # You do not need to do anything with this.
        self.parse_function_header()    
        self.eat(Token.LBRACE)

        # your solution should have p containing a list
        # of three address instructions
        p = self.parse_statement_list()        
        self.eat(Token.RBRACE)
        return p

    # You do not need to modify this for your homework
    # but you can look :) 
    def parse_function_header(self) -> None:
        self.eat(Token.VOID)
        function_name = self.to_match.value
        self.eat(Token.ID)        
        self.eat(Token.LPAR)
        self.function_name = function_name
        args = self.parse_arg_list()
        self.function_args = args
        self.eat(Token.RPAR)

    # You do not need to modify this for your homework
    # but you can look :) 
    def parse_arg_list(self) -> List[Tuple[str, str]]:
        token_id = self.get_token_id(self.to_match)
        if token_id == Token.RPAR:
            return
        arg = self.parse_arg()
        token_id = self.get_token_id(self.to_match)
        if token_id == Token.RPAR:
            return [arg]
        self.eat(Token.COMMA)
        arg_l = self.parse_arg_list()
        return arg_l + [arg]

    # You do not need to modify this for your homework
    # but you can look :) 
    def parse_arg(self) -> Tuple[str, str]:
        token_id = self.get_token_id(self.to_match)
        if token_id == Token.FLOAT:
            self.eat(Token.FLOAT)
            data_type = Type.FLOAT
            data_type_str = "float"            
        elif token_id == Token.INT:
            self.eat(Token.INT)
            data_type = Type.INT
            data_type_str = "int"
        else:
            raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.INT, Token.FLOAT])
        self.eat(Token.AMP)
	# change strings and indexing token.names .value .token
        id_name = self.to_match.value
        self.eat(Token.ID)

        # storing an IO variable to the symbol table
        # new name and current name should be identical, for io variable, so pass in identical strings
        self.symbol_table.insert(id_name, id_name, IDType.IO, data_type)
        return (id_name, data_type_str)
        
    # The top level parsing function for your homework
    # This function needs to return a list of three address codes
    def parse_statement_list(self) -> List[str]:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.INT, Token.FLOAT, Token.ID, Token.IF, Token.LBRACE, Token.FOR]:
            current = self.parse_statement()
            future = self.parse_statement_list()
            #print(current)
            #print(future)
            #print("")
            return current + future
        if token_id in [Token.RBRACE]:
            return []
        
    # you need to return a list of three address instructions
    # from the statement that gets parsed
    def parse_statement(self) -> List[str]:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.INT, Token.FLOAT]:
            return self.parse_declaration_statement()
        elif token_id in [Token.ID]:
            return self.parse_assignment_statement()
        elif token_id in [Token.IF]:
            return self.parse_if_else_statement()
        elif token_id in [Token.LBRACE]:
            return self.parse_block_statement()
        elif token_id in [Token.FOR]:
            return self.parse_for_statement()
        else:
            raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.FOR, Token.IF, Token.LBRACE, Token.INT, Token.FLOAT, Token.ID])

    # you need to return a list of three address instructions
    def parse_declaration_statement(self) -> List[str]:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.INT]:
            self.eat(Token.INT)
            id_name = self.to_match.value
            # Add the ID and it's new name to the ht.
            # With how this is set up, lookup will still happen with ID, but the info retuned will referance the corresponding new name
            self.symbol_table.insert(id_name, self.nng.mk_new_name(id_name), IDType.VAR, Type.INT)
            self.eat(Token.ID)
            self.eat(Token.SEMI)
            # declaration statements only need to add to the symbol table, so return an empty list
            return []
        if token_id in [Token.FLOAT]:
            self.eat(Token.FLOAT)
            id_name = self.to_match.value
            # Add the ID and it's new name to the ht.
            # With how this is set up, lookup will still happen with ID, but the info retuned will referance the corresponding new name
            self.symbol_table.insert(id_name, self.nng.mk_new_name(id_name), IDType.VAR, Type.FLOAT)
            self.eat(Token.ID)
            self.eat(Token.SEMI)
            # declaration statements only need to add to the symbol table, so return an empty list
            return []
        
        raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.INT, Token.FLOAT])

    # you need to return a list of three address instructions
    def parse_assignment_statement(self) -> List[str]:
        temp = self.parse_assignment_statement_base()
        self.eat(Token.SEMI)
        # all of the three address instructions will be handles in parse base
        return temp
 
    # you need to return a list of three address instructions
    def parse_assignment_statement_base(self) -> List[str]:
        id_name = self.to_match.value
        id_data = self.symbol_table.lookup(id_name)
        if id_data == None:
            raise SymbolTableException(self.scanner.get_lineno(), id_name)
        self.eat(Token.ID)
        self.eat(Token.ASSIGN)
        tree = self.parse_expr()
        assined_value = linarize_tree(tree)
        linarized_base = []
        if(id_data.get_id_type() == IDType.VAR):
            conversion_function = ""
            # if we are assigning to an interal variable, and the types match, no conversion are needed
            if tree.node_type == id_data.get_data_type():
                pass
            # if the var is a float and they don't match, then we need to convert to an int
            elif tree.node_type == Type.FLOAT:
                conversion_function = "vr_float2int"
            # otherwise, we need to convert to a float
            else:
                conversion_function = "vr_int2float"
            linarized_base = ["%s = %s(%s);"%(id_data.new_name, conversion_function, tree.vr)]
        else:
            # if we are assigning to an IO variable, we must first convert the vr to the correct type
            # if nessisary, then convert the vr to a float/int
            vr_to_use = tree.vr
            # if the data types are equal, converting the vr to an appropriate type is unnessisary
            if(id_data.get_data_type() == tree.node_type):
                pass
            # if the node type is a float and they were not equal, then the IO type must be an int
            # make a new vr and assign the converted value to that vr
            elif tree.node_type == Type.FLOAT:
                vr_to_use = self.vra.mk_new_vr()
                linarized_base += ["%s = %s(%s);"%(vr_to_use, "vr_float2int", tree.vr)]
            # otherwise, then the IO type must be a float, and the node type an int
            # make a new vr and assign the converted value to that vr
            else:
                vr_to_use = self.vra.mk_new_vr()
                linarized_base += ["%s = %s(%s);"%(vr_to_use, "vr_int2float", tree.vr)]
            if  id_data.get_data_type() == Type.FLOAT:
                linarized_base += ["%s = %s(%s);"%(id_data.get_new_name(), "vr2float", vr_to_use)]
            else:
                linarized_base += ["%s = %s(%s);"%(id_data.get_new_name(), "vr2int", vr_to_use)]

        return assined_value + linarized_base

    # you need to return a list of three address instructions
    def parse_if_else_statement(self) -> List[str]:
        self.eat(Token.IF)
        self.eat(Token.LPAR)
        tree = self.parse_expr()
        code = linarize_tree(tree)
        # first, we need to make a new vr that contains a 0 for the branch
        false_vr = self.vra.mk_new_vr()
        code += ["%s = int2vr(0);"%(false_vr)]
        start_of_else = self.nlg.mk_new_label()
        end_of_else = self.nlg.mk_new_label()
        # this should branch to the start of the else statement if the value of the expr that
        # tree represents is false
        code += ["beq(%s, %s, %s);"%(false_vr, tree.vr, start_of_else)]
        self.eat(Token.RPAR)
        # this is the statement inside the if
        code += self.parse_statement()
        # at the end of the if, skip past the else
        code += ["branch(%s);"%(end_of_else)]
        code += [start_of_else + ":"]
        self.eat(Token.ELSE)
        # this is the else statement
        code += self.parse_statement()
        # add the label at the bottom for skipping past
        code += [end_of_else + ":"]
        return code
    
    # you need to return a list of three address instructions
    def parse_block_statement(self) -> List[str]:
        self.eat(Token.LBRACE)
        self.symbol_table.push_scope()
        temp = self.parse_statement_list()
        self.symbol_table.pop_scope()
        self.eat(Token.RBRACE)
        # Due to how we handle scopes, no new 3 address code needs to be created
        # so just return whatever the internal statement list returned
        return temp

    # you need to return a list of three address instructions
    def parse_for_statement(self) -> List[str]:
        self.eat(Token.FOR)
        self.eat(Token.LPAR)
        # for the for loop "for(i = x; i < y; i = i + z)"
        # we start by initializing i to x
        code = self.parse_assignment_statement()
        start_label = self.nlg.mk_new_label()
        end_lable = self.nlg.mk_new_label()
        # then, before handling anything else, we add in a label represnting the start of the loop
        code += [start_label + ":"]
        # we then parse the comparison statement
        comp_tree = self.parse_expr()
        code += linarize_tree(comp_tree)
        false_vr = self.vra.mk_new_vr()
        code += ["%s = int2vr(0);"%(false_vr)]
        # then, if the comparison is false, we skip to after the for loop has completed
        code += ["beq(%s, %s, %s);"%(false_vr, comp_tree.vr, end_lable)]
        self.eat(Token.SEMI)
        # we then hold the increment until after the loop body
        incrementor = self.parse_assignment_statement_base()
        self.eat(Token.RPAR)
        # we do whatever is inside the for loop
        code += self.parse_statement()
        # before incrementing at the end
        code += incrementor
        # we then jump up to the top to loop through again
        code += ["branch(%s);"%(start_label)]

        # finally, we add the lable that the comparison will use to escape if it is false
        code += [end_lable + ":"]
        return code

    # you need to build and return an AST
    def parse_expr(self) -> ASTNode:        
        left_node = self.parse_comp()
        temp = self.parse_expr2(left_node)
        # While I *could* run type inference everywhere parse_expr is called, i could also do it here
        # b4 it returns. This is inefficient due to (expr)s not really needing type checking here, but
        # is more easily extendable b/c i don't need to think about this every time i parse a new expr
        type_inference(temp, self.vra)
        return temp

    # you need to build and return an AST
    def parse_expr2(self, left_node) -> ASTNode:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.EQ]:
            self.eat(Token.EQ)
            right_node = self.parse_comp()
            new_left = ASTEqNode(left_node, right_node)
            return self.parse_expr2(new_left)
        if token_id in [Token.SEMI, Token.RPAR]:
            return left_node
        
        raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.EQ, Token.SEMI, Token.RPAR])
    
    # you need to build and return an AST
    def parse_comp(self) -> ASTNode:
        left_node = self.parse_factor()
        return self.parse_comp2(left_node)

    # you need to build and return an AST
    def parse_comp2(self, left_node) -> ASTNode:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.LT]:
            self.eat(Token.LT)
            right_node = self.parse_factor()
            new_left = ASTLtNode(left_node, right_node)
            return self.parse_comp2(new_left)
        if token_id in [Token.SEMI, Token.RPAR, Token.EQ]:
            return left_node
        
        raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.EQ, Token.SEMI, Token.RPAR, Token.LT])

    # you need to build and return an AST
    def parse_factor(self) -> ASTNode:
        left_node = self.parse_term()
        return self.parse_factor2(left_node)

    # you need to build and return an AST
    def parse_factor2(self, left_node) -> ASTNode:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.PLUS]:
            self.eat(Token.PLUS)
            right_node = self.parse_term()      
            new_left = ASTPlusNode(left_node, right_node)      
            return self.parse_factor2(new_left)
        if token_id in [Token.MINUS]:
            self.eat(Token.MINUS)
            right_node = self.parse_term()      
            new_left = ASTMinusNode(left_node, right_node)   
            return self.parse_factor2(new_left)
        if token_id in [Token.EQ, Token.SEMI, Token.RPAR, Token.LT]:
            return left_node

        raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.EQ, Token.SEMI, Token.RPAR, Token.LT, Token.PLUS, Token.MINUS])
    
    # you need to build and return an AST
    def parse_term(self) -> ASTNode:
        left_node = self.parse_unit()
        # If there is no new node to be made, then parse_term2 will shrimply return left node again
        return self.parse_term2(left_node)

    # you need to build and return an AST
    def parse_term2(self, left_node) -> ASTNode:
        token_id = self.get_token_id(self.to_match)
        if token_id in [Token.DIV]:
            self.eat(Token.DIV)
            right_node = self.parse_unit()
            new_left = ASTDivNode(left_node, right_node)
            return self.parse_term2(new_left)
        if token_id in [Token.MUL]:
            self.eat(Token.MUL)
            right_node = self.parse_unit()
            new_left = ASTMultNode(left_node, right_node)
            return self.parse_term2(new_left)
        if token_id in [Token.EQ, Token.SEMI, Token.RPAR, Token.LT, Token.PLUS, Token.MINUS]:
            return left_node

        raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.EQ, Token.SEMI, Token.RPAR, Token.LT, Token.PLUS, Token.MINUS, Token.MUL, Token.DIV])

    # you need to build and return an AST
    def parse_unit(self) -> ASTNode:
        token_id = self.get_token_id(self.to_match)
        token = self.to_match
        # if it's a number, return a number node
        if token_id in [Token.NUM]:
            self.eat(Token.NUM)            
            return ASTNumNode(token.value)
        if token_id in [Token.ID]:
            id_name = self.to_match.value
            id_data = self.symbol_table.lookup(id_name)
            if id_data == None:
                raise SymbolTableException(self.scanner.get_lineno(), id_name)
            self.eat(Token.ID)

            # Return the corresponding type of ID node based on the lookup
            if(id_data.id_type == IDType.IO):
                # Based on symbol table implmentation, the new name of IO nodes == old name
                return ASTIOIDNode(id_data.get_new_name(), id_data.get_data_type())
            return ASTVarIDNode(id_data.get_new_name(), id_data.get_data_type())
        
        if token_id in [Token.LPAR]:
            # parens don't add anything to an ast, so just return whatever the expr returns
            self.eat(Token.LPAR)
            temp = self.parse_expr()
            self.eat(Token.RPAR)
            return temp
            
        raise ParserException(self.scanner.get_lineno(),
                              self.to_match,            
                              [Token.NUM, Token.ID, Token.LPAR])    

# Type inference top level
def type_inference(node, allocator : VRAllocator) -> Type:
    
    # All our uniary operators are conversions, making their type as trivial as leaf nodes
    if is_leaf_node(node) or is_un_op(node):
        # Might as well assign vrs while I'm iterating over the AST anyway
        if(isinstance(node, ASTVarIDNode)):
            node.vr = node.value
        else:
            node.vr = allocator.mk_new_vr()
        return node.node_type
    
    # At this point we know we have a binary operator, so call type inferance on both childern
    left = type_inference(node.l_child, allocator)
    right = type_inference(node.r_child, allocator)

    # Might as well assign vrs while I'm iterating over the AST anyway
    node.vr = allocator.mk_new_vr()
    
    # If we have a comparison binary operator (ie == or <) do special things
    if is_comp_bin_op(node):
        # If both childern are the same type, then we do nothing and return int
        if left == right:
            node.node_type = Type.INT
            return Type.INT
        
        # If they are different types, convert one to float, then compare before returning type int
        # Convert to float b/c 4.5 == 4 should be false, but the truncation due to float to int would make
        # them equal when they shouldn't be
        if left == Type.INT:
            temp = node.l_child
            node.l_child = ASTIntToFloatNode(temp)
            # Can't forget to give the conversion node it's own vr
            node.l_child.vr = allocator.mk_new_vr()

        if right == Type.INT:
            temp = node.r_child
            node.r_child = ASTIntToFloatNode(temp)
            # Can't forget to give the conversion node it's own vr
            node.r_child.vr = allocator.mk_new_vr()

        node.node_type = Type.INT
        return Type.INT
    
    # Finally, if all that is left are the standard binary operators
    # if both childern are the same type, return whatever that type may be
    if left == right:
        node.node_type = left
        return left
    
    # Otherwise, find the int and convert it before returning type float

    if left == Type.INT:
        temp = node.l_child
        node.l_child = ASTIntToFloatNode(temp)
        # Can't forget to give the conversion node it's own vr
        node.l_child.vr = allocator.mk_new_vr()

    if right == Type.INT:
        temp = node.r_child
        node.r_child = ASTIntToFloatNode(temp)
        # Can't forget to give the conversion node it's own vr
        node.r_child.vr = allocator.mk_new_vr()

    node.node_type = Type.FLOAT
    return Type.FLOAT
    
    # next check if it is a unary op, then a bin op.
    # remember that comparison operators (eq and lt)
    # are handled a bit differently
