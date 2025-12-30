#include <bits/stdc++.h>

using namespace std;

 int reverseNumber(int n) {
  // 532
    long long num = 0;
    while(n != 0){
      int remainder = n/10;
      num = num *10 + remainder;
      n /= 10;
    }
    return num;
}

int main(){
    cout << reverseNumber(21523) << endl;
    return 0;
}