  
void fortest(int &a, float &b, int &c, float &d, int &e, int &f, int &g, int &h) {
  a = 0;
  int i;
  int j;
  int x;
  x = 1;
  int y;
  y = 2;
  for(i = 0; i < 100; i = i + 1){
    a = x + y;
    b = y + x;
    c = x - y;
    d = y - x;
    e = x == y;
    f = y == x;
    g = x * y;
    h = y * x;
  }

  }
