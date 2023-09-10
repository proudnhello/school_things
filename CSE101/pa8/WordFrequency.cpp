// Moore Macauley
// mmacaule
// pa8

#include<iostream>
#include<fstream>
#include<string>
#include"Dictionary.h"

using namespace std;

#define MAX_LEN 300

// returns s in lowercase
string lowerizer(string s){
    string returnable = "";
    for(unsigned long i = 0; i < s.length(); i++){
        char temp = tolower(s[i]);
        returnable.append(&temp, 1);
    }
    return returnable;
}

// Very, VERY heavily based on the c++ io example avaliable here https://people.ucsc.edu/~ptantalo/cse101/Winter23/Examples/C++/FileIO/FileIO.cpp
// May be more accurate to call it copied
int main(int argc, char * argv[]){

    size_t begin, end, len;
    ifstream in;
    ofstream out;
    string line;
    string token;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789"; 

    // check command line for correct number of arguments
    if( argc != 3 ){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    Dictionary D;

    // read each line of input file, then count and print tokens 
    while( getline(in, line) )  {
        len = line.length();

        // get first token
        begin = min(line.find_first_not_of(delim, 0), len);
        end   = min(line.find_first_of(delim, begin), len);
        token = line.substr(begin, end-begin);
        token = lowerizer(token);
        
        while( token!="" ){  // we have a token
            bool test = D.contains(token);
            if(test){
                D.getValue(token)++;
            }else{
                D.setValue(token, 1);
            }

            // get next token
            begin = min(line.find_first_not_of(delim, end+1), len);
            end   = min(line.find_first_of(delim, begin), len);
            token = line.substr(begin, end-begin);
            token = lowerizer(token);
            
        }

    }

    out << D;

    // close files 
    in.close();
    out.close();

    return 0;
}

