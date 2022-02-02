#include <bits/stdc++.h>
#include "rb_tree.h"

using namespace std;

#define all(v) (v).begin(), (v).end()
#define sz(v) (int)(v).size()

typedef long long ll;
typedef pair<int, int> pii;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<typename T>
std::ostream& operator << (std::ostream& os, const vector<T>& a) {
    for (const T& x : a) {
        os << x << ' ';
    }
    return os;
}
template<typename T>
std::ostream& operator << (std::ostream& os, const set<T>& a) {
    for (const T& x : a) {
        os << x << ' ';
    }
    return os;
}
template<typename T>
std::ostream& operator << (std::ostream& os, const Set<T>& a) {
    for (const T& x : a) {
        os << x << ' ';
    }
    return os;
}


Set<int> f() {
    Set<int> t({6, 8, 9});
    return t;
}

int main() {
    /*
    RB_tree<int> tree;
    set<int> s;

    const int MAX_KEY = 1000;

    for (int i = 0; i < MAX_KEY / 2; ++i) {
        cout << "i = " << i << '\n';
        int x = rng() % MAX_KEY;
        cout << "x = " << x << '\n';
        tree.insert(x);
        s.insert(x);

        vector<int> in_order = tree.InOrder();
        cout << "InOrder: \t" << in_order << '\n';

        if (i > 10) {
            int y = rng() % MAX_KEY;
            cout << "y = " << y << '\n';

            if (tree.find(y) != (s.find(y) != s.end())) {
                cout << "??? find\n";
                return 0;
            }
        }

        cout << "\n";
    }

    cout << "\n\nerase:\n\n";

    while (!s.empty()) {
        vector<int> elems = tree.NextOrder();
        if (sz(elems) > 0) {
            int x = elems[rng() % sz(elems)];
            tree.erase(x);
            s.erase(x);

            cout << "x = " << x << '\n';
        }
        cout << "tree = \t" << tree.NextOrder() << '\n';
        cout << "set = \t" << s << '\n';
        if (vector<int>(s.begin(), s.end()) != tree.NextOrder() || tree.size() != s.size()) {
            cout << "????????????????\n";
            return 0;
        }

        tree.Check();
    }
    */



    vector<int> v = {1, 2, 3, 4, 5};

    //const RB_tree<int> t(v.begin(), v.end());
    const Set<int> t({1, 2, 3, 4, 5});

    cout << t << "\n";

    Set<int> t2 = f();
    t2.insert(10);

    cout << t << "\n";
    cout << t2 << "\n";

    /*
    int step = 0;
    for (auto it = --t.end(); step < 100; --it, ++step) {
        cout << *it << "\n";
    }
    */

    return 0;
}
