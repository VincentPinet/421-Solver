#include <array>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <print>
#include <random>
#include <unordered_map>

using namespace std;

static constexpr int N_ROLL = 3;
static constexpr int N_DICE = 3;

static constexpr array ranking = {
    421, 111, 611, 666, 511, 555, 411, 444, 311, 333, 211, 222, 654, 543,
    432, 321, 665, 664, 663, 662, 661, 655, 653, 652, 651, 644, 643, 642,
    641, 633, 632, 631, 622, 621, 554, 553, 552, 551, 544, 542, 541, 533,
    532, 531, 522, 521, 443, 442, 441, 433, 431, 422, 332, 331, 322, 221
};

static constexpr array ipow10 = {1, 10, 100};

struct entry {
    double val;
    int action = 0;
};

static unordered_map<int, unordered_map<int, entry>> f{};
static unordered_map<int, unordered_map<int, unordered_map<int, entry>>> g{};

int canonical(int r) {
    int res = 0;
    for (int k = 6; k >= 1; k--) {
        for (int tmp = r; tmp > 0; tmp /= 10) {
            if (tmp % 10 == k) {
                res *= 10;
                res += tmp % 10;
            }
        }
    }
    return res;
}

int replace(int r, int i, int d) {
    return r - (r / ipow10[i] % 10) * ipow10[i] + d * ipow10[i];
}

list<int> outcomes(int r = 0, int a = 0b111) {
    // if (r == 221) return {}; // rule variant
    list<int> res = {r};
    for (int i = 0; i < N_DICE; i++) {
        if ((a >> i) & 1) {
            size_t remaining = res.size();
            while (remaining--) {
                int curr = res.front();
                res.pop_front();
                for (int d = 1; d <= 6; d++)
                    res.push_back(replace(curr, i, d));
            }
        }
    }
    transform(res.begin(), res.end(), res.begin(), canonical);
    return res;
}

double eval(int a, int b) {
    if (a == 221) return 0;
    if (a == b) return 0.5;
    for (int r : ranking)
        if (a == r)
            return 1;
        else if (b == r)
            return 0;
    return -1;
}

double ev(const list<int>& X, function<double(double, int)> func) {
    return accumulate(X.begin(), X.end(), 0.0, func) / X.size();
}

void update(entry& min, const entry& o, function<bool(double, double)> cmp) {
    if (cmp(o.val, min.val)) {
        min.val = o.val;
        min.action = o.action;
    }
}

void compute_f() {
    for (int r : ranking)
        f[r][N_ROLL - 1].val = ev(outcomes(), [r](double sum, int xi) { return sum + g[r][xi][N_ROLL - 1].val; });

    for (int t = 1; t >= 0; t--) {
        for (int r : ranking) {
            entry max{ev(outcomes(), [r, t](double sum, int xi) { return sum + g[r][xi][t].val; })};
            for (int a = 0b001; a <= 0b111; a++)
                update(max, {ev(outcomes(r, a), [t](double sum, int xi) { return sum + f[xi][t + 1].val; }), a}, greater<double>());
            f[r][t] = max;
        }
    }
}

void compute_g() {
    for (int r1 : ranking)
        for (int r2 : ranking)
            g[r1][r2][0].val = eval(r1, r2);

    for (int t = 1; t < N_ROLL; t++) {
        for (int r1 : ranking) {
            for (int r2 : ranking) {
                entry min{eval(r1, r2)};
                for (int a = 0b001; a <= 0b111; a++)
                    update(min, {ev(outcomes(r2, a), [r1, t](double sum, int xi) { return sum + g[r1][xi][t - 1].val; }), a}, less<double>());
                g[r1][r2][t] = min;
            }
        }
    }
}

static random_device rd{};
static mt19937 roll{rd()};
static uniform_int_distribution<int> die{1, 6};

int apply_action(int r, int a) {
    for (int i = 0; i < N_DICE; i++)
        if ((a >> i) & 1)
            r = replace(r, i, die(roll));
    return canonical(r);
}

double sim() {
    int t = 0;

    int r = canonical(die(roll) * 100 + die(roll) * 10 + die(roll));
    for (; f[r][t].action; t++)
        r = apply_action(r, f[r][t].action);

    int s = canonical(die(roll) * 100 + die(roll) * 10 + die(roll));
    for (; g[r][s][t].action; t--)
        s = apply_action(s, g[r][s][t].action);

    return eval(r, s);
}

string to_string(int r, int a) {
    string res{};
    for (int i = N_DICE - 1; i >= 0; i--)
        if (!((a >> i) & 1))
            res += '0' + r / ipow10[i] % 10;
    return res;
}

void print_ans(unordered_map<int, unordered_map<int, entry>>& ans) {
    std::print("{:^{}} |", "r\\t", N_DICE);
    for (int i = 0; i < N_ROLL; i++)
        std::print("{:^14}|", i);
    cout << endl;
    for (int r2 : ranking) {
        cout << r2 << " |";
        for (int t = 0; t < N_ROLL; t++) {
            std::print("{:6.2f}% ({:>3}) |", ans[r2][t].val * 100, to_string(r2, ans[r2][t].action));
        }
        cout << endl;
    }
}

int main() {
    compute_g();
    compute_f();
    print_ans(f);
    cout << ev(outcomes(), [](double sum, int xi) { return sum + f[xi][0].val; }) << endl;
    // for (int r1 : ranking)
    //     print_ans(g[r1]);
    return 0;
}
