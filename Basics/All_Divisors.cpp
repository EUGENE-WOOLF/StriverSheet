#include <bits/stdc++.h>

std::set<int> divisors(int n) {
    int temp = n;
    std::set<int> v;
    for (int i = 1; i * i <= n; i++) {
    if (n % i == 0) {
        v.insert(i);
        v.insert(n / i);
    }
}
    // v.push_back(temp);       
    return v;
} 
    
int main(){

    std::set<int> v = divisors(12);
    for(auto it : v) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
    return 0;
}