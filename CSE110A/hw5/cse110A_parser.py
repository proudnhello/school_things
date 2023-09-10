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

    def mk_new_name(self, original_name: str) -> str:
        new_name = "_new_name_" + original_name + str(self.counter)
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
    def parse(self, s: str, uf: int) -> List[str]:

        # Set the scanner and get the first token
        self.scanner.input_string(s)
        self.to_match = self.scanner.token()
        
        if uf != None:
            # update the uf to be whatever the passed in unroll factor is
            self.uf = uf
        else:
            # if nothing is passed in initialize the unroll factor to be 1, as in the body gets added one time to the loop
            self.uf = 1

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

    # this function will validate that the passed in three address code assigns an int to a iterating variable
    def assign_unroll_test(self, code: List[str], iter_var: SymbolTableData):
        # We want to test to see if code is setting the iter variable to a constant integer
        # for this to be the case, code must be in the form
        # ['vrx = int2vr(some int);', 'iter_var_vr = (vrx);'] or ['vrx = int2vr(some int);', 'iter_var_vr = vr2int(vrx);']
        # so, we first check if code has two lines of three address code in it
        if len(code) != 2:
            return (1, 0)
        
        conversion_line = code[0]
        assign_line = code[1]
        # find the = in the first string, returning 1 if there is no = 
        equal_index = conversion_line.find("=")
        if equal_index == -1:
            return (1, 0)
        
        # This should create a string consisting of vrx
        vrx = conversion_line[0:(equal_index-1)]

        str_to_match = "= int2vr("
        # If vrx is not being assigned to a constant integer, return 1
        if len(conversion_line) < equal_index + len(str_to_match) or conversion_line[equal_index:(equal_index + len(str_to_match))] != str_to_match:
            return (1, 0)
        
        # If the second line does not match one of the two options, return 1 as well
        option1 = iter_var.get_new_name() + " = (" + vrx + ");"
        option2 = iter_var.get_new_name() + " = vr2int(" + vrx + ");" 
        if assign_line != option1 and assign_line != option2:
            return (1, 0)
        
        # find the number the iterator is being assigned to, and return it
        num_start = conversion_line.find("(") + 1
        num_end = conversion_line.find(")")
        num_str = conversion_line[num_start:num_end]

        if  not num_str.isdigit():
            return (1, 0)

        return (self.uf, int(num_str))
    
    # Check to see if tree is a valid comparison tree for unrolling
    def comparison_unroll_test(self, tree: ASTNode, iter_var:SymbolTableData):
        # To begin with, if tree is not a lt node, return
        if not isinstance(tree, ASTLtNode):
            return (1, 0)
        
        # next, if tree's left child is not an ID or IO node, return as well
        if not isinstance(tree.l_child, ASTIOIDNode) and not isinstance(tree.l_child, ASTVarIDNode):
            return (1, 0)
        
        # check to see if the left child's id matches the one passed in
        if tree.l_child.value != iter_var.get_new_name():
            return (1, 0)
        
        #finally, check to see if the right node is a number
        if not isinstance(tree.r_child, ASTNumNode):
            return (1, 0)
                
        return (self.uf, int(tree.r_child.value))

    # This function checks to see if the passed in three address code is a simple iterator or not
    def is_simple_incrementor(self, iter_var: SymbolTableData, code: List[str]):
        # If it is a simple incrementor, the code will be in one of two forms
        # ['vrx = int2vr(iter_var);', 'vry = int2vr(1);', 'vrz = addi(vrx, vry);', 'iter_var = vr2int(vrz);']
        # ['', 'vrx = int2vr(1);', 'vry = addi(iter_var, vrx);', 'iter_var = (vry);']
        # So, start by checking if there are 4 items in the passed in List
        if len(code) != 4:
            return 1
        
        # Option 2 above
        if code[0] == "":
            # Find all the vrs
            equ1 = code[1].find("=")
            equ2 = code[2].find("=")
            vrx = code[1][0:(equ1 - 1)]
            vry = code[2][0:(equ2 - 1)]

            # Build the strs to match
            to_match0 = ""
            to_match1 = (vrx + " = int2vr(1);")
            to_match2 = (vry + " = addi(" + iter_var.get_new_name() + ", " + vrx + ");")
            to_match3 = (iter_var.get_new_name() + " = (" + vry + ");")
        # Option 1 above
        else:
            # Find all the vrs
            equ0 = code[0].find("=")
            equ1 = code[1].find("=")
            equ2 = code[2].find("=")
            vrx = code[0][0:(equ0 - 1)]
            vry = code[1][0:(equ1 - 1)]
            vrz = code[2][0:(equ2 - 1)]

            # Build strs to check against
            to_match0 = vrx + " = int2vr(" + iter_var.get_new_name() + ");"
            to_match1 = vry + " = int2vr(1);"
            to_match2 = vrz + " = addi(" + vrx + ", " + vry + ");"
            to_match3 = iter_var.get_new_name() + " = vr2int(" + vrz + ");"

        # If any of them do not match, return 1
        if code[0] != to_match0:
            return 1
        if code[1] != to_match1:
            return 1
        if code[2] != to_match2:
            return 1
        if code[3] != to_match3:
            return 1

        return self.uf
        

    # you need to return a list of three address instructions
    def parse_for_statement(self) -> List[str]:
        # used to determine if this loop should be unrolled
        # all checks for unrolling first check if this is not 1, so that more complex checks
        # may be short circuited away
        this_loop_uf = self.uf
        assigned_value = 0
        less_than_value = 0

        self.eat(Token.FOR)
        self.eat(Token.LPAR)
        # for the for loop "for(i = x; i < y; i = i + z)"
        # we start by initializing i to x
        iter_var = self.symbol_table.lookup(self.to_match.value)
        code = self.parse_assignment_statement()
        start_label = self.nlg.mk_new_label()
        end_lable = self.nlg.mk_new_label()

        # Check to see if the intial assignment statement is assigning to a constant
        if this_loop_uf != 1:
            this_loop_uf, assigned_value = self.assign_unroll_test(code, iter_var)

        # then, before handling anything else, we add in a label represnting the start of the loop
        code += [start_label + ":"]

        # we then parse the comparison statement
        comp_tree = self.parse_expr()

        # test to see if the comparison is valid for unrolling
        if this_loop_uf != 1:
            this_loop_uf, less_than_value = self.comparison_unroll_test(comp_tree, iter_var)

        code += linarize_tree(comp_tree)
        false_vr = self.vra.mk_new_vr()
        code += ["%s = int2vr(0);"%(false_vr)]

        # then, if the comparison is false, we skip to after the for loop has completed
        code += ["beq(%s, %s, %s);"%(false_vr, comp_tree.vr, end_lable)]
        self.eat(Token.SEMI)

        # we then hold the increment until after the loop body
        incrementor = self.parse_assignment_statement_base()
        if this_loop_uf != 1:
            this_loop_uf = self.is_simple_incrementor(iter_var, incrementor)
        self.eat(Token.RPAR)

        # we keep track of the body of the loop
        body = self.parse_statement()

        # Check to see if we do anything that makes loop unrolling illegal
        if this_loop_uf != 1:
            for line in body:
                # If we find the iter_var = ..., then we are changing the value of the iter_var in the loop, which is illegal for unrolling
                if line.find((iter_var.get_new_name() + " =")) == 0:
                    this_loop_uf = 1
                    break
                #:s only exist as part of a label, and both beq and branch signal control follow, which makes unrolling forboden
                if line.find(":") != -1 or line.find("beq(") == 0 or line.find("branch(") == 0:
                    this_loop_uf = 1
                    break

        # and add the incrementor
        body += incrementor
        # If the number of times we will loop does not divide evenly by the unrolling factor, don't unroll
        times_looped = less_than_value - assigned_value
        if times_looped % this_loop_uf != 0:
            this_loop_uf = 1
            
        # we add the body of the loop to the code as many times as the uf demands
        # this will be set to 1 if any of the unrolling factors were violated
        # as in the body will only be added once
        for i in range(0, this_loop_uf):
            code += body

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