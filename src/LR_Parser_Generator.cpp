#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;


struct Rule {
    string l;
    vector<string> r;

    bool operator == (Rule& a) {
        if (l != a.l || r.size() != a.r.size())
            return false;

        int n = r.size();

        for (int i = 0; i < n; i++) {
            if (r[i] != a.r[i])
                return false;
        }

        return true;
    }

    bool operator != (Rule& a) {
        return !(*this == a);
    }

    bool operator < (Rule& a) {
        if (l < a.l)
            return true;

        int n = min(r.size(), a.r.size());

        for (int i = 0; i < n; i++) {
            if (r[i] < a.r[i])
                return true;
        }

        return r.size() < a.r.size();
    }
};

typedef vector<Rule> RuleTable;

bool RuleTableCmp(vector<Rule>& a, vector<Rule>& b) {
    if(a.size() != b.size())
        return false;

    int n = a.size();

    for (int i = 0; i < n; i++) {
        if(a[i] != b[i])
            return false;
    }

    return true;
}

void readRuleTable(const char* file, RuleTable& ruleTable) {
    FILE* fp = fopen(file, "r");
    Rule rule;
    char *line, * record;
    char buffer[1024];

    ruleTable.clear();

    while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL) {
        line = strtok(line, "\n");
        rule.l = record = strtok(line, " ");

        while ((record = strtok(NULL, " ")) != NULL)
            rule.r.push_back(record);
        
        ruleTable.push_back(rule);
        rule.r.clear();
    }

}

void getSymbol(RuleTable& ruleTable, vector<string>& symbol, vector<string>& terminal, vector<string>& nonterminal) {
    symbol.clear();
    terminal.clear();
    nonterminal.clear();
    
    for (auto a : ruleTable) {
        if (find(terminal.begin(), terminal.end(), a.l) == terminal.end())
            terminal.push_back(a.l);

        for (auto b : a.r) {
            if (find(symbol.begin(), symbol.end(), b) == symbol.end())
                symbol.push_back(b);
        }
    }

    for (auto a : symbol) {
        if (find(terminal.begin(), terminal.end(), a) == terminal.end())
            nonterminal.push_back(a);
    }
}

void getStates(RuleTable& ruleTable, vector<string>& symbol, vector<RuleTable>& states, vector<vector<int>>& dots) {
    states.clear();
    dots.clear();
    vector<int> dot;
    vector<Rule> table;

    for (int i = 0; i < ruleTable.size(); i++)
        dot.push_back(0);

    table = ruleTable;
    sort(table.begin(), table.end());
    states.push_back(ruleTable);
    dots.push_back(dot);

    int cur = -1;

    while ((++cur) < states.size()) {

        for (auto s : symbol) {
            table.clear();
            dot.clear();

            int i = 0;

            for (auto a : states[cur]) {
                if (dots[cur][i] < a.r.size() && a.r[dots[cur][i]] == s) {
                    table.push_back(a);
                    dot.push_back(dots[cur][i] + 1);

                    for (auto b : ruleTable) {
                        if (dots[cur][i] + 1 < a.r.size() &&
                            a.r[dots[cur][i] + 1] == b.l) {

                            bool fg = 1;
                            for (auto c : table) {
                                if (c == b) {
                                    fg = 0;
                                    break;
                                }
                            }
                            if (fg) {
                                table.push_back(b);
                                dot.push_back(0);
                            }
                        }
                    }
                }
                i++;
            }

            if (table.size() > 0) { 
                sort(table.begin(), table.end());

                bool fg = 1;

                for (auto a : states) {
                    if (RuleTableCmp(a, table)) {
                        fg = 0;
                        break;
                    }
                }

                if (fg) {
                    states.push_back(table);
                    dots.push_back(dot);
                }
            }

        }
    }
}

int main(int argc, char* argv[]) {

    RuleTable ruleTable;
    readRuleTable(argv[1], ruleTable);

    vector<string> symbol, terminal, nonterminal;
    getSymbol(ruleTable, symbol, terminal, nonterminal);

    vector<RuleTable> states;
    vector<vector<int>> dots;
    getStates(ruleTable, symbol, states, dots);

    printf("%d \n", dots.size());

    for (int i = 0; i < states.size(); i++) {
        printf("----------- : %d\n", i);
        for (int j = 0; j < states[i].size(); j++) {
            printf("%d : ", dots[i][j]);
            printf("%s -> ", states[i][j].l);
            for (auto c : states[i][j].r) {
                printf("%s ", c);
            }
            printf("\n");
        }
    }
}

