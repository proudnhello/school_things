// Moore Macauley
// mmacaule
// pa6
#include"BigInteger.h"
#include<iostream>
#include<fstream>
#include<string>

int main(int argc, char const *argv[]){

    std::ifstream in;
    std::ofstream out;

    if( argc != 3 ){
      std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
      return(EXIT_FAILURE);
    }

    in.open(argv[1]);
    if( !in.is_open() ){
        std::cerr << "Unable to open file " << argv[1] << " for reading" << std::endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
        std::cerr << "Unable to open file " << argv[2] << " for writing" << std::endl;
        return(EXIT_FAILURE);
    }

    std::string firstNum;
    std::string empty;
    std::string secondNum;

    getline(in, firstNum);
    getline(in, empty);
    getline(in, secondNum);

    BigInteger A(firstNum);
    BigInteger B(secondNum);

    // First, print A and B
    out << A << "\n\n";
    out << B << "\n\n";

    // Then A+B, A-B, and A-A
    out << A+B << "\n\n";
    out << A-B << "\n\n";
    out << A-A << "\n\n";

    // 3A - 2B
    out << (A + A + A - B - B) << "\n\n";

    // AB, A*A and B*B
    out << A*B << "\n\n";
    out << A*A << "\n\n";
    out << B*B << "\n\n";

    // And finally, 9A^4 + 16B^5
    BigInteger total;
    BigInteger sixteen("16");
    BigInteger nine("9");
    BigInteger A4;
    BigInteger B5;

    A4 = A*A*A*A;
    B5 = B*B*B*B*B;

    total = (A4*nine) + (B5*sixteen);
    out << total;

    return 0;
}
