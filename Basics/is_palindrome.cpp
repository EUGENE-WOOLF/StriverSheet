#include <bits/stdc++.h>
using namespace std;

 bool isPalindrome(int n) {
        int num = 0;
        int check_num = n;
        while(n != 0){
            int remainder = n%10;
            num = num * 10 + remainder;
            n = n /10;
        }
    
      bool answer = check_num == num ? true : false;
      return answer;
}

int main(){
    cout << isPalindrome(-121) << endl;
    return 0;
} 