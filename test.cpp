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



int main() {
    RB_tree<int> tree;
    set<int> s;
    
    const int MAX_KEY = 100;
    
    for (int i = 0; i < 20; ++i) {
        cout << "i = " << i << '\n';
        int x = rng() % MAX_KEY;
        cout << "x = " << x << '\n';
        tree.insert(x);
        s.insert(x);
        
        vector<int> in_order = tree.InOrder();
        cout << "InOrder: \t" << in_order << '\n';
        
        /*
        if (i > 10) {
            int y = rng() % MAX_KEY * 8 / 10;
            cout << "y = " << y << '\n';
            
            
            cout << "lower_bound     = \t" << tree.lower_bound(y) << '\n';
            cout << "set lower_bound = \t" << *s.lower_bound(y) << '\n';
            
            if (tree.lower_bound(y) != *s.lower_bound(y)) {
                cout << "??? lower_bound\n";
                return 0;
            }
        }
        */
        
        
        vector<int> next_order = tree.NextOrder();
        //cout << "NextOrder: \t" << next_order << '\n';
        
        vector<int> prev_order = tree.PrevOrder();
        reverse(all(prev_order));
        
        //cout << "Set<int>: \t" << s << '\n';
        
        if (next_order != in_order || vector<int>(s.begin(), s.end()) != next_order || next_order != prev_order) {
            cout << "????????????????\n";
            return 0;
        }
        
        tree.Check();
        
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
}
