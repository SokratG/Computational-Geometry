
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using std::min;
using std::max;
using std::vector;
using std::cout;
using std::endl;
using std::cin;


void CalcNumber(size_t N)
{
	N -= 2;
	vector<size_t> number(N + 1, 0);
	number[0] = 1;
	number[1] = 1;
	for (size_t i = 2; i < N + 1; ++i) {
		for (size_t j = 0; j < i; ++j) {
			number[i] += number[j] * number[i - 1 - j];
		}
	}
	cout << number[N] << endl;
}


int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);
	size_t N = 0;
	cin >> N;

	CalcNumber(N);


	return 0;
}