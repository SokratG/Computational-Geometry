#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

using std::min;
using std::max;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::numeric_limits;

constexpr int inf = numeric_limits<int>::max();

class Point
{
public:
	int x;
	int y;
public:
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	long long  length() {
		return sqrt(x * x + y * y);
	}
	bool operator==(const Point& p) const {
		return (x == p.x && y == p.y);
	}
	bool operator<(const Point& p) const;
};



void ClosestPoints(const vector<Point>& points, const vector<Point>& queries)
{
	size_t n = points.size(), m = queries.size();
	int dist;
	for (size_t i = 0; i < m; ++i) {
		dist = inf;
		int l, r;
		for (size_t j = 0; j < n; ++j) {
			l = abs(points[j].x - queries[i].x);
			r = abs(points[j].y - queries[i].y);
			dist = min(dist, max(l, r));
		}
		cout << dist << endl;
	}	
}



int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);

	size_t N = 0;
	cin >> N;
	size_t i;

	vector<Point> points;
	for (i = 0; i < N; ++i)
	{
		int x, y;
		cin >> x >> y;
		points.emplace_back(Point(x, y));
	}

	size_t M = 0;
	cin >> M;
	vector<Point> queries;
	for (i = 0; i < M; ++i)
	{
		int x, y;
		cin >> x >> y;
		queries.emplace_back(Point(x, y));
	}
	ClosestPoints(points, queries);
	

	return 0;
}
