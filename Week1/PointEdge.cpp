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

const int MAX = 1000;
const int MAXVAL = 10000;

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
	RIGHT = -1,
	LEFT = 1,
	ON_LINE = 0,
	ON_SEGMENT = 2
};


POSITION Area2(const Point& a, const Point& b, const Point& c)
{
	int res = ((b.x - a.x) * (c.y - a.y))
			- ((c.x - a.x) * (b.y - a.y));

	if (res > 0)
		return LEFT;		// left turn
	else if (res < 0)
		return RIGHT;		// right turn
	if (res == 0) {
		if (((min(a.x, b.x) <= c.x) && (max(a.x, b.x) >= c.x)) // in interval
			&& ((min(a.y, b.y) <= c.y) && (max(a.y, b.y) >= c.y))) {
			return ON_SEGMENT;
		}
		else
			return ON_LINE;
	}
}


#ifdef TEST
POSITION Area2(const Point& a, const Point& b, const Point& c)
{

	long long res = ((static_cast<long long>(b.x) - a.x) * ((static_cast<long long>(c.y) - a.y))
					- (static_cast<long long>(c.x) - a.x) * (static_cast<long long>(b.y) - a.y));
	if (res > 0) 
		return LEFT;		// left turn
	else if (res < 0)
		return RIGHT;		// right turn
	else {					// collinear(on line)
		Point p1(b - a);
		Point p2(c - a);
		if (p1.length() < p2.length())
			return ON_LINE;
	}
	return ON_SEGMENT;
}
#endif
void PointOfEdge(const Edge& Vec, const vector<Point>& points)
{
	if (points.empty())
		return;

	for (const auto& p : points) {

		if (p == Vec.org || p == Vec.dest) {
			cout << "ON_SEGMENT" << endl;
			continue;
		}	

		POSITION res = Area2(Vec.org, Vec.dest, p);
		switch (res)
		{
		case RIGHT:
			cout << "RIGHT" << endl;
			break;
		case LEFT:
			cout << "LEFT" << endl;
			break;
		case ON_LINE:
			cout << "ON_LINE" << endl;
			break;
		case ON_SEGMENT:
			cout << "ON_SEGMENT" << endl;
			break;
		default:
			return;
			break;
		}
	}
}


int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	// vector on plane
	Point org, dest;
	cin >> org.x >> org.y >> dest.x >> dest.y;
	Point test = dest - org;

	if (test.length() == 0)
		return 0;

	Edge Vec(org, dest);

	// amount of points:
	int N;
	cin >> N;

	if (N < 1 || N > MAX)
		return 0;

	vector<Point> points;
	
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		if (x > MAXVAL || y > MAXVAL)
			return 0;
		points.emplace_back(Point(x, y));
	}

	PointOfEdge(Vec, points);



	return 0;
}


