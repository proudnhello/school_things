// Moore Macauley
// mmacaule
// pa6
#include"BigInteger.h"
#include"List.h"
#include<iostream>
#include<string>
#include<stdexcept>

const int base = 1000000000; // SET THIS TO 1000000000 BEFORE TURNING IT IN!
const int power = 9; // SET THIS TO 9 BEFORE TURNING IT IN!

BigInteger::BigInteger(){
    signum = 0;
    List temp;
    digits = temp;
}

void clean(List& L){
    L.moveFront();
    // Removes leading 0 digits from L
    // IE 0000 00000 1234 becomes 1234
    while(L.position() < L.length()){
        L.moveNext();
        if(L.peekPrev() == 0){
            L.eraseBefore();
        }else{
            break;
        }
    }
}

BigInteger::BigInteger(const BigInteger& N){
    signum = N.signum;
    digits = N.digits;
}

BigInteger::BigInteger(std::string s){
    if(s.empty()){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    List temp;
    digits = temp;

    long end;

    // If there's a leading - or +, set signum appropriately, and tell the loop to ignore the + or -
    if(s[0] == '-'){
        signum = -1;
        end = 1;
    }else if(s[0] == '+'){
        signum = 1;
        end = 1;
    }else{
        end = 0;
        signum = 1;
    }

    // Iterate over all of the chars in s backwards
    for(long i = s.length()-1; i >= end;){
        std::string digit = "";
        long jLimit = i-power;
        // Iterate over power # of chars
        for(long j = i; (j > jLimit) && (j >= end); (j--),(i--)){
            // Throw an error if any char is not a digit
            if(!(std::isdigit(s[j]))){
                throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
            }
            // Add the char from s to the ending digit
            digit = s[j] + digit;
        }
        // Insert the digit at the begining of the List
        digits.insertAfter(std::stol(digit));
    }

    clean(digits);
    // If clean removed all the digits, set signum to 0
    if(digits.length() == 0){
        signum = 0;
    }
}

int BigInteger::sign() const{
    return signum;
}

int BigInteger::compare(const BigInteger& N) const{
    List dig = digits;
    List NDig = N.digits;

    // if they are both 0, then the result is obvious
    if(signum == 0 && N.signum == 0){
        return 0;
    }

    // Similarly, if one is negative while the other positive, then the result is also obvious
    if(signum > N.signum){
        return 1;
    }else if (signum < N.signum){
        return -1;
    }

    // If one has more digits than the other, then one is obviously greater
    // Multiply by -1 if the signum is -1, b/c a negitve number with a larger magnitude is, in fact, smaller
    if((dig.length())*signum > NDig.length()*N.signum){
        return 1;
    }else if (NDig.length()*signum > dig.length()*N.signum){
        return -1;
    }
    
    dig.moveFront();
    NDig.moveFront();
    // Starting at the largest digit, if one digit is larger than the other, then that int must also be larger
    // For example, 1xxxx is obviously smaller than 5xxxx
    // We then compare all of the digits
    while(dig.position() < dig.length()){
        dig.moveNext();
        NDig.moveNext();
        // All values multiplied by the signum to make negative comparisons work properly
        if((dig.peekPrev())*signum > (NDig.peekPrev())*N.signum){
            return 1;
        }else if((dig.peekPrev())*signum < (NDig.peekPrev())*signum){
            return -1;
        }
    }
    // If all the digits were equal, then the ints were equal
    return 0;
}

void BigInteger::makeZero(){
    signum = 0;
    digits.clear();
}

void BigInteger::negate(){
    // 1 * -1 = -1, -1 * -1 = 1, and 0 * -1 = 0
    signum *= -1;
}

void negateList(List &L){
    L.moveFront();
    while(L.position() < L.length()){
        L.moveNext();
        L.setBefore(L.peekPrev() * -1);
    }
}

int normalize(List &L){
    if(L.front() < 0){
        negateList(L);
        normalize(L);
        return -1;
    }
    L.moveBack();
    long carry = 0;

    while(L.position() > 0){
        L.movePrev();
        L.setAfter(L.peekNext() + carry);
        carry = 0;
        long val = L.peekNext();
        if(val >= base){
            carry = val/base;
            L.setAfter(val % base);
        }else if(val < 0){
            carry = -1;
            L.setAfter(val + base);
        }
    }
    if(carry > 0){
        L.insertBefore(carry);
    }

    clean(L);
    if(L.length() == 0){
        return 0;
    }

    return 1;
}

void sumList(List&S, List A, List B, int sgn){
    int len = 0;
    if(A.length() < B.length()){
        len = A.length();
    }else{
        len = B.length();
    }

    A.moveBack();
    B.moveBack();

    // Iterate over all of the digits, starting at the lsd, adding as we go
    for(int i = 0; i < len; i++){
        A.movePrev();
        B.movePrev();
        S.insertAfter(A.peekNext() + (B.peekNext() * sgn));
    }
    // If there are any remianing digits in A, add them on
    while(A.position() > 0){
        A.movePrev();
        S.insertAfter(A.peekNext());
    }
    // Do the same for B
    while(B.position() > 0){
        B.movePrev();
        S.insertAfter(B.peekNext() * sgn);
    }
}

BigInteger BigInteger::add(const BigInteger& N) const{
    // If we are adding a negative to a postive, subtract the abs value of the negative from the positive instead
    // -x + y = y - x
    if(signum == -1 && N.signum != -1){
        BigInteger temp = *this;
        temp.negate();
        return N.sub(temp);
    // x + (-y) = x - y
    }else if(N.signum == -1 && signum != -1){
        BigInteger temp = N;
        temp.negate();
        return sub(temp);
    }
    BigInteger C;
    List A = digits;
    List B = N.digits;
    sumList(C.digits, A, B, 1);
    // * signum as if we are adding two negatives, then the result should be negative instead
    C.signum = normalize(C.digits) * signum;
    return C;
}

BigInteger BigInteger::sub(const BigInteger& N) const{
    // If we are subtracting a negative from a positive or visa versa, add the magnitudes instead
    // -x - y = -x + -y                 or  x - (-y) = x + y 
    if((signum == -1 && N.signum != -1) || (N.signum == -1 && signum != -1)){
        BigInteger temp = N;
        temp.negate();
        return add(temp);
    }

    BigInteger C;
    List A = digits;
    List B = N.digits;
    sumList(C.digits, A, B, -1);
    // * signum as if we are subracting two negatives, then the result should be inverted
    C.signum = normalize(C.digits) * signum;
    return C;
}

void shiftList(List& L, int p){
    L.moveBack();
    for(int i = 0; i < p; i++){
        L.insertBefore(0);
    }
}

void scalarMult(List& L, ListElement p){
    L.moveBack();
    while(L.position() > 0){
        L.movePrev();
        L.setAfter(L.peekNext() * p);
    }
}

BigInteger BigInteger::mult(const BigInteger& N) const{
    // If N or this are 0, then the resulting BigInt must be 0
    if(signum == 0 || N.signum == 0){
        BigInteger temp;
        return temp;
    }

    BigInteger returnable;
    List A = digits;
    List B = N.digits;
    int shift = 0;

    // For every digit in A
    // Multiply every digit in B by that A
    // Shift the result over the position of the A digit
    // Then sum the result
    // So in base 10, 15*27 would look like 5*27 + 10 * 27
    // Just like long multiplication on paper

    A.moveBack();
    while(A.position() > 0){
        List temp = B;
        A.movePrev();
        ListElement scalar = A.peekNext();
        scalarMult(temp, scalar);
        shiftList(temp, shift);
        shift++;
        List temp2; // The temp 2 rubbish is done b/c sumList implodes violently when two of the parameters are the same
        sumList(temp2, returnable.digits, temp, 1);
        returnable.digits = temp2;
        normalize(returnable.digits);
    }

    if((signum == 1 && N.signum == 1) || (signum == -1 && N.signum == -1)){
        returnable.signum = 1;
    }else{
        returnable.signum = -1;
    }

    return returnable;
}

// Returns a string containing x and the appropriate number of leading 0s
std::string digitToString(ListElement x){
    std::string value = "";
    int leadingZeros = base/10;
    // If x % leadingZeros == x, that means that there is a 0 in that digit, and as such a leading 0 must be added
    while(leadingZeros > 1 && x % leadingZeros == x){
        value += "0";
        leadingZeros /= 10;
    }
    value += std::to_string(x);
    return value;         
}  

std::string BigInteger::to_string(){
    if(signum == 0){
        return "0";
    }

    std::string value = "";
    if(signum == -1){
        value += "-";
    }

    digits.moveFront();
    digits.moveNext();
    // Immitaitely add the first value, as digitToString will add leading 0s, which we don't want for this
    value += std::to_string(digits.peekPrev());

    while(digits.position() < digits.length()){
        digits.moveNext();
        value += digitToString(digits.peekPrev());
    }

    return value;
}

std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream << N.to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == 0){
        return true;
    }
    return false;
}

   // operator<()
   // Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ){
    if(A.compare(B) == -1){
        return true;
    }
    return false;
}

   // operator<=()
   // Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ){
    int test = A.compare(B);
    if(test == -1 || test == 0){
        return true;
    }
    return false;
}

   // operator>()
   // Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ){
    int test = A.compare(B);
    if(test == 1){
        return true;
    }
    return false;
}

   // operator>=()
   // Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ){
    int test = A.compare(B);
    if(test == 1 || test == 0){
        return true;
    }
    return false;
}

   // operator+()
   // Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
}

   // operator+=()
   // Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    A = A.add(B);
    return A;
}

   // operator-()
   // Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
}

   // operator-=()
   // Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    A = A.sub(B);
    return A;
}

   // operator*()
   // Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
}

   // operator*=()
   // Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A.mult(B);
    return A;
}