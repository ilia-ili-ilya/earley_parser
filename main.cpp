#include <iostream>
#include "tests.h"

using namespace std;

int main() {
    Grammar g;
    g.Init();
    int x;
    cin >> x;
    for (int i = 0; i < x; ++i) {
        string s;
        cin >> s;
        if (g.Earley(s)) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }
}

