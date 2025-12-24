#include <iostream>

int main(){
    int n = 5;

    for (int i = 1; i <= 2 * n - 1; i++) {

        int stars = (i <= n) ? (2 * i - 1) : (2 * (2 * n - i) - 1);
        // int spaces = (i <= n) ? (n - i) : (i - n);

        // for (int s = 0; s < spaces; s++) {
        //     std::cout << " ";
        // }

        for (int j = 0; j < stars; j++) {
            std::cout << "*";
        }

        std::cout << "\n";
    }

    return 0;
}