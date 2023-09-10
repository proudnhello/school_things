// Moore Macauley
// mmacaule
// pa8
#include"Dictionary.h"
#include<iostream>
#include<fstream>
#include<string>

int main(int argc, char const *argv[]){

    std::ifstream in;
    std::ofstream out;

    // Check to ensure the right number of args were passed
    if( argc != 3 ){
      std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
      return(EXIT_FAILURE);
    }

    // Open infile
    in.open(argv[1]);
    if( !in.is_open() ){
        std::cerr << "Unable to open file " << argv[1] << " for reading" << std::endl;
        return(EXIT_FAILURE);
    }

    // Open outfile
    out.open(argv[2]);
    if( !out.is_open() ){
        std::cerr << "Unable to open file " << argv[2] << " for writing" << std::endl;
        return(EXIT_FAILURE);
    }

    std::string line;
    Dictionary D;
    int lineNumber = 1;
    // Read in lines, adding them and their line number to the dictionary
    while(getline(in, line)){
        D.setValue(line, lineNumber);
        lineNumber++;
    }

    out << D;
    out << "\n";
    out << D.pre_string();

    return 0;
}
