// ComputationalGeometry.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <tuple>

using std::sort;
using std::tuple;
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
	int x;
	int y;
public:
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	int length() {
		return sqrt(x * x + y * y);
	}
	Point operator-(const Point& p) const;
	bool operator==(const Point& p) const;
	bool operator<(const Point& p) const;
};

bool Point::operator<(const Point& p) const
{
	return (x < p.x) || ((x == p.x) && (y < p.y));
}
bool Point::operator==(const Point& p) const
{
	return (x == p.x) && (y == p.y);
}

Point Point::operator-(const Point& p) const
{
	return Point(x - p.x, y - p.y);
}


class Edge
{
public:
	Point org;
	Point dest;
public:
	Edge() : org(Point(0, 0)), dest(Point(0, 0)) {}
	Edge(const Point& _org, const Point& _dest) : org(_org), dest(_dest) {}

};

enum  POSITION {
	BORDER,
	INSIDE,
	OUTSIDE
};


bool cmpDouble(double a, double b)
{
	constexpr double EPSILON = 0.000001;
	return fabs(a - b) < EPSILON;
}

// check if point C lies on segment AB
bool OnSegment(const Point& a, const Point& b, const Point& c)
{
	return (((min(a.x, b.x) <= c.x) && (max(a.x, b.x) >= c.x)) // in interval
		&& ((min(a.y, b.y) <= c.y) && (max(a.y, b.y) >= c.y)));
}


int Area(const Point& a, const Point& b, const Point& c)
{
	return ((b.x - a.x) * (c.y - a.y))
		- ((c.x - a.x) * (b.y - a.y));
}

size_t BinarySearch(vector<AngleIndex>& anglevec, double angle)
{
	size_t left = 0, right = anglevec.size() - 1;
	
	while (left < right) {
		size_t mid = (left + right) / 2;
		if (get<0>(anglevec[mid]) == angle)
			return get<1>(anglevec[mid]);
		else if (get<0>(anglevec[mid]) < angle)
			left = mid + 1;
		else
			right = mid;

	}

	if ((right == anglevec.size() - 1) && (get<0>(anglevec[right]) < angle))
		return get<1>(anglevec[0]);

	return get<1>(anglevec[right]);
}

void PointConvexPolygon(const vector<Point>& Polygon, const vector<Point>& Points)
{
	// compute center masses
	const int N = 3;
	int _x = 0; int _y = 0;
	for (int i = 0; i < N; i++) {
		_x += Polygon[i].x;
		_y += Polygon[i].y;
	}
	Point CM(_x / N, _y / N);

	//
	vector<AngleIndex> AnglePoint;
	const size_t PolSize = Polygon.size();
	for (size_t i = 0; i < PolSize; ++i) {
		Point dp = Polygon[i];
		int dx = dp.x - CM.x; int dy = dp.y - CM.y;
		AnglePoint.emplace_back(std::make_tuple(atan2(dy, dx), i));
	}

	// sort for b search
	sort(AnglePoint.begin(), AnglePoint.end());

	for (const auto& point : Points) {
		int dx = point.x - CM.x; int dy = point.y - CM.y;
		double angle = atan2(dy, dx);
		size_t idx_v = BinarySearch(AnglePoint, angle);
		size_t idx = (idx_v - 1) % PolSize;
		int res = Area(Polygon[idx], Polygon[idx_v], point);
		if (res == 0 && OnSegment(Polygon[idx], Polygon[idx_v], point)) // check collinear
			cout << "BORDER" << endl;
		else if (res > 0)
			cout << "INSIDE" << endl;
		else
			cout << "OUTSIDE" << endl;
	}
}




int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	// input Convex Polygon
	int m = 0;
	cin >> m;
	if (m < 3 || m > 20000)
		return 0;
	vector<Point> ConvexPolygon;
	for (int i = 0; i < m; ++i) {
		int x, y;
		cin >> x >> y;
		ConvexPolygon.emplace_back(Point(x, y));
	}

	
	// input Points query
	int N = 0;
	cin >> N;
	if (N < 1 || N > 50000)
		return 0;
	vector<Point> points;
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		points.emplace_back(Point(x, y));
	}


	PointConvexPolygon(ConvexPolygon, points);


	return 0;
}
