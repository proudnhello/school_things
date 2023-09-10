
void iftest(int &a, float &b, int &c, float &d, int &e) {
  a = 1;
  if(a) a = 2;
  else a = 3;
  b = 0.0;
  if(b) b = 2.0;
  else b = 3.0;

  int i;
  i = 0;
  if(i) c = 1;
  else c = 2;
  float j;
  j = 1.0;
  if(j) d = 1;
  else d = 2;

  if((12+162.0/163*7.2-256*162*a*b) < (12.8*d*c*j)) e = 1;
  else e = 2;

}
