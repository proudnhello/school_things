import re
from functools import reduce
from time import time
import argparse
import pdb
import sys
sys.path.append("../part2/")
from tokens import tokens,Token,Lexeme
from typing import Callable,List,Tuple,Optional

# No line number this time
class ScannerException(Exception):
    pass

class NGScanner:
    def __init__(self, tokens: List[Tuple[Token,str,Callable[[Lexeme],Lexeme]]]) -> None:
        self.tokens = tokens
        self.BIG_RE = ""
        for t in self.tokens:
            # This produces (?P<TOKEN ID>RE)| for every token
            self.BIG_RE += "(?P<"
            self.BIG_RE += t[0].value
            self.BIG_RE += ">"
            self.BIG_RE += t[1]
            self.BIG_RE += ")|"
        # Remove the final | that shouldn't be there from the last token
        self.BIG_RE = self.BIG_RE[:-1]

    def input_string(self, input_string:str) -> None:
        self.istring = input_string
        
    def token(self) -> Optional[Lexeme]:
        while True:
            if len(self.istring) == 0:
                return None

            # Try and match the BIG_RE to the input string
            matcher = re.match(self.BIG_RE, self.istring)

            # If there were no matches, throw an error
            if matcher == None:
                raise ScannerException()

            actualMatch = None

            # Iterate over all the tokens, and see if we had any matches
            for t in tokens:
                if matcher[t[0].value] != None:
                    actualMatch = (t[0], matcher, t[2])
                    break            
            
            # apply the token action
            ret = actualMatch[2](Lexeme(actualMatch[0],actualMatch[1][0]))

            # figure how much we need to chop from our input string

            chop = len(ret.value)
            self.istring = self.istring[chop:]

            # if we did not match an IGNORE token, then we can
            # return the lexeme
            if ret.token != Token.IGNORE:
                return ret

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', type=str)
    parser.add_argument('--verbose', '-v', action='store_true')
    args = parser.parse_args()
    
    f = open(args.file_name)    
    f_contents = f.read()
    f.close()

    verbose = args.verbose

    s = NGScanner(tokens)
    s.input_string(f_contents)

    start = time()
    while True:
        t = s.token()
        if t is None:
            break
        if (verbose):
            print(t)
    end = time()
    print("time to parse (seconds): ",str(end-start))    
