// Online C++ compiler to run C++ program online
#include <iostream>

 int GCD(int n1,int n2) {
        int temp = n1;
      
        n1 = n2 % n1;
        if(n1 == 1 || n1 == 0){
            // std::cout << temp;
            if(n1 == 1){
                return 1;
            }
            return temp;
        }
        n2 = temp;
        return GCD(n1, n2);
        // std::cout <<  "It reaches here" << std::endl;
}

int main() {
    std::cout << GCD(12, 51) << std::endl;
    return 0;
}