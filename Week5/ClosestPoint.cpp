
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


int BinarySearch(const vector<int>& points, int point)
{
	size_t left = 0, right = points.size() - 1;

	while (left < right) {
		size_t mid = (left + right) / 2;
		if (points[mid] == point)
			return point;
		else if (points[mid] < point)
			left = mid + 1;
		else
			right = mid;

	}

	if ((left > 0) && abs(points[left-1] - point) < abs(points[left] - point) )
		return points[left - 1];

	return points[left];
}

void ClosetPoint(vector<int>& points, const vector<int>& queries)
{
	size_t m = queries.size();

	std::sort(points.begin(), points.end());

	for (size_t i = 0; i < m; ++i) {
		cout << BinarySearch(points, queries[i]) << endl;
	}
	
}



int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);

	size_t N = 0;
	cin >> N;

	vector<int> points;
	for (size_t i = 0; i < N; ++i) {
		int x;
		cin >> x;
		points.emplace_back(x);
	}

	size_t M = 0;
	cin >> M;
	vector<int> queries;
	for (size_t i = 0; i < M; ++i) {
		int x;
		cin >> x;
		queries.emplace_back(x);
	}

	ClosetPoint(points, queries);

	return 0;
}
