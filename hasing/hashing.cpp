#include <bits/stdc++.h>

using namespace std;

int frequenciesMapper(int num, vector<int> &arr);

int main(){
    vector<int> v = {5, 3, 5, 2, 8, 3, 3, 5, 1, 8};
    map<int, int> map2;
    for(int vals : v){
        map2[vals] = frequenciesMapper(vals , v);
    }
    
    for(auto it: map2){
        cout << it.first << " " << it.second << endl;
    }
}

int frequenciesMapper(int num, vector<int> &arr){
    int count = 0;
    for (int val : arr){
        if(val == num){
            count++;
        }
    }
    return count;
}