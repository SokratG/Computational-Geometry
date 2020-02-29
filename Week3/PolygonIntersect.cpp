// ComputationalGeometry.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
#include <tuple>
#include <limits>
#include <set>
#include <iterator>

using std::set;
using std::numeric_limits;
using std::min;
using std::max;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::sort;
using std::tuple;


using std::get;
using AngleIndex = tuple<double, size_t>;
constexpr long long inf = numeric_limits<int>::max();

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
	Point operator-(const Point& p) const;
	bool operator==(const Point& p) const {
		return (x == p.x && y == p.y);
	}
	bool operator<(const Point& p) const {
		return (x < p.x) || ((x == p.x) && (y < p.y));
	}
};

using AnglePoint = tuple<double, Point>;


enum class POSITIONPOINT {
	RIGHT = -1,
	LEFT = 1,
	COLLINEAR = 0,
};

// check if point C lies on segment AB
bool OnSegment(const Point& a, const Point& b, const Point& c)
{
	return (((min(a.x, b.x) <= c.x) && (max(a.x, b.x) >= c.x)) // in interval
		&& ((min(a.y, b.y) <= c.y) && (max(a.y, b.y) >= c.y)));
}


long long Area(const Point& a, const Point& b, const Point& c)
{
	return (((long long)b.x - a.x) * ((long long)c.y - a.y))
		- (((long long)c.x - a.x) * ((long long)b.y - a.y));
}

Point pointIntersect(const Point& a, const Point& b, const Point& c, const Point& d)
{
	long long A1 = ((long long)b.y - a.y);
	long long B1 = ((long long)a.x - b.x);
	long long C1 = A1 * a.x + B1 * a.y;
	// second Line
	long long A2 = ((long long)d.y - c.y);
	long long B2 = ((long long)c.x - d.x);
	long long C2 = A2 * c.x + B2 * c.y;
	// calc determinant
	long long det = A1 * B2 - A2 * B1;

	assert(det != 0);

	return Point((B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det);
}


POSITIONPOINT Area2(const Point& a, const Point& b, const Point& c)
{
	long long res = Area(a, b, c);

	if (res > 0)
		return POSITIONPOINT::LEFT;		// left turn
	else if (res < 0)
		return POSITIONPOINT::RIGHT;		// right turn
	else {					// == 0
		return POSITIONPOINT::COLLINEAR;	// collinear
	}
}

bool cmpDouble(double a, double b)
{
	constexpr double EPSILON = 0.000000001;
	return fabs(a - b) < EPSILON;
}

size_t BinarySearch(vector<AngleIndex>& anglevec, double angle)
{
	size_t left = 0, right = anglevec.size() - 1;

	while (left < right) {
		size_t mid = (left + right) / 2;
		if (cmpDouble(get<0>(anglevec[mid]), angle))//get<0>(anglevec[mid]) == angle)
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


Point GetPointIntersect(const Point& a, const Point& b, const Point& c, const Point& d)
{
	if (((a == c && !OnSegment(a, b, d)) || (a == d && !OnSegment(a, b, c))) && !OnSegment(c, d, b)) {
		return a;
	}
	else if (((b == c && !OnSegment(a, b, d)) || (b == d && !OnSegment(a, b, c))) && !OnSegment(c, d, a)) {
		return b;
	}

	POSITIONPOINT P1 = Area2(a, b, c);
	POSITIONPOINT P2 = Area2(a, b, d);
	POSITIONPOINT P3 = Area2(c, d, a);
	POSITIONPOINT P4 = Area2(c, d, b);

	// if all position are different => intersect
	if (P1 != P2 && P3 != P4)
	{	
		return pointIntersect(a, b, c, d);
	}


	if (P1 == POSITIONPOINT::COLLINEAR && OnSegment(a, b, c))
		return c;
	else if (P2 == POSITIONPOINT::COLLINEAR && OnSegment(a, b, d))
		return d;
	else if (P3 == POSITIONPOINT::COLLINEAR && OnSegment(c, d, a))
		return a;
	else if (P4 == POSITIONPOINT::COLLINEAR && OnSegment(c, d, b))
		return b;
	else
		return Point(inf, inf);
}


set<Point> PointConvexPolygon(const vector<Point>& Polygon, const vector<Point>& Points)
{
	// compute center masses
	const int N = 3;
	//long long _x = 0; long long _y = 0;
	double _x = 0.; double _y = 0.;
	for (int i = 0; i < N; i++) {
		_x += Polygon[i].x;
		_y += Polygon[i].y;
	}
	double CMx = (_x / N); double CMy =(_y / N);

	//
	vector<AngleIndex> AnglePoint;
	const size_t PolSize = Polygon.size();
	for (size_t i = 0; i < PolSize; ++i) {
		Point dp = Polygon[i];
		double dx = (double)dp.x - CMx; double dy = (double)dp.y - CMy;
		AnglePoint.emplace_back(std::make_tuple(atan2(dy, dx), i));
	}

	// sort for b search
	sort(AnglePoint.begin(), AnglePoint.end(), [](const AngleIndex A1, const AngleIndex A2) {
		return (get<0>(A1) < get<0>(A2));
		});
	
	set<Point> SetPolygon;
	for (const auto& point : Points) {
		double dx = (double)point.x - CMx; double dy = (double)point.y - CMy;
		double angle = atan2(dy, dx);
		size_t idx_v = BinarySearch(AnglePoint, angle);
		size_t idx = (idx_v == 0) ? (PolSize - 1) : ((idx_v - 1) % PolSize);
		POSITIONPOINT res = Area2(Polygon[idx], Polygon[idx_v], point);
		if (res == POSITIONPOINT::COLLINEAR && OnSegment(Polygon[idx], Polygon[idx_v], point)) // check collinear 
			SetPolygon.insert(point);
		else if (res == POSITIONPOINT::LEFT)
			SetPolygon.insert(point);		
	}
	return SetPolygon;
}



void PolygonIntersect(const vector<Point>& Polygon1, const vector<Point>& Polygon2)
{
	set<Point> SetPolygon;
	size_t SizeP1 = Polygon1.size();
	size_t SizeP2 = Polygon2.size();

	for (size_t i = 0; i < SizeP1; ++i) {
		Point A = Polygon1[i];
		Point B = Polygon1[(i+1) % SizeP1];
		for (size_t j = 0; j < SizeP2; ++j) {
			Point C = Polygon2[j];
			Point D = Polygon2[(j+1) % SizeP2];
			Point res = GetPointIntersect(A, B, C, D);
			if (res.x == inf)
				continue;
			SetPolygon.insert(res);
		}
	}

	set<Point> _SetPolygon1(std::move(PointConvexPolygon(Polygon1, Polygon2)));
	set<Point> _SetPolygon2(std::move(PointConvexPolygon(Polygon2, Polygon1)));


	set<Point> ResultSet(std::move(SetPolygon));
	std::merge(_SetPolygon1.begin(), _SetPolygon1.end(), _SetPolygon2.begin(), _SetPolygon2.end(),
				std::inserter(ResultSet, ResultSet.begin()));


	if (ResultSet.empty()) {
		cout << 0 << endl;
		return;
	}

	vector<Point> points{ ResultSet.begin(), ResultSet.end() };
	const int N = 3;
	long long _x = 0; long long _y = 0;
	for (int i = 0; i < N; i++) {
		_x += points[i].x;
		_y += points[i].y;
	}
	Point CM(_x / N, _y / N);

	size_t res_size = ResultSet.size();
	vector<AnglePoint> _points;
	for (size_t i = 0; i < res_size; ++i) {
		Point dp = points[i];
		long long dx = (long long)dp.x - CM.x; long long dy = (long long)dp.y - CM.y;
		_points.emplace_back(std::make_tuple(atan2(dy, dx), dp));
	}

	sort(_points.begin(), _points.end(), [](const AnglePoint A1, const AnglePoint A2) {
		return (get<0>(A1) < get<0>(A2));
		});

	// print result
	cout << _points.size() << endl;
	for (const auto& p : _points)
		cout << get<1>(p).x << " " << get<1>(p).y << " ";

}


int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);
	
	size_t N = 0u, M = 0u;

	cin >> N;
	vector<Point> Polygon_1;
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		Polygon_1.emplace_back(Point(x, y));
	}
	cin >> M;
	vector<Point> Polygon_2;
	for (int i = 0; i < M; ++i) {
		int x, y;
		cin >> x >> y;
		Polygon_2.emplace_back(Point(x, y));
	}

	PolygonIntersect(Polygon_1, Polygon_2);

	return 0;
}
