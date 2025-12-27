#include <bits/stdc++.h>

using namespace std;

 int reverseNumber(int n) {
  // 532
      int num;
      int count = 1;
      //take the number -> take remainder -> add it -> mu
      while(n != 0){
        int remainder = n%10;
        if(count == 1){
          num = remainder;
          count++;
          n = n/10;
          continue;
        }
        n = n/10;
        num *= 10;
        num += remainder;
      }
      return num;

}

int main(){
    cout << reverseNumber(21523) << endl;
    return 0;
}