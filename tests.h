#include "earley.h"
#include <random>
#include <cassert>

namespace SmallTests {
    void Test1() {
        Grammar g;
        g.InitForTests(1, 1, 1, {"A", "a", "A -> a", "A"});
        assert ((g.Earley("a") == true));
        assert((g.Earley("") == false));
        assert((g.Earley("aa") == false));
    }

    void Test2() {
        Grammar g;
        g.InitForTests(1, 1, 2, {"A", "a", "A -> a", "A -> AA", "A"});
        assert ((g.Earley("a") == true));
        assert((g.Earley("") == false));
        assert((g.Earley("aa") == true));
        string aaa;
        for (int i = 0; i < 100; ++i) aaa += 'a';
        assert((g.Earley(aaa) == true));
    }

    void Test3() {
        Grammar g;
        g.InitForTests(2, 1, 3, {"A", "ab", "A -> a", "A -> b", "A -> AA", "A"});
        assert ((g.Earley("a") == true));
        assert((g.Earley("b") == true));
        assert((g.Earley("ababa") == true));
        string aaa;
        for (int i = 0; i < 100; ++i) {
            if(i % 2 == 0) aaa += 'a';
            else aaa += 'b';
        }
        assert((g.Earley(aaa) == true));
    }

    void Test4() {
        Grammar g;
        g.InitForTests(2, 1, 3, {"A", "ab", "A -> ab", "A -> ba", "A -> AA", "A"});
        assert((g.Earley("a") == false));
        assert((g.Earley("abbaaabb") == false));
    }
}

namespace RandomTests {
    void Test1() {
        Grammar g;
        g.InitForTests(2, 1, 3, {"A", "ab", "A -> a", "A -> b", "A -> AA", "A"});
        for (int j = 0; j < 10; ++j) {
            string s;
            for (int i = 0; i < 100; ++i) {
                if (rand() % 2 == 0) s += 'a';
                else s += 'b';
            }
            assert((g.Earley(s) == true));
        }
    }
    void Test2() {
        Grammar g;
        g.InitForTests(2, 1, 3, {"A", "ab", "A -> ab", "A -> ba", "A -> AA", "A"});
        for (int j = 0; j < 10; ++j) {
            string s;
            for (int i = 0; i < 100; ++i) {
                if (rand() % 2 == 0) s += "ba";
                else s += "ab";
            }
            assert((g.Earley(s) == true));
        }
    }
}

bool AllTests() {
    SmallTests::Test1();
    SmallTests::Test2();
    SmallTests::Test3();
    SmallTests::Test4();
    RandomTests::Test1();
    RandomTests::Test2();

    return true;
}
