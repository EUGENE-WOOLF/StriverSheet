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
void explainMultiSet();
void explainMap();

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
    // explainSet();
    // explainMultiSet();
    explainMap();

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


/*
====================================================
                MULTISET NOTES
====================================================

- Ordered associative container
- Implemented using Red-Black Tree (balanced BST)
- Stores elements in sorted order
- Allows DUPLICATES
- Iterators are BIDIRECTIONAL (++, -- only)
- No random access (NO it + 1)

Time Complexity:
- insert      -> O(log n)
- find        -> O(log n)
- erase       -> O(log n)
- count       -> O(log n + occurrences)

IMPORTANT:
- erase(value)  -> removes ALL occurrences
- erase(it)     -> removes ONE element
*/

void explainMultiSet() {
    multiset<int> ms;

    ms.insert(1);
    ms.insert(1);
    ms.insert(3);
    ms.insert(4);

    // Count occurrences
    cout << "Count of 1: " << ms.count(1) << endl;

    // Find ONE occurrence
    auto it = ms.find(1);

    // Erase only ONE occurrence (correct)
    if (it != ms.end()) {
        ms.erase(it);
    }

    // Traversal (always sorted)
    cout << "Multiset elements: ";
    for (auto x : ms) {
        cout << x << " ";
    }
    cout << endl;

    // Range erase (erase ONE element safely)
    auto it2 = ms.find(1);
    if (it2 != ms.end()) {
        //so if it doesn't exist then it has to do that 
        ms.erase(it2, next(it2));
    }
}

/*
====================================================
              UNORDERED SET NOTES
====================================================

- Unordered associative container
- Implemented using HASH TABLE
- Stores UNIQUE elements only
- Order is NOT guaranteed
- Iterators are FORWARD only

Time Complexity (Average Case):
- insert      -> O(1)
- find        -> O(1)
- erase       -> O(1)

Worst Case (hash collision):
- O(n)

IMPORTANT:
- Faster than set/multiset
- No ordering
- No duplicates
*/

void explainUnorderedSet() {
    unordered_set<int> us;

    us.insert(1);
    us.insert(2);
    us.insert(3);
    us.insert(3); // duplicate ignored

    // Find
    if (us.find(2) != us.end()) {
        cout << "2 is present" << endl;
    }

    // Erase
    us.erase(2);

    // Traversal (random order)
    cout << "Unordered set elements: ";
    for (auto x : us) {
        cout << x << " ";
    }
    cout << endl;
}

/*
====================================================
            QUICK COMPARISON
====================================================

multiset:
- ordered
- duplicates allowed
- O(log n)
- tree based

unordered_set:
- unordered
- unique only
- O(1) average
- hash based
*/




/*
====================================================
                    MAP
====================================================

- Stores UNIQUE keys in SORTED order
- Implemented using Red-Black Tree (balanced BST)
- Keys are ordered using < operator
- Duplicate keys are NOT allowed

Time Complexity:
- insert       -> O(log n)
- find         -> O(log n)
- erase        -> O(log n)
- lower_bound  -> O(log n)
- upper_bound  -> O(log n)

Iterator points to:
pair<const Key, Value>
*/

void explainMap() {

    map<int, int> map1;                       // key -> value
    map<pair<int, int>, int> map2;            // pair key
    map<int, pair<int, int>> map3;            // value is a pair

    map2[{1,2}] = 10;

    map1[1] = 2;              // inserts if key not present
    map1.emplace(2, 4);       // preferred (no extra copy)
    map1.insert({3, 4});      // insert pair

    cout << map1[1] << endl;

    // find()
    auto it = map1.find(3);
    if (it != map1.end()) {
        cout << it->second << endl;
    }

    // bounds (only valid for ordered containers)
    auto it1 = map1.lower_bound(2); // first key >= 2
    auto it2 = map1.upper_bound(2); // first key > 2

    // traversal (sorted order)
    // for (auto it : map1) {
    //     cout << it.first << " " << it.second << endl;
    // }
}

/*
====================================================
                UNORDERED_MAP
====================================================

- Stores UNIQUE keys
- Order is NOT guaranteed
- Implemented using HASH TABLE
- Faster than map in average case

Time Complexity (average):
- insert  -> O(1)
- find    -> O(1)
- erase   -> O(1)

Worst case:
- O(n) (hash collisions)

IMPORTANT:
- No lower_bound / upper_bound
- No ordering
*/

void explainUnorderedMap() {

    unordered_map<int, int> ump;

    ump[1] = 10;
    ump[2] = 20;
    ump.emplace(3, 30);

    if (ump.find(2) != ump.end()) {
        cout << ump[2] << endl;
    }

    // traversal (random order)
    // for (auto it : ump) {
    //     cout << it.first << " " << it.second << endl;
    // }
}

/*
====================================================
                    MULTIMAP
====================================================

- Allows DUPLICATE keys
- Keys are SORTED
- Implemented using Red-Black Tree

Time Complexity:
- insert  -> O(log n)
- find    -> O(log n)
- erase   -> O(log n)

IMPORTANT:
- operator[] is NOT available
- find() returns ONE occurrence
- use equal_range() for all duplicates
*/

void explainMultiMap() {

    multimap<int, int> mm;

    mm.insert({1, 10});
    mm.insert({1, 20});
    mm.insert({2, 30});

    // find() -> one occurrence
    auto it = mm.find(1);
    if (it != mm.end()) {
        cout << it->first << " " << it->second << endl;
    }

    // get all values of a key
    auto range = mm.equal_range(1);
    for (auto it = range.first; it != range.second; it++) {
        cout << it->second << " ";
    }
    cout << endl;
}

/*
====================================================
                QUICK SUMMARY
====================================================

map:
- unique keys
- sorted
- O(log n)

unordered_map:
- unique keys
- not sorted
- O(1) average

multimap:
- duplicate keys allowed
- sorted
- O(log n)
*/



