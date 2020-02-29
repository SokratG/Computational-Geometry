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
	int length() {
		return sqrt(x * x + y * y);
	}
	Point operator-(const Point& p) const;
	bool operator==(const Point& p) const {
		return (x == p.x && y == p.y);
	}
	bool operator<(const Point& p) const;
};


enum  POSITIONPOINT {
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
	return ((b.x - a.x) * (c.y - a.y))
		- ((c.x - a.x) * (b.y - a.y));
}


POSITIONPOINT Area2(const Point& a, const Point& b, const Point& c)
{
	long long res = Area(a, b, c);

	if (res > 0)
		return LEFT;		// left turn
	else if (res < 0)
		return RIGHT;		// right turn
	else {					// == 0
		return COLLINEAR;	// collinear
	}
}

void isIntersect(const Point& a, const Point& b, const Point& c, const Point& d)
{

	
	if (((a == c && !OnSegment(a, b, d))  ||  (a == d && !OnSegment(a, b, c))) && !OnSegment(c, d, b)) {
		cout << "The intersection point is (" << a.x << ", " << a.y << ")." << endl;
		return;
	}
	else if (((b == c && !OnSegment(a, b, d)) || (b == d && !OnSegment(a, b, c))) && !OnSegment(c, d, a)) {
		cout << "The intersection point is (" << b.x << ", " << b.y << ")." << endl;
		return;
	}

	POSITIONPOINT P1 = Area2(a, b, c);
	POSITIONPOINT P2 = Area2(a, b, d);
	POSITIONPOINT P3 = Area2(c, d, a);
	POSITIONPOINT P4 = Area2(c, d, b);


	// if all position are different => intersect
	if (P1 != P2 && P3 != P4)
	{
		// first Line
		long long A1 = (b.y - a.y);
		long long B1 = (a.x - b.x);
		long long C1 = A1 * a.x + B1 * a.y;
		// second Line
		long long A2 = (d.y - c.y);
		long long B2 = (c.x - d.x);
		long long C2 = A2 * c.x + B2 * c.y;
		// calc determinant
		long long det = A1 * B2 - A2 * B1;

		if (det == 0) {
			cout << "No common points." << endl;
			return;
		}

		Point P((B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det);
		cout << "The intersection point is (" << P.x << ", " << P.y << ")." << endl;
		return;
	}
		

	if (   (P1 == COLLINEAR && OnSegment(a, b, c))
		|| (P2 == COLLINEAR && OnSegment(a, b, d))
		|| (P3 == COLLINEAR && OnSegment(c, d, a))
		|| (P4 == COLLINEAR && OnSegment(c, d, b)) ) {
		cout << "A common segment of non-zero length." << endl;
		return;
	}
	
	cout << "No common points." << endl;
}


int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);
	const size_t N = 4;
	vector<Point> Segments;

	

	for (size_t i = 0; i < N; ++i) {
		long long x, y;
		cin >> x >> y;
		Segments.emplace_back(Point(x, y));
	}
	Point a = Segments[0];
	Point b = Segments[1];
	Point c = Segments[2];
	Point d = Segments[3];

	isIntersect(a, b, c, d);

	return 0;
}
