// ComputationalGeometry.cpp : Defines the entry point for the application.
//

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



class Point
{
public:
	long long x;
	long long y;
public:
	Point() : x(0), y(0) {}
	Point(long long  _x, long long  _y) : x(_x), y(_y) {}
	long long  length() {
		return sqrt(x * x + y * y);
	}
	Point operator-(const Point& p) const;
	bool operator==(const Point& p) const;
	bool operator<(const Point& p) const {
		return (x < p.x || ((x == p.x) && (y < p.y)));
	}
};


long long Area(const Point& a, const Point& b, const Point& c)
{
	return ((b.x - a.x) * (c.y - a.y))
		   - ((c.x - a.x) * (b.y - a.y));
}

bool toLeft(const Point& a, const Point& b, const Point& c) {
	return (Area(a, b, c) > 0);
}

vector<Point> CheckTangent(const vector<Point>& CH, const Point& p)
{
	vector<Point> resultLeft; vector<Point> resultRight;
	size_t m = CH.size();
	Point Prev = CH[m - 1]; Point Next; Point Curr;
	for (size_t i = 0; i < m; ++i) {
		if (i > 0)
			Prev = CH[i - 1];			
		Curr = CH[i];
		Next = CH[(i + 1) % m];
		bool p_prev = toLeft(Prev, Curr, p);
		bool p_next = toLeft(Curr, Next, p);
		if (!p_prev && p_next)
			resultRight.emplace_back(Curr);
		if (p_prev && !p_next)
			resultLeft.emplace_back(Curr);
	}
	
	vector<Point> result{ resultLeft };
	result.insert(result.end(), resultRight.begin(), resultRight.end());
	
	return result;
}

void BuildTangent(const vector<Point>& CH, const vector<Point>& points)
{
	for (const auto& p : points) {
		vector<Point> TangentVector{ CheckTangent(CH, p) };
		for (const auto& it : TangentVector)
			cout << " " << it.x << " " << it.y;
		cout << endl;
	}

}

int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);
	
	size_t N = 0;
	cin >> N;


	vector<Point> ConvexPolygon;
	for (size_t i = 0; i < N; ++i) {
		long long  x, y;
		cin >> x >> y;
		ConvexPolygon.emplace_back(Point(x, y));
	}

	size_t M = 0;
	cin >> M;

	vector<Point> points;
	for (size_t i = 0; i < M; ++i) {
		long long  x, y;
		cin >> x >> y;
		points.emplace_back(Point(x, y));
	}

	BuildTangent(ConvexPolygon, points);

	return 0;
}
