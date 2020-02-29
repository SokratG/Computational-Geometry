
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <limits>

using std::numeric_limits;
using std::string;
using std::min;
using std::max;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::to_string;


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
};



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


bool isPointsIntersect(const Point& a, const Point& b, const Point& c, const Point& d)
{
	if (((a == c && !OnSegment(a, b, d)) || (a == d && !OnSegment(a, b, c))) && !OnSegment(c, d, b)) {
		return false;
	}
	else if (((b == c && !OnSegment(a, b, d)) || (b == d && !OnSegment(a, b, c))) && !OnSegment(c, d, a)) {
		return false;
	}

	POSITIONPOINT P1 = Area2(a, b, c);
	POSITIONPOINT P2 = Area2(a, b, d);
	POSITIONPOINT P3 = Area2(c, d, a);
	POSITIONPOINT P4 = Area2(c, d, b);

	// if all position are different => intersect
	if (P1 != P2 && P3 != P4)
	{
		if (a == c || a == d)
			return false;
		if (b == c || b == d)
			return false;
		return true;
	}

	if (P1 == POSITIONPOINT::COLLINEAR && OnSegment(a, b, c))
		return true;
	else if (P2 == POSITIONPOINT::COLLINEAR && OnSegment(a, b, d))
		return true;
	else if (P3 == POSITIONPOINT::COLLINEAR && OnSegment(c, d, a))
		return true;
	else if (P4 == POSITIONPOINT::COLLINEAR && OnSegment(c, d, b))
		return true;
	else
		return false;
}

bool cmpDouble(double a, double b)
{
	constexpr double EPSILON = 0.000000001;
	return fabs(a - b) < EPSILON;
}

bool isInternal(int _i, int _j, const vector<Point>& polygon)
{
	int N = polygon.size();
	Point A = polygon[_i];
	Point B = polygon[_j];
	double Ix = (static_cast<double>(A.x) + B.x) / 2; double Iy = (static_cast<double>(A.y) + B.y) / 2 ;

	size_t cointIntersection = 0;
	for (int i = 0; i < N; ++i) {
		Point C = polygon[(i == 0) ? (N - 1) : (i - 1) % N];
		Point D = polygon[i];
		if ((min(C.y, D.y) <= Iy) && (max(C.y, D.y) > Iy)) {
			if (C.y != D.y) {
				double x = static_cast<double>(C.x) + (static_cast<double>(C.x) - static_cast<double>(D.x)) * (Iy - static_cast<double>(C.y)) / (static_cast<double>(C.y) - D.y);
				if (x <= Ix) {
					cointIntersection++;
				}
					
			}
		}
	}
	return ((cointIntersection % 2) == 1);

}

string BuildResult(int _i, int _j, const vector<Point>& polygon)
{
	size_t N = polygon.size();
	Point A = polygon[_i];
	Point B = polygon[_j];
	string Res{ to_string(A.x) + " " + to_string(A.y) + " " + to_string(B.x) + " " + to_string(B.y) + " " };

	for (size_t k = 0; k < N; ++k) {
		Point C = polygon[k];
		Point D = polygon[(k + 1) % N];
		if (isPointsIntersect(A, B, C, D))
			return { Res + "INTERSECT" };
	}
	if (isInternal(_i, _j, polygon))
		return { Res + "INTERNAL" };
	return  { Res + "EXTERNAL" };
}


void CountDiagonals(const vector<Point>& polygon)
{
	size_t N = polygon.size();
	size_t NumberDiagonals = (N * (N - 1) / 2 - N);
	cout << NumberDiagonals << endl;
	for (int i = 0; i < N - 1; ++i) {
		for (int j = i; j < N; ++j) {
			if (i == j)
				continue;
			if (!(abs(i - j) == 1 || abs(i - j) == (N - 1)))
				cout << BuildResult(i, j, polygon) << endl;
		}
	}

}

int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);

	size_t N = 0;
	cin >> N;
	vector<Point> polygon;
	for (size_t i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		polygon.emplace_back(Point(x, y));
	}

	CountDiagonals(polygon);


	return 0;
}