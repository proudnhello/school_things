#include <iostream>
using namespace std;

#if defined(IR)
#include "for2ir.cpp"
#else
#include "for2.cpp"
#endif

int main() {
  int a = 0;
  float b = 0;
  int c = 0;
  float d = 0;
  int e = 0;
  int f = 0;
  int g = 0;
  int h = 0;
  fortest(a, b, c, d, e, f, g, h);
  cout << a << endl;
  cout << b << endl;
  cout << c << endl;
  cout << d << endl;
  cout << e << endl;
  cout << f << endl;
  cout << g << endl;
  cout << h << endl;
  return 0;
}
