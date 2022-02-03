#include <bits/stdc++.h>
#include "test_set.h"

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
template<typename T>
std::ostream& operator << (std::ostream& os, const TestSet<T>& a) {
    for (const T& x : a) {
        os << x << ' ';
    }
    return os;
}



TestSet<int> f() {
    TestSet<int> t({6, 8, 9});
    return t;
}

int main() {
    const int MAX_KEY = 100;


    int n = 3;
    vector<TestSet<int>> t(n);


    for (int test = 0; test < 300; ++test) {
        cout << "\n\ntest = " << test << "\n\n";

        int type = rng() % 20;
        cout << "type = " << type << "\n";

        if (type == 0) {
            int i = rng() % n;
            int j = rng() % n;
            cout << "i, j = " << i << ", " << j << "\n";
            swap(t[i], t[j]);
        } else {
            int i = rng() % n;
            if (1 <= type && type <= 4) {
                int x = rng() % MAX_KEY;
                if (type == 1) {
                    cout << "insert ";
                    t[i].insert(x);
                }
                if (type == 2) {
                    cout << "erase ";
                    t[i].erase(x);
                }
                if (type == 3) {
                    cout << "find ";
                    t[i].find(x);
                }
                if (type == 4) {
                    cout << "lower_bound ";
                    t[i].lower_bound(x);
                }
                cout << "i, x = " << i << ", " << x << "\n";
            }
            if (type >= 5) {
                vector<int> v;
                int szv = 1 + rng() % 20;
                for (int i = 0; i < szv; ++i) {
                    v.push_back(rng() % MAX_KEY);
                }
                if (rng() % 2) {
                    sort(v.begin(), v.end());
                }
                cout << "construct " << v << "\n";
                t[i] = TestSet<int>(v.begin(), v.end());
            }
        }


        cout << "\n";
        for (int i = 0; i < n; ++i) {
            cout << "t[" << i << "] = " << t[i] << "\n";
        }
        cout << "\n";

    }




    /*
    vector<int> v = {1, 2, 3, 4, 5};

    //const RB_tree<int> t(v.begin(), v.end());
    TestSet<int> t({1, 2, 3, 4, 5});

    cout << t << "\n";

    TestSet<int> t2 = f();
    t2.insert(10);

    cout << t << "\n";
    cout << t2 << "\n";

    int step = 0;
    for (auto it = --t.end(); step < 20; it--, ++step) {
        cout << *it << " ";
    }
    cout << "\n";


    t2 = t2;
    */

    return 0;
}
