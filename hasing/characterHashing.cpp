#include<bits/stdc++.h>

using namespace std;

int characterFrequencies(string s, char c );

int main(){
    string s;
    cin >> s;
    int sizeOfLetters = 26;
    int arr[sizeOfLetters] = {0};
    for(int i = 0; i < s.length(); i++){
        arr[s[i] - 'a'] = characterFrequencies(s ,s[i]);
    }
    for(auto it:arr){
        cout << it << endl;
    }
}

int characterFrequencies(string s, char c ){
    int count = 0;
    for(int i=0; i < s.length(); i++){
        if(c == s[i]){
            count++;
        }
    }
    return count;
}