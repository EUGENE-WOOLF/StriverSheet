#include<bits/stdc++.h>

int give_sum(int &a, int &b){
    a++;
    b++;
    return a+b;
}
int main(){
    int a = 5;
    int b = 4;
    std::cout << a +b << "\n";
    give_sum(a,b );
    std::cout << a +b;

    return 0;
}