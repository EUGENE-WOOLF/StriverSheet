#include <bits/stdc++.h>
using namespace std;

void vectors();

int main() {
    // ===== Pair Basics =====
    pair<int, int> p = {1, 3};
    pair<int, pair<int, int>> pair2 = {1, {2, 3}};
    pair<int, int> arr[] = {{1, 2}, {2, 3}};

    // cout << p.first << " " << p.second << "\n";
    // cout << pair2.first << " "
    //      << pair2.second.first << " "
    //      << pair2.second.second << "\n";

    vectors();
    return 0;
}

void vectors() {

    // ===== Basic Vector Operations =====
    vector<int> v;
    v.push_back(1);
    v.emplace_back(2);

    // ===== Vector of Pairs =====
    vector<pair<int, int>> vecPair;
    vecPair.emplace_back(1, 2);
    vecPair.push_back({5, 6});

    // ===== Pre-sized Vector =====
    vector<int> v3(3, 100);   // {100, 100, 100}
    vector<int> v2(v3);       // Copy constructor

    // ===== Dynamic Insertions =====
    vector<int> v5;
    v5.emplace_back(6);
    v5.emplace_back(7);
    v5.emplace_back(8);

    // ===== Iterators =====
    vector<int>::iterator it = v5.begin();
    vector<int>::iterator it2 = v5.end();

    // cout << *(--it2) << "\n";     // last element
    // cout << *it << " " << *(it + 2) << "\n";

    // ===== Iteration Using Iterators =====
    for (vector<int>::iterator it = v5.begin(); it != v5.end(); it++) {
        // cout << *it << " ";
    }

    // ===== Auto Keyword =====
    auto s = "hello world";
    // cout << s << endl;

    // ===== Final Test Vector =====
    vector<int> final_test_vector = {
        920, 1111, 1112, 222, 200, 101
    };

    // Range-based for loop
    for (auto value : final_test_vector) {
        cout << value << " ";
    }
    cout << "\n";

    // ===== Erase =====
    auto final_iterator = final_test_vector.begin();
    final_test_vector.erase(final_iterator, final_iterator + 2);

    // ===== Insert =====
    final_test_vector.insert(final_test_vector.begin(), 920);
    final_test_vector.insert(final_test_vector.begin() + 1, 2, 20);

    vector<int> copy(2, 50);
    final_test_vector.insert(
        final_test_vector.begin(),
        copy.begin(),
        copy.end()
    );

    // ===== Pop Back =====
    final_test_vector.pop_back();

    // ===== Final Output =====
    for (auto value : final_test_vector) {
        cout << value << " ";
    }
    cout << "\n";
}
