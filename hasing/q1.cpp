// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>

using namespace std;

int count(vector<int>& nums, int num){
    int count = 0;
    for(auto it : nums){
        if(num == it){
            count++;
        }
    }
    return count;
} 

// vector<vector<int>>
vector<vector<int>> countFrequencies(vector<int>& nums) {
    vector<vector<int>> v;
    set<int> alreadythere;
    for (auto it : nums){
        if(alreadythere.find(it) != alreadythere.end()){
            continue;
        }
        alreadythere.insert(it);
        int numCount = count(nums, it);
        v.push_back({it, numCount});
    }
    return v;
}


int main() {
    // Write C++ code here
    vector<int> v  = {5, 5, 5, 5};
    vector<vector<int>> test = countFrequencies(v);
    for(auto it : test)
    cout << it[0] << " -> " << it[1] << endl;
    return 0;
}


// Input: nums = [1, 2, 2, 1, 3]
// Output: [[1, 2], [2, 2], [3, 1]]