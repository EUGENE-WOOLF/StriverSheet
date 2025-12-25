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
void explainPriorityQueue();
void explainSet();

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
    // explainQueue();
    // explainPriorityQueue();
    explainSet();

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


void explainPriorityQueue(){
    //this is known as the max head as the greatest value number stays on top 
    priority_queue<int> pq;
    pq.push(1);
    pq.push(5);
    pq.push(2);
    pq.push(7);

    cout << pq.top();
    pq.pop();
    cout << pq.top();


    //min heap
    priority_queue<int, vector<int>, greater<int>> pq2;
    pq2.push(5);
    pq2.push(1);
    pq2.push(2);

    cout << pq2.top();
    
    //push , pop- O(logn) top - O(1)
}

void explainSet() {
    // -------------------------------
    // Basic properties of set
    // - sorted
    // - unique
    // - O(log n) operations
    // -------------------------------

    set<int> st;

    // insert / emplace
    st.insert(1);
    st.emplace(2);
    st.insert(2);   // duplicate, ignored
    st.insert(5);
    st.insert(920);

    // traversal (always sorted)
    cout << "Set elements: ";
    for (int x : st) {
        cout << x << " ";
    }
    cout << endl;

    // -------------------------------
    // find()
    // -------------------------------
    auto it = st.find(2);
    if (it != st.end()) {
        cout << "Found 2: " << *it << endl;
    }

    // -------------------------------
    // element not found
    // -------------------------------
    auto it2 = st.find(7);
    if (it2 == st.end()) {
        cout << "7 not found" << endl;

        // safely move to previous element
        if (!st.empty()) {
            auto prevIt = st.end();
            --prevIt;
            cout << "Largest element < 7: " << *prevIt << endl;
        }
    }

    // -------------------------------
    // erase
    // -------------------------------
    st.erase(5);   // erase by value

    // -------------------------------
    // count
    // -------------------------------
    cout << "Count of 1: " << st.count(1) << endl; // 0 or 1

    // erase by iterator (safe example)
    auto itErase = st.find(2);
    if (itErase != st.end()) {
        st.erase(itErase);
    }

    cout << "After erase: ";
    for (int x : st) {
        cout << x << " ";
    }
    cout << endl;

    // -------------------------------
    // lower_bound & upper_bound
    // -------------------------------
    set<int> s;
    s.emplace(1);
    s.emplace(2);
    s.emplace(4);
    s.emplace(9);
    s.emplace(27);

    // lower_bound: first >= x
    auto lb = s.lower_bound(5);
    if (lb != s.end()) {
        cout << "lower_bound(5): " << *lb << endl;
    }

    // upper_bound: first > x
    auto ub = s.upper_bound(9);
    if (ub != s.end()) {
        cout << "upper_bound(9): " << *ub << endl;
    }
}