from enum import Enum
from typing import List

# I have added print statements to make debugging the asts easier
# When called, it should peform a post order tree walk

# HOMEWORK: For each AST node you will need
# to write a "three_addr_code" function which
# writes the three address code instruction
# for each node.

# HOMEWORK: For each AST node you should also
# write a "linearize" function which provides
# a list of 3 address instructions for the node
# and its descendants.

# Hint: you may want to utilize the class hierarchy
# to avoid redundant code.

# enum for data types in ClassIeR
class Type(Enum):
    FLOAT = 1
    INT = 2

# base class for an AST node. Each node
# has a type and a VR
class ASTNode():
    def __init__(self) -> None:
        self.node_type = None
        self.vr = None

    def linearize() -> str:
        return "this should never get used, it should always be overriden"

# AST leaf nodes
class ASTLeafNode(ASTNode):
    def __init__(self, value: str) -> None:
        self.value = value
        super().__init__()
    
    def print(self):
        print(self.value)
        print(self.vr)

    # Returns the string for corrisponding const to vr conversion
    def type_converter(self) -> str:
        if(self.node_type == Type.INT):
            return "int2vr"
        return "float2vr"
    
    # This version of linearlize will be ovewritten by ASTVar, b/c it's different for them
    def linearize(self) -> str:
        return "%s = %s(%s);"%(self.vr, self.type_converter(), self.value)

######
# A number leaf node

# The value passed in should be a number
# (probably as a string).

# HOMEWORK: Determine if the number is a float
# or int and set the type
######
class ASTNumNode(ASTLeafNode):
    def __init__(self, value: str) -> None:        
        super().__init__(value)
        # The general idea of this super scuffed and hackey float detector is that floats have 
        # decimal points. Therefore, if the num has one, it must be a float
        # This should work for expr, i may need to be careful in assignment statements
        self.node_type = Type.INT
        for char in value:
            if char == ".":
                self.node_type = Type.FLOAT

    def print(self):
        print(self.value)
        print(self.node_type)
        print(self.vr)

    # Linarize is in leaf node, b/c it's the same anyway
        

######
# A program variable leaf node

# The value passed in should be an id name
# eventually it should be the new name generated
# by the symbol table to handle scopes.

# When you create this node, you will also need
# to provide its data type
######
class ASTVarIDNode(ASTLeafNode):
    def __init__(self, value: str, value_type) -> None:
        super().__init__(value)
        self.node_type = value_type

    def print(self):
        print(self.value)
        print(self.node_type)
        print(self.vr)

    # When we find a variable leaf node, we don't really want to do anything
    # b/c the vr should just be the name/value, there's no need to do anything there
    # So return an empty str
    def linearize(self) -> str:
        return ""

######
# An IO leaf node

# The value passed in should be an id name.
# Because it is an IO node, you do not need
# to get a new name for it.

# When you create this node, you will also need
# to provide its data type. It is recorded in
# the symbol table
######
class ASTIOIDNode(ASTLeafNode):
    def __init__(self, value: str, value_type) -> None:
        super().__init__(value)
        self.node_type = value_type

    def print(self):
        print(self.value)
        print(self.node_type)
        print(self.vr)

    # Linarize is in leaf node, b/c it's the same anyway

######
# Binary operation AST Nodes

# These nodes require their left and right children to be
# provided on creation
######
class ASTBinOpNode(ASTNode):
    def __init__(self, l_child, r_child) -> None:
        self.l_child = l_child
        self.r_child = r_child
        super().__init__()
    def type_converter(self) -> str:
        return "This should never get printed, it should always be overriden"
    # This version of linearize should be the same for every Binary op, so long as type converter
    # is overriden to produce the correct ir operation
    def linearize(self) -> str:
        return "%s = %s(%s, %s);"%(self.vr, self.type_converter(), self.l_child.vr, self.r_child.vr)

class ASTPlusNode(ASTBinOpNode):
    def __init__(self, l_child, r_child) -> None:
        super().__init__(l_child,r_child)
    def print(self):
        self.l_child.print()
        self.r_child.print()
        print(self.vr)
        print("+")
        print()

    def type_converter(self) -> str:
        if(self.node_type == Type.INT):
            return "addi"
        return "addf"


class ASTMultNode(ASTBinOpNode):
    def __init__(self, l_child, r_child) -> None:
        super().__init__(l_child,r_child)
    def print(self):
        self.l_child.print()
        self.r_child.print()
        print(self.vr)
        print("*")
        print()

    def type_converter(self) -> str:
        if(self.node_type == Type.INT):
            return "multi"
        return "multf"

class ASTMinusNode(ASTBinOpNode):
    def __init__(self, l_child, r_child) -> None:
        super().__init__(l_child,r_child)
    def print(self):
        self.l_child.print()
        self.r_child.print()
        print(self.vr)
        print("-")
        print()

    def type_converter(self) -> str:
        if(self.node_type == Type.INT):
            return "subi"
        return "subf"

class ASTDivNode(ASTBinOpNode):
    def __init__(self, l_child, r_child) ->None:
        super().__init__(l_child,r_child)
    def print(self):
        self.l_child.print()
        self.r_child.print()
        print(self.vr)
        print("/")
        print()
        
    def type_converter(self) -> str:
        if(self.node_type == Type.INT):
            return "divi"
        return "divf"

######
# Special BinOp nodes for comparisons

# These operations always return an int value
# (as an untyped register):
# 0 for false and 1 for true.

# Because of this, their node type is always
# an int. However, the operations (eq and lt)
# still need to be typed depending
# on their inputs. If their children are floats
# then you need to use eqf, ltf, etc.
######
class ASTEqNode(ASTBinOpNode):
    def __init__(self, l_child, r_child) ->None:
        self.node_type = Type.INT
        super().__init__(l_child,r_child)
    def print(self):
        self.l_child.print()
        self.r_child.print()
        print(self.vr)
        print("==")
        print()
        
    def type_converter(self) -> str:
        # Have to look at the left child's type b/c comp always returns an int
        if(self.l_child.node_type == Type.INT):
            return "eqi"
        return "eqf"

class ASTLtNode(ASTBinOpNode):
    def __init__(self, l_child, r_child: ASTNode) -> None:
        self.node_type = Type.INT
        super().__init__(l_child,r_child)
    def print(self):
        self.l_child.print()
        self.r_child.print()
        print(self.vr)
        print("<")
        print()

    def type_converter(self) -> str:
        # Have to look at the left child's type b/c comp always returns an int
        if(self.l_child.node_type == Type.INT):
            return "lti"
        return "ltf"

######
# Unary operation AST Nodes

# The only operations here are converting
# the bits in a virtual register to another
# virtual register of a different type,
# i.e. corresponding to the CLASSIeR instructions:
# vr_int2float and vr_float2int
######
class ASTUnOpNode(ASTNode):
    def __init__(self, child) -> None:
        self.child = child
        super().__init__()
    def type_converter(self) -> str:
        # This would be more complex if we had more types
        # But we don't, so i shleep
        if(self.node_type == Type.INT):
            return "vr_float2int"
        return "vr_int2float"
    def linearize(self) -> str:
        return "%s = %s(%s);"%(self.vr, self.type_converter(), self.child.vr)
        
class ASTIntToFloatNode(ASTUnOpNode):
    # Converts from an int to a float, the end result is fairly obvious
    def __init__(self, child) -> None:
        self.node_type = Type.FLOAT
        super().__init__(child)
    def print(self):
        print("converting to float :)")
        print(self.vr)
        self.child.print()

class ASTFloatToIntNode(ASTUnOpNode):
    # Converts from a float to an int, the end result is fairly obvious
    def __init__(self, child) -> None:
        self.node_type = Type.INT
        super().__init__(child)

    def print(self):
        print("converting to int :>")
        print(self.vr)
        self.child.print()

# These functions moved from parser to ast b/c i needed to use them here
# and parser already imports everything from ast
def is_leaf_node(node) -> bool:
    return issubclass(type(node), ASTLeafNode)

# Detects special binary operators
def is_comp_bin_op(node) -> bool:
    return isinstance(node, ASTEqNode) or isinstance(node, ASTLtNode)

def is_un_op(node) -> bool:
    return issubclass(type(node), ASTUnOpNode)

# This function will linearize the tree with a root at node
# It does so by performing a post order tree walk on that node
# Every type of AST node contains a linearize funtion that returns
# that node lineaized, so that will be used to create this tree
def linarize_tree(node : ASTNode) -> List[str]:
    # if it is a leaf node, just linearize that node and return
    if is_leaf_node(node):
        temp = node.linearize()
        return [temp]
    # if it's a uniary operator, linearize the child, then prepend the linearization of the current node
    if is_un_op(node):
        temp1 = linarize_tree(node.child)
        temp1 += [node.linearize()]
        return temp1
    # Otherwise, it must be a binary op
    # linearize the left child, the right child, and the current node
    left = linarize_tree(node.l_child)
    right = linarize_tree(node.r_child)
    current = node.linearize()
    # Then add the left and right portions together before prepending the current node
    final = left + right + [current]
    return final
