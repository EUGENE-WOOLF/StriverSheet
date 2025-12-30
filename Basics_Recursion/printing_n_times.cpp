// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>

//forward tracking 

void ForwardTracking(int a , int max){
    if(a > max){
        return;
    }
    std::cout << a << std::endl;
    ForwardTracking(a + 1, max);
}

int main() {
    // Write C++ code here
    ForwardTracking(1, 10);
    return 0;
}   

