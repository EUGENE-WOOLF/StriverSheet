#include<bits/stdc++.h>

 std::string removeNonAlnum(std::string s) {
        s.erase(
            std::remove_if(s.begin(), s.end(),
                [](unsigned char c) {
                    return !std::isalnum(c);
                }),
            s.end()
        );
        return s;
}

std::string formatString(std::string &s){
    s = removeNonAlnum(s);
    int len = s.length();
    for(int i = 0; i < len; i++){
    if(!std::islower(s[i])) {
        s[i] = std::tolower(s[i]);
        }
        
    }
    return s;
}

bool isPalindrome(std::string& s) {
    s = formatString(s);
    int i = 0, indexes = s.length() - 1;
    while(i < indexes){
        if(s[i] != s[indexes]){
            return false;
        }
        i++;
        indexes--;
    }
    return true;
}

int main(){
    std::string name = "hannah";
    std::cout << isPalindrome(name) << std::endl;
    return 0;
}