#include <bits/stdc++.h>
using namespace std;

/*
 Comparator:
 - sort by second (ascending)
 - if second same → sort by first (descending)
*/
bool comp(const pair<int,int>& p1,
          const pair<int,int>& p2) {
    if (p1.second != p2.second)
        return p1.second < p2.second;
    return p1.first > p2.first;
}

int main() {

    /* ---------- VECTOR SORT ---------- */

    vector<int> v = {1, 6, 3, 9, 2};

    // descending sort
    sort(v.begin(), v.end(), greater<int>());

    /* ---------- ARRAY OF PAIRS SORT ---------- */

    pair<int,int> a[] = {{1,2}, {2,1}, {4,1}};
    int n = sizeof(a) / sizeof(a[0]);

    sort(a, a + n, comp);

    for (auto &p : a) {
        cout << p.first << " -> " << p.second << "\n";
    }

    for (int x : v) {
        cout << x << " ";
    }
    cout << "\n";

    /* ---------- BIT OPERATIONS ---------- */

    int num = 7;
    cout << "__builtin_popcount(7): "
         << __builtin_popcount(num) << "\n";

    long long num2 = 12312312345LL;
    cout << "__builtin_popcountll(num2): "
         << __builtin_popcountll(num2) << "\n";

    /* ---------- PERMUTATIONS ---------- */

    string s = "123";
    do {
        cout << s << "\n";
    } while (next_permutation(s.begin(), s.end()));

    /* ---------- MAX ELEMENT ---------- */

    int maxi = max({a[0].first, a[1].first, a[2].first});
    cout << "Max first element: " << maxi << "\n";

    return 0;
}
