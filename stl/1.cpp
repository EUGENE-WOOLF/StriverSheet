#include <bits/stdc++.h>
using namespace std;

/*
========================================
 STL QUICK REVISION
----------------------------------------
 Containers:
   - vector  : dynamic array (random access)
   - list    : doubly linked list
   - deque   : double-ended queue

 Adapters:
   - stack   : LIFO (top only)
   - queue   : FIFO (front remove, back insert)

 Key Rule:
   Adapters RESTRICT access on purpose.
========================================
*/

// Function declarations
void explainVector();
void explainList();
void explainDeque();
void explainStack();
void explainQueue();

int main() {

    // ===== pair (basic utility) =====
    pair<int, int> p = {1, 3};
    pair<int, pair<int, int>> nestedPair = {1, {2, 3}};
    pair<int, int> arr[] = {{1, 2}, {2, 3}};

    // Uncomment to test
    // explainVector();
    // explainList();
    // explainDeque();
    // explainStack();
    explainQueue();

    return 0;
}


void explainVector() {

    /*
      VECTOR:
      - Contiguous memory
      - Random access O(1)
      - push_back may reallocate
    */

    vector<int> v;
    v.push_back(1);
    v.emplace_back(2);

    // Vector of pairs
    vector<pair<int, int>> vecPair;
    vecPair.emplace_back(1, 2);
    vecPair.push_back({5, 6});

    // Pre-sized vector
    vector<int> v3(3, 100);   // {100, 100, 100}
    vector<int> v2(v3);       // copy

    // Dynamic insertions
    vector<int> v5 = {6, 7, 8};

    // Iterators
    auto it = v5.begin();
    auto itEnd = v5.end();

    // Final test vector
    vector<int> test = {920, 1111, 1112, 222, 200, 101};

    for (int x : test) {
        cout << x << " ";
    }
    cout << "\n";

    // Erase first two elements
    test.erase(test.begin(), test.begin() + 2);

    // Insert examples
    test.insert(test.begin(), 920);
    test.insert(test.begin() + 1, 2, 20);

    vector<int> copy(2, 50);
    test.insert(test.begin(), copy.begin(), copy.end());

    test.pop_back();

    for (int x : test) {
        cout << x << " ";
    }
    cout << "\n";
}


void explainList() {

    /*
      LIST:
      - Non-contiguous memory
      - O(1) insert/delete (iterator known)
      - No random access
    */

    list<int> ls;

    ls.push_back(2);
    ls.emplace_back(3);
    ls.push_front(1);
    ls.emplace_front(0);

    for (int x : ls) {
        cout << x << " ";
    }
    cout << "\n";
}


void explainDeque() {

    /*
      DEQUE:
      - Fast push/pop at both ends
      - Random access allowed
      - Default backend for stack/queue
    */

    deque<int> dq;

    dq.push_back(1);
    dq.emplace_back(2);
    dq.push_front(0);
    dq.emplace_front(-1);

    dq.pop_back();
    dq.pop_front();

    for (int x : dq) {
        cout << x << " ";
    }
    cout << "\n";
}


void explainStack() {

    /*
      STACK:
      - LIFO
      - Only access: top
      - No iteration
    */

    stack<int> st;

    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);

    cout << "Top: " << st.top() << "\n";

    st.pop();

    cout << "New Top: " << st.top() << "\n";
    cout << "Size: " << st.size() << "\n";

    // Swap stacks
    stack<int> st2;
    st2.swap(st);
}


void explainQueue() {

    /*
      QUEUE:
      - FIFO
      - Insert at back
      - Remove from front
      - Can READ both ends
    */

    queue<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    cout << "Back (last inserted): " << q.back() << "\n";

    // back() returns reference
    q.back() += 5;
    cout << "Modified Back: " << q.back() << "\n";

    cout << "Front (first inserted): " << q.front() << "\n";

    q.pop();  // removes front

    cout << "New Front: " << q.front() << "\n";
}


