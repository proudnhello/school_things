#include <iostream>
using namespace std;

#if defined(IR)
#include "ifir.cpp"
#else
#include "if.cpp"
#endif

int main() {
  int a = 0;
  float b = 0;
  int c = 0;
  float d = 0;
  int e = 0;
  iftest(a, b, c, d, e);
  cout << a << endl;
  cout << b << endl;
  cout << c << endl;
  cout << d << endl;
  cout << e << endl;
  return 0;
}
