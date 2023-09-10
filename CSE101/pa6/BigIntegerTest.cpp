// Moore Macauley
// mmacaule
// pa6
#include"BigInteger.h"
#include<iostream>

// All tests were designed with power = 2 and base = 100 in mind, and tested with that and power = 9 base = 1b

int main(int argc, char const *argv[])
{

    BigInteger a("-123456789");
    std::cout << a;
    std::cout << "\n";

    std::cout << "-----------------------------------------------------------\n";

    BigInteger b("-123456789");
    BigInteger c("0");
    BigInteger d("42");
    BigInteger e("-999999999");
    std::cout << b;
    std::cout << "\n";
    std::cout << a.compare(b);
    std::cout << "\n";
    std::cout << c;
    std::cout << "\n";
    std::cout << a.compare(c);
    std::cout << "\n";
    std::cout << d;
    std::cout << "\n";
    std::cout << a.compare(d);
    std::cout << "\n";
    std::cout << e;
    std::cout << "\n";
    std::cout << a.compare(e);
    std::cout << "\n";

    std::cout << "-----------------------------------------------------------\n";
    
    b.makeZero();
    d.negate();
    std::cout << b;
    std::cout << "\n";
    std::cout << d;
    std::cout << "\n";

    std::cout << "-----------------------------------------------------------\n";
    BigInteger f("4212");
    BigInteger g("5080");
    BigInteger h("11111");
    BigInteger i("9999");
    std::cout << f;
    std::cout << "\n";
    std::cout << g;
    std::cout << "\n";
    std::cout << h;
    std::cout << "\n";
    std::cout << i;
    std::cout << "\n";

    std::cout << f.add(g);
    std::cout << "\n";
    std::cout << f.add(h);
    std::cout << "\n";
    std::cout << f.add(i);
    std::cout << "\n";
    std::cout << "-----------------------------------------------------------\n";

    BigInteger j("-4212");
    BigInteger k("5080");
    BigInteger l("-11111");
    BigInteger m("9999");

    std::cout << k.add(j);
    std::cout << "\n";
    std::cout << j.add(k);
    std::cout << "\n";
    std::cout << j.add(l);
    std::cout << "\n";
    std::cout << k.add(l);
    std::cout << "\n";
    std::cout << j.add(m);
    std::cout << "\n";

    std::cout << "-----------------------------------------------------------\n";

    std::cout << f.sub(g);
    std::cout << "\n";
    std::cout << f.sub(h);
    std::cout << "\n";
    std::cout << f.sub(i);
    std::cout << "\n";
    std::cout << "-----------------------------------------------------------\n";

    std::cout << k.sub(j);
    std::cout << "\n";
    std::cout << j.sub(k);
    std::cout << "\n";
    std::cout << j.sub(l);
    std::cout << "\n";
    std::cout << k.sub(l);
    std::cout << "\n";
    std::cout << j.sub(m);
    std::cout << "\n";

    std::cout << "-----------------------------------------------------------\n";

    std::cout << f.mult(g);
    std::cout << "\n";
    std::cout << f.mult(h);
    std::cout << "\n";
    std::cout << f.mult(i);
    std::cout << "\n";
    std::cout << "-----------------------------------------------------------\n";

    std::cout << k.mult(j);
    std::cout << "\n";
    std::cout << j.mult(k);
    std::cout << "\n";
    std::cout << j.mult(l);
    std::cout << "\n";
    std::cout << k.mult(l);
    std::cout << "\n";
    std::cout << j.mult(m);
    std::cout << "\n";
    
    BigInteger smol("111122223333");
    BigInteger veryLarge("111122223333");

    std::cout << smol.mult(veryLarge);
    std::cout << "\n";

return 0;

}
