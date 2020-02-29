// ComputationalGeometry.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>


using std::numeric_limits;
using std::min;
using std::max;
using std::vector;
using std::cout;
using std::endl;
using std::cin;


constexpr int inf = numeric_limits<int>::max();

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
};


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


enum  POSITIONPOINT {
	RIGHT = -1,
	LEFT = 1,
	COLLINEAR = 0,
};

int Area(const Point& a, const Point& b, const Point& c)
{
	return ((b.x - a.x) * (c.y - a.y))
			- ((c.x - a.x) * (b.y - a.y));
}


// check if point C lies on segment AB
bool OnSegment(const Point& a, const Point& b, const Point& c) 
{
	return (((min(a.x, b.x) <= c.x) && (max(a.x, b.x) >= c.x)) // in interval
			&& ((min(a.y, b.y) <= c.y) && (max(a.y, b.y) >= c.y)));
}


POSITIONPOINT Area2(const Point& a, const Point& b, const Point& c)
{
	int res = Area(a, b, c);

	if (res > 0)
		return LEFT;		// left turn
	else if (res < 0)
		return RIGHT;		// right turn
	else {					// == 0
		return COLLINEAR;	// collinear
	}
}

// intersection two segment
bool isIntersect(const Point& a, const Point& b, const Point& c, const Point& d)
{
	POSITIONPOINT P1 = Area2(a, b, c);
	POSITIONPOINT P2 = Area2(a, b, d);
	POSITIONPOINT P3 = Area2(c, d, a);
	POSITIONPOINT P4 = Area2(c, d, b);
	
	// if all position are different => intersect
	if (P1 != P2 && P3 != P4)
		return true;
	// else if someone collinear check boundary
	if (P1 == COLLINEAR && OnSegment(a, b, c))
		return true;
	if (P2 == COLLINEAR && OnSegment(a, b, d))
		return true;
	if (P3 == COLLINEAR && OnSegment(c, d, a))
		return true;
	if (P4 == COLLINEAR && OnSegment(c, d, b))
		return true;

	return false;

}

// if odd intersections means inside else if even outside
POSITION checkPointOfPolygon(const vector<Point>& Polygon, const Point& point)
{
	Point extreme(inf, point.y);

	int N = Polygon.size();

	size_t count = 0, i = 0;
	do {
		size_t next = (i + 1) % N;
		if (isIntersect(Polygon[i], Polygon[next], point, extreme)) {

			if (Area2(Polygon[i], point, Polygon[next]) == COLLINEAR)
				if ((OnSegment(Polygon[i], Polygon[next], point)))
					return BORDER;

			++count;
		}

		i = next;
	} while (i != 0);


	if (count % 2) //check odd
		return INSIDE;
	else
		return OUTSIDE;
}




void PointPolygon(const vector<Point>& Polygon, const vector<Point>& Points)
{

	for (const auto& point : Points) {

		POSITION res = checkPointOfPolygon(Polygon, point);
		switch (res)
		{
		case BORDER:
			cout << "BORDER" << endl;
			break;
		case INSIDE:
			cout << "INSIDE" << endl;
			break;
		case OUTSIDE:
			cout << "OUTSIDE" << endl;
			break;
		default:
			break;
		}
	}


}


int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);
	// input Polygon
	int m = 0;
	cin >> m;
	if (m < 3 || m > 200)
		return 0;
	vector<Point> polygon;
	for (int i = 0; i < m; ++i) {
		int x, y;
		cin >> x >> y;
		polygon.emplace_back(Point(x, y));
	}

	// input Points query
	int N = 0;
	cin >> N;
	if (N < 1 || N > 200)
		return 0;
	vector<Point> points;
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		points.emplace_back(Point(x, y));
	}




	PointPolygon(polygon, points);
	



	return 0;
}
