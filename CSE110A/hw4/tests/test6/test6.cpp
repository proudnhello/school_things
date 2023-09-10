
void test6(int &x, int &y) {
  float potato;
  int i;
  i = i + 5;
  {
    int i;
    i = i + 6.9;
    {
      x = i + 7;
      x = potato + i;
    }
  }
  potato = potato + 6.2;
  potato = 2.2 + 2.2;
  y = i + 8.8;  
  i = y + x;
  x = i == i;
}
