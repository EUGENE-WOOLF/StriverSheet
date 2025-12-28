#include <bits/stdc++.h>

bool isPrime(int n) {
    std::vector<int> v;
    for(int i = 0; i*i <=n; i++){
        if(n%i == 0){
            v.push_back(i);
            // int temp = (int)n/i;
            // v.push_back(temp);
        }
    }
    for(auto it : v){
        std::cout << it  << " ";
    }
    return (v.size() == 2 ? true : false);
        
}

int main(){
    std::cout << isPrime(7) << std::endl;
    return 0;
}