#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <map>

using namespace std;

const int64_t max_len_of_word = 10'000'005;

struct Symbol {
    char val;
    bool is_term;

    Symbol() = default;

    explicit Symbol(const char& val) : val(val) {
        is_term = (!((val >= 'A') && (val <= 'Z'))) && (val != '$');
    }
};


class Rule {
public:
    static inline int count_of_rules{};
    int index;
    Symbol lhs;
    vector<Symbol> rhs;

    Rule(char c, const string& s) : lhs(c) {
        for (char ss: s) rhs.emplace_back(ss);
        index = count_of_rules;
        ++count_of_rules;
    }

    Rule(const Rule&) = default;

    Rule(const string& s) {
        index = count_of_rules;
        ++count_of_rules;
        for (int i = 0; i < s.size() - 1; ++i) {
            if ((s[i] == '-') && (s[i + 1] == '>')) {
                string s1, s2;
                for (int j = 0; j < i; ++j) {
                    if (s[j] != ' ') {
                        s1 += s[j];
                    }
                }
                for (int j = i + 2; j < s.size(); ++j) {
                    if (s[j] != ' ') {
                        s2 += s[j];
                    }
                }
                if (s1.size() != 1) {
                    throw "incorrect rule";
                }
                lhs = Symbol(s1[0]);
                for (char c: s2) rhs.emplace_back(c);
                return;
            }
        }
    }

    void Print() {
        cout << lhs.val << "->";
        for (auto c: rhs) cout << c.val;
        cout << endl;
    }

    Rule() = default;
};


class Situation {
private:
    Rule *rule;
    int i;
    int j;
    int point_position;
public:
    Situation(Rule *rule, const int i, const int j) : rule(rule), i(i), j(j), point_position(0) {}

    Situation(Rule *rule, const int i, const int j, const int point_pos) : rule(rule), i(i), j(j),
                                                                           point_position(point_pos) {}


    Situation() : rule(nullptr), i(0), j(0), point_position(0) {}

    int64_t Index() const {
        return static_cast<int64_t>(rule->index) * max_len_of_word * max_len_of_word * max_len_of_word +
               static_cast<int64_t>(i) * max_len_of_word * max_len_of_word + static_cast<int64_t>(j) * max_len_of_word +
               point_position;
    }

    int I() const {
        return i;
    }

    int J() const {
        return j;
    }

    int PointPosition() const {
        return point_position;
    }

    Rule *GetRule() const {
        return rule;
    }

    int GetLen() const {
        return rule->rhs.size();
    }

    Symbol GetNonterm() {
        return rule->lhs;
    }

    bool operator==(const Situation& other) const {
        return (Index() == other.Index());
    }

    bool operator!=(const Situation& other) const {
        return (Index() != other.Index());
    }

    const Symbol& operator[](int ind) const {
        return rule->rhs[ind];
    }

    void Print() const {
        cout << "i: " << i << " j: " << j << " point: " << point_position << ' ';
        rule->Print();
    }
};


class SituationList {
    unordered_set<int64_t> indexes;
public:
    unordered_map<char, vector<Situation>> situations;

    bool Check(const Situation& situation) const {
        return indexes.contains(situation.Index());
    }

    void Add(const Situation& situation) {
        if (situation.PointPosition() == situation.GetLen()) {
            situations[0].push_back(situation);
        } else {
            char kek = situation[situation.PointPosition()].val;
            situations[kek].push_back(situation);
        }
        indexes.insert(situation.Index());
    }

    bool TryToAdd(const Situation& situation) {
        if (!Check(situation)) {
            Add(situation);
            return true;
        }
        return false;
    }

    bool Empty() const {
        return indexes.empty();
    }

    SituationList& operator=(const SituationList& other) = default;
};


class Grammar {
private:
    deque<Rule> rules;
    vector<vector<Rule *>> rules_for_every_non_term;
    Rule *first_rule;
    vector<Symbol> terminals;
    vector<Symbol> nonterminals;

public:
    Grammar() : rules(), rules_for_every_non_term('Z' - 'A' + 1), first_rule(nullptr), terminals(), nonterminals() {}

    void AddRule(const Rule& rule) {
        rules.push_back(rule);
        if (rule.lhs.val != '$')
            rules_for_every_non_term[rule.lhs.val - 'A'].push_back(&rules[rules.size() - 1]);
    };

    void Init() {
        Rule::count_of_rules = 0;
        int count_of_nonterm, count_of_term, count_of_rules;
        cin >> count_of_nonterm >> count_of_term >> count_of_rules;
        string terms, nonterms;
        cin >> nonterms;
        cin >> terms;
        for (int i = 0; i < count_of_term; ++i) {
            terminals.emplace_back(terms[i]);
        }
        for (int i = 0; i < count_of_nonterm; ++i) {
            nonterminals.emplace_back(nonterms[i]);
        }
        for (int i = 0; i < count_of_rules; ++i) {
            string new_rule;
            while (new_rule.empty()) getline(cin, new_rule);
            AddRule(Rule(new_rule));
        }
        string start;
        cin >> start;
        AddRule(Rule('$', start));
        first_rule = &rules[rules.size() - 1];
    }

    void InitForTests(int count_of_nonterm, int count_of_term, int count_of_rules, const vector<string>& imput) {
        Rule::count_of_rules = 0;
        string terms, nonterms;
        nonterms = imput[0];
        terms = imput[1];
        for (int i = 0; i < count_of_term; ++i) {
            terminals.emplace_back(terms[i]);
        }
        for (int i = 0; i < count_of_nonterm; ++i) {
            nonterminals.emplace_back(nonterms[i]);
        }
        for (int i = 0; i < count_of_rules; ++i) {
            string new_rule = imput[i+2];
            AddRule(Rule(new_rule));
        }
        string start = imput[count_of_rules+2];

        AddRule(Rule('$', start));
        first_rule = &rules[rules.size() - 1];
    }

    bool Complete(vector<SituationList>& actual_situations, int nom_of_symbol) {
        vector<Situation> to_add;
        for (auto situation: actual_situations[nom_of_symbol].situations[0]) {
            for (auto last_situation: actual_situations[situation.I()].situations[situation.GetNonterm().val]) {
                Situation potential_situation(last_situation.GetRule(), last_situation.I(), situation.J(),
                                              last_situation.PointPosition() + 1);
                if (!actual_situations[nom_of_symbol].Check(potential_situation)) {
                    to_add.push_back(potential_situation);
                }
            }
        }
        if (to_add.empty()) return false;
        for (auto sit_to_add: to_add) {
            actual_situations[nom_of_symbol].TryToAdd(sit_to_add);
        }
        return true;
    }

    bool Predict(SituationList& actual_situations) {
        vector<Situation> to_add;
        for (auto nonterm: nonterminals) {
            for (auto sit: actual_situations.situations[nonterm.val]) {
                for (auto rule: rules_for_every_non_term[nonterm.val - 'A']) {
                    if (!actual_situations.Check(Situation(rule, sit.J(), sit.J(), 0))) {
                        to_add.emplace_back(rule, sit.J(), sit.J(), 0);
                    }
                }
            }
        }
        if (to_add.empty()) return false;
        for (auto sit: to_add) actual_situations.TryToAdd(sit);
        return true;
    }

    SituationList Scan(SituationList& actual_situations, char c) {
        Symbol next_symbol(c);
        SituationList next_actual_situations;
        for (auto situation: actual_situations.situations[c]) {
            next_actual_situations.TryToAdd(
                Situation(situation.GetRule(), situation.I(), situation.J() + 1, situation.PointPosition() + 1));
        }
        return next_actual_situations;
    }

    bool Earley(const string& s) {
        int n = s.size();
        vector<SituationList> situations(n + 1);
        situations[0].Add(Situation(first_rule, 0, 0));
        while (Predict(situations[0]) || Complete(situations, 0)) {}
        for (int i = 0; i < n; ++i) {
            situations[i + 1] = Scan(situations[i], s[i]);
            if (situations[i + 1].Empty()) {
                return false;
            }
            while (Predict(situations[i + 1]) || Complete(situations, i + 1)) {}
        }
        return situations[n].Check(Situation(first_rule, 0, n, 1));
    }
};
