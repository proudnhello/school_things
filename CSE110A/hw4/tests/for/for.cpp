
void fortest(int &a, float &b, int &c, float &d, int &e, int &f) {
  for(a = 0; a < 100; a = a + 1){
    a = a + 1;
    b = b + 1;
  }
  for(c = 0; c == 342789; c = c + 1) c = c + 1;

  for(d = 0; d < 100; d = d + 1){
    if((d/2) < 25) e = e + 2;
    else e = e + 5;
  }

  int i;
  for(i = 100; 0 < i - 1; i = i / 2) f = f + 1;

}
