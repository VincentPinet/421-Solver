#include <iostream>
#include <cstdio>
#include <cmath>
#include <functional>
#include <vector>
#include <list>
#include <unordered_map>
#include <random>

#define N_ROLL 3
#define N_DICE 3
#define DICE distribution(engine)

using namespace std;

typedef struct {
	float ev;
	int action = 0;
} entry;

unordered_map<int, unordered_map<int, unordered_map<int, entry>>> d;
unordered_map<int, unordered_map<int, entry>> z;

default_random_engine engine;
uniform_int_distribution<int> distribution(1,6);

vector<int> ranking = {421,111,611,666,511,555,411,444,311,333,211,222,654,543,432,321,
					   665,664,663,662,661,655,653,652,651,644,643,642,641,633,632,631,
					   622,621,554,553,552,551,544,542,541,533,532,531,522,521,443,442,
					   441,433,431,422,332,331,322,221};


float eval(int f, int s) {
	if (f == s) return 0.5;
	for (int r : ranking)
		if (f == r) return 1;
		else if (s == r) return 0;
	return -1;
}

int canonicalize(int n) {
	int res = 0;
	for (int k = 6; k >= 1; k--) {
		for (int tmp = n; tmp > 0; tmp/=10) {
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

list<int> outcomes(int r, int a) {
	if (r == 221) return {};
	list<int> res = {r};
	for (int i  = 0; i < N_DICE; i++) {
		if ((a >> i) & 1) {
			int end = res.size();
			for (int s = 0; s < end; s++) {
				int curr = res.front();
				res.pop_front();
				for (int d = 1; d <= 6; d++) {
					res.push_back(canonicalize(replace(curr, i, d)));
				}
			}
		}
	}
	return res;
}

float gensum(int m, int o, int t, int a, function< float(int, int, int)> f) {
	float sum = 0;
	int total = 0;
	for (int r : outcomes(m, a)) {
		sum += f(r, o, t);
		total++;
	}
	return sum / total;
}

void compute_a() {
	for (int f : ranking)
		z[f][2].ev = gensum(0, f, 2, 7, [] (int m, int o, int t) { return 1 - d[o][m][t].ev; });

	for (int f : ranking) {
		for (int t = 1; t >= 0; t--) {
			float curr = gensum(0, f, t, 7, [] (int m, int o, int t) { return 1 - d[o][m][t].ev; });;
			int curr_a = 0;
			for (int a = 1; a < 8; a++) {
				float sum = gensum(f, 0, t, a, [] (int m, int o, int t) { return z[m][t+1].ev; });
				if (sum > curr) {
					curr = sum;
					curr_a = a;
				}
			}
			z[f][t].ev = curr;
			z[f][t].action = curr_a;
		}
	}
}

void compute_d() {
	for (int f : ranking)
		for (int s : ranking)
			d[f][s][0].ev = eval(s, f);

	for (int f : ranking) {
		for (int s : ranking) {
			for (int t = 1; t < N_ROLL; t++) {
				float curr = eval(s, f);
				int curr_a = 0;
				for (int a = 1; a < 8; a++) {
					float sum = gensum(s, f, t, a, [] (int m, int o, int t) { return d[o][m][t-1].ev; });
					if (sum > curr) {
						curr = sum;
						curr_a = a;
					}
				}
				d[f][s][t].ev = curr;
				d[f][s][t].action = curr_a;
			}
		}
	}
}

string action_to_string(int a, int r) {
	string res = "";
	for (int i = N_DICE - 1; i >= 0; i--)
		if (!((a >> i) & 1))
			res += '0' + r / (int)pow(10, i) % 10;
	return res;
}

int apply_action(int r, int a) {
	int res;
	for (int i  = 0; i < N_DICE; i++)
		if ((a >> i) & 1)
			res = replace(r, i, DICE);
	return canonicalize(res);
}

float sim() {
	int t = 0;
	int f = canonicalize(DICE * 100 + DICE * 10 + DICE);

	// cout << "P1: " << f << " (" << t << ")" << endl;
	for (;z[f][t].action; t++)
		f = apply_action(f, z[f][t].action);

	int s = canonicalize(DICE * 100 + DICE * 10 + DICE);
	// cout << "P2: " << s << " (" << t << ")" << endl;
	for (;d[f][s][t].action; t--) {
		s = apply_action(s, d[f][s][t].action);
	}

	return eval(f, s);
}

int main(int argc, char const * argv[]) {

	compute_d();
	compute_a();

	cout << "    |  0           |  1           |  2           |\n";
	for (int f : ranking) {
		cout << f << " | ";
		for (int t = 0; t < N_ROLL; t++)
			printf("%.4f (%3s) | ", z[f][t].ev, action_to_string(z[f][t].action, f).c_str());
		cout << endl;
	}

	float ev = 0;
	int total = 0;
	for (int f : outcomes(0, 7)) {
		ev += z[f][0].ev;
		total++;
	}
	cout << "TH  : " << ev / total << endl;

	float res = 0;
	for (int i = 0; i <10000000; i++)
		res += sim();
	cout << "SIM : " << res/10000000 << endl;

	return 0;
}
