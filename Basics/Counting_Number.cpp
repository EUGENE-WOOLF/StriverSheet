#include <bits/stdc++.h>

using namespace std;

void countDigit(int n) {\
    //n  = 1 
        int startFactor = 10;
        int count = 0;        
        //n = 10 
        do{
            n = n/startFactor;

            count++;
        }while(n != 0);
        cout << count << endl;
}

int main(){
    countDigit(1);
    return 0;
}