#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <unordered_map>

#define N_ROLL 3
#define N_DICE 3

using namespace std;

typedef struct {
	float val;
	int action = 0;
} entry;

unordered_map<int, unordered_map<int, entry>> f;
unordered_map<int, unordered_map<int, unordered_map<int, entry>>> g;

vector<int> ranking = {
	421,111,611,666,511,555,411,444,311,333,211,222,654,543,
	432,321,665,664,663,662,661,655,653,652,651,644,643,642,
	641,633,632,631,622,621,554,553,552,551,544,542,541,533,
	532,531,522,521,443,442,441,433,431,422,332,331,322,221
};

int canonicalize(int r) {
	int res = 0;
	for (int k = 6; k >= 1; k--) {
		for (int tmp = r; tmp > 0; tmp/=10) {
			if (tmp % 10 == k) {
				res *= 10;
				res += tmp % 10;
			}
		}
	}
	return res;
}

int replace(int r, int i, int d) {
	int tmp = pow(10, i);
	return r - (r / tmp % 10) * tmp + d * tmp;
}

void update(entry &min, entry o, function<bool(float,float)> cmp) {
	if (cmp(o.val, min.val)) {
		min.val = o.val;
		min.action = o.action;
	}
}

list<int> outcomes(int r = 0, int a = 0b111) {
	// if (r == 221) return {}; // rule variant
	list<int> res = {r};
	for (int i = 0; i < N_DICE; i++) {
		if ((a >> i) & 1) {
			int end = res.size();
			for (int s = 0; s < end; s++) {
				int curr = res.front();
				res.pop_front();
				for (int d = 1; d <= 6; d++)
					res.push_back(replace(curr, i, d));
			}
		}
	}
	transform(res.begin(), res.end(), res.begin(), &canonicalize);
	return res;
}

float ev(list<int> X, function< float(float, int)> f) {
	return accumulate(X.begin(), X.end(), 0.0, f) / X.size();
}

float eval(int a, int b) {
	if (a == 221) return 0;
	if (a == b) return 0.5;
	for (int r : ranking)
		if (a == r) return 1;
		else if (b == r) return 0;
	return -1;
}

void compute_f() {
	for (int r : ranking)
		f[r][N_ROLL - 1].val = ev(outcomes(), [r] (float sum, float xi) { return sum + g[r][xi][N_ROLL - 1].val; });

	for (int t = 1; t >= 0; t--) {
		for (int r : ranking) {
			entry max = { ev(outcomes(), [r, t] (float sum, float xi) { return sum + g[r][xi][t].val; }) };
			for (int a = 0b001; a <= 0b111; a++)
				update(max, {ev(outcomes(r, a), [t] (float sum, float xi) { return sum + f[xi][t+1].val; }), a}, greater<float>());
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
				entry min = {eval(r1, r2)};
				for (int a = 0b001; a <= 0b111; a++)
					update(min, {ev(outcomes(r2, a), [r1, t] (float sum, float xi) { return sum + g[r1][xi][t-1].val; }), a}, less<float>());
				g[r1][r2][t] = min;
			}
		}
	}
}

string action_to_string(int r, int a) {
	string res = "";
	for (int i = N_DICE - 1; i >= 0; i--)
		if (!((a >> i) & 1))
			res += '0' + r / (int)pow(10, i) % 10;
	return res;
}

uniform_int_distribution<int> die(1,6);
default_random_engine roll;

int apply_action(int r, int a) {
	int res;
	for (int i  = 0; i < N_DICE; i++)
		if ((a >> i) & 1)
			res = replace(r, i, die(roll));
	return canonicalize(res);
}

float sim() {
	int t = 0;

	int r = canonicalize(die(roll) * 100 + die(roll) * 10 + die(roll));
	for ( ;f[r][t].action; t++)
		r = apply_action(r, f[r][t].action);

	int s = canonicalize(die(roll) * 100 + die(roll) * 10 + die(roll));
	for ( ;g[r][s][t].action; t--)
		s = apply_action(s, g[r][s][t].action);

	return eval(r, s);
}

void print_f() {
	cout << "    |  0           |  1           |  2           |" << endl;
	cout << "    | proba |action| proba |action| proba |action|" << endl;
	for (int r : ranking) {
		cout << r << " | ";
		for (int t = 0; t < N_ROLL; t++)
			printf("%5.2f%% (%3s) | ", f[r][t].val*100, action_to_string(r, f[r][t].action).c_str());
		cout << endl;
	}
}


int main(int argc, char const * argv[]) {

	compute_g();
	compute_f();
	print_f();
	cout << ev(outcomes(), [] (float sum, int xi) { return sum + f[xi][0].val; }) << endl;

	return 0;
}
