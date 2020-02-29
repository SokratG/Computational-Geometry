// ComputationalGeometry.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stack>
#include <tuple>
#include <set>

using std::set;
using std::sort;
using std::tuple;
using std::stack;
using std::min;
using std::max;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::get;

using AngleIndex = tuple<double, size_t>;

class Point
{
public:
	long long x;
	long long y;
public:
	Point() : x(0), y(0) {}
	Point(long long  _x, long long  _y) : x(_x), y(_y) {}
	int length() {
		return sqrt(x * x + y * y);
	}
	Point operator-(const Point& p) const;
	bool operator==(const Point& p) const {
		return (x == p.x && y == p.y);
	}
	bool operator<(const Point& p) const {
		return (x < p.x || ((x == p.x) && (y < p.y)));
	}
};


long long Area(const Point& a, const Point& b, const Point& c)
{
	return ((b.x - a.x) * (c.y - a.y))
		- ((c.x - a.x) * (b.y - a.y));
}


void BuildConvexPolygon(vector<Point>& points)
{
	size_t Size = points.size();
	size_t min = 0;
	// Find the bottommost point 
	for (size_t i = 1; i < Size; ++i) {
		if ((points[i].y < points[min].y) || (points[i].y == points[min].y && points[i].x > points[min].x))
			min = i;	
	}

	
	vector<AngleIndex> AnglePoint;
	for (size_t i = 0; i < Size; ++i) {
		if (i != min) {
			long long  dx = points[i].x - points[min].x; long long  dy = points[i].y - points[min].y;
			AnglePoint.emplace_back(std::make_tuple(atan2(dy, dx), i));
		}
	}

	sort(AnglePoint.begin(), AnglePoint.end(), [](const AngleIndex A1, const AngleIndex A2) {
			return (get<0>(A1) < get<0>(A2));
	});

	vector<size_t> index;
	for (size_t i = 0; i < AnglePoint.size(); ++i) {
		index.emplace_back(get<1>(AnglePoint[i]));
	}

	vector<size_t> S;
	S.push_back(index[index.size() - 1]);
	S.push_back(min);

	size_t i = 0;
	while (i < index.size()) {
		size_t prev = *(S.end() - 2);
		size_t top = S.back();
		size_t curr = index[i];
		if (Area(points[prev], points[top], points[curr]) > 0) {
			S.push_back(curr);
			++i;
		}
		else
			S.pop_back();
	}


	vector<size_t> res_index = { S.begin(), S.end() - 1 };
	vector<Point> res;
	for (const auto& it : res_index) {
		res.emplace_back(points[it]);
	}

	cout << res.size() << endl;
	for (const auto& p : res)
		cout << p.x << " " << p.y << " ";
}


void UnionPolygons(const vector<vector<Point>>& Polygons)
{
	set<Point> SetPolygon;
	// create set of one polygon
	for (const auto& polygon : Polygons) {
		for (const auto& p : polygon) {
			SetPolygon.insert(p);
		}
	}

	vector<Point> UnionPolygon;
	for (const auto& p : SetPolygon) {
		UnionPolygon.emplace_back(p);
	}

	BuildConvexPolygon(UnionPolygon);	
}


int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);
	size_t N = 0;
	cin >> N;


	vector<vector<Point>> Polygons(N);
	for (size_t i = 0; i < N; ++i) {
		size_t M = 0;
		cin >> M;
		for (size_t j = 0; j < M; ++j) {
			long long  x, y;
			cin >> x >> y;
			Polygons[i].emplace_back(Point(x, y));
		}
	}

	UnionPolygons(Polygons);

	return 0;
}
