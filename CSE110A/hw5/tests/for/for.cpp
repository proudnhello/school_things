  
void fortest(int &a, float &b, int &c, float &d, int &e, int &f) {
  for(a = 0; a < 100; a = a + 1){
    a = a + 1;
    b = b + 1;
  }
  int i;
  for(i = 100; i == 342789; i = i + 1) c = c + 1;

  c = 100;
  for(i = 0; i < c; i = i + 1){
    if((i/2) < 25) e = e + 2;
    else e = e + 5;
  }

  for(i = c; 0 < i; i = i / 2) f = f + 1;

  for(i = 0; i < 100; i = i + 1){
    if(i < 50){
      d = d + 1;
    }else{
      d = d + 2;
    }
  }

}
