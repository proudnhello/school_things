#include <iostream>
using namespace std;

#if defined(IR)
#include "forir.cpp"
#else
#include "for.cpp"
#endif

int main() {
  int a = 0;
  float b = 0;
  int c = 0;
  float d = 0;
  int e = 0;
  int f = 0;
  fortest(a, b, c, d, e, f);
  cout << a << endl;
  cout << b << endl;
  cout << c << endl;
  cout << d << endl;
  cout << e << endl;
  cout << f << endl;
  return 0;
}
