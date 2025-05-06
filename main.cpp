#include <compare>
#include <tuple>

struct entry {
    int action{0};
    double val;

    constexpr auto operator<=>(const entry& rhs) const { return val <=> rhs.val; }
};

namespace std {
    template<>
    struct tuple_size<entry> : tuple_size<pair<int, double>> {};

    template<size_t I>
    struct tuple_element<I, entry> : tuple_element<I, pair<int, double>> {};

    // P2769R3 waiting room, big hacks in the meantime
    // I feel entitled to be able to use struct entry with tuple-like based ranges
    // and I am not sacrificing .action/.val naming for .first/.second or get<0>/get<1>
    template<>
    constexpr bool __is_tuple_like_v<entry> = true;

    template<size_t I>
    constexpr auto get(const entry& e) {
        static_assert(I < 2);
        if constexpr (I == 0) return e.action;
        if constexpr (I == 1) return e.val;
    }
}
#include <ranges> // don't move this :')


#include <algorithm>
#include <array>
#include <numeric>
#include <print>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

static constexpr int max_reroll_limit = 3;
static constexpr int max_dice_count = 3;

static constexpr std::array rankings = {
    421, 111, 611, 666, 511, 555, 411, 444, 311, 333, 211, 222, 654, 543,
    432, 321, 665, 664, 663, 662, 661, 655, 653, 652, 651, 644, 643, 642,
    641, 633, 632, 631, 622, 621, 554, 553, 552, 551, 544, 542, 541, 533,
    532, 531, 522, 521, 443, 442, 441, 433, 431, 422, 332, 331, 322, 221
};

template<typename T>
using umap = std::unordered_map<int, T>;

static umap<umap<entry>> f{};
static umap<umap<umap<entry>>> g{};

static constexpr std::array ipow10  = {1, 10, 100};
static constexpr auto max_precision = std::numeric_limits<double>::digits10 + 1;


template<std::ranges::sized_range Range>
static constexpr auto ev(Range rg) {
    return std::ranges::fold_left(rg, 0.0, std::plus{}) / static_cast<double>(std::ranges::size(rg));
}

static constexpr auto canonical(int r) {
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

static constexpr auto replace(int r, int i, int d) {
    return r - (r / ipow10[i] % 10) * ipow10[i] + d * ipow10[i];
}

static constexpr auto eval(int a, int b) {
    if (a == 221) return 0.0;
    if (a == b)   return 0.5;
    for (int r : rankings) {
        if (a == r) return 1.0;
        if (b == r) return 0.0;
    }
    std::unreachable();
}

static constexpr auto outcomes(int r = 0, int a = 0b111) {
    std::vector<int> res = {r};
    for (int i = 0; i < max_dice_count; i++) {
        if (a >> i & 1) {
            auto remaining = res.size();
            while (remaining--) {
                const auto curr = res.front();
                res.erase(res.begin());
                for (int d = 1; d <= 6; d++)
                    res.push_back(replace(curr, i, d));
            }
        }
    }
    std::ranges::transform(res, res.begin(), canonical);
    return res;
}

static constexpr auto has_reroll_left(int t) {
    return t > 0;
}

static constexpr auto can_reroll(int t) {
    return t < max_reroll_limit;
}

static auto die_roll() {
    static std::mt19937 prng{std::random_device{}()};
    static std::uniform_int_distribution<int> dice_dist{1, 6};
    return dice_dist(prng);
}

static auto openning_throw() {
    return canonical(die_roll() * 100 + die_roll() * 10 + die_roll());
}

static auto apply_action(int r, int a) {
    for (int i = 0; i < max_dice_count; i++)
        if (a >> i & 1)
            r = replace(r, i, die_roll());
    return canonical(r);
}

[[maybe_unused]]
static auto sim() {
    int r = openning_throw();
    int s = openning_throw();
    int t = 0;
    while (f[r][t].action)    r = apply_action(r, f[r][t++].action);
    while (g[r][s][t].action) s = apply_action(s, g[r][s][t--].action);
    return eval(r, s);
}

static auto action_to_string(int r, int a) {
    std::string res{};
    for (int i = max_dice_count - 1; i >= 0; i--)
        if (not (a >> i & 1))
            res += std::to_string(r / ipow10[i] % 10);
    return res;
}

void print_ans(const umap<umap<entry>>& ans) {
    std::print("{:^{}} |", "r\\t", max_dice_count);
    for (int i = 0; i < max_reroll_limit; i++)
        std::print("{:^14}|", i);

    std::println();

    for (int r : rankings) {
        std::print("{} |", r);
        for (int t = 0; t < max_reroll_limit; t++) {
            const auto& curr = ans.at(r).at(t);
            std::print("{:6.2f}% ({:>3}) |", curr.val * 100
                                           , action_to_string(r, curr.action));
        }
        std::println();
    }
    std::println();
}


void solve() {
    // in a typical dynamic programming fashion
    // starting from the end (i.e., g[r1][r2][0])
    // meaning second player with no reroll left
    // and minimaxing our way up to the first state

    for (int t = 0; t < max_reroll_limit; t++) {
        for (int r1 : rankings) {
            for (int r2 : rankings) {
                auto& curr = g[r1][r2][t];
                curr = {.val = eval(r1, r2)};
                for (int a = 0b001; a <= 0b111 and has_reroll_left(t); a++) {
                    const auto next = [r1, t](int xi) { return g[r1][xi][t - 1]; };
                    const auto v = ev(outcomes(r2, a) | std::views::transform(next) | std::views::values);
                    curr = std::min(curr, {a, v});
                }
            }
        }
    }

    for (int t = max_reroll_limit - 1; t >= 0; t--) {
        for (int r : rankings) {
            auto& curr = f[r][t];
            const auto stand = [r, t](int xi) { return g[r][xi][t]; };
            curr = {.val = ev(outcomes() | std::views::transform(stand) | std::views::values)};
            for (int a = 0b001; a <= 0b111 and can_reroll(t); a++) {
                const auto next = [t](int xi) { return f[xi][t + 1]; };
                const auto v = ev(outcomes(r, a) | std::views::transform(next) | std::views::values);
                curr = std::max(curr, {a, v});
            }
        }
    }
}


int main() {

    solve();
    print_ans(f);

    // // print second player strategy
    // std::ranges::for_each(rankings, [](auto r) { std::println("{}", r); print_ans(g[r]); });

    const auto initial_state = [](int r) { return f[r][0]; };
    const auto ret = ev(outcomes() | std::views::transform(initial_state) | std::views::values);
    std::println("{:.{}}", ret, max_precision);

    // // run simulations
    // double sum = 0;
    // std::ranges::for_each(std::views::iota(0, 1000000), [&sum](auto) { sum += sim(); });
    // std::println("{:.{}}", sum / 1000000, max_precision);

    return 0;
}
