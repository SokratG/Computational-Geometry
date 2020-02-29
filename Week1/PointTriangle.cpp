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


int Area(const Point& a, const Point& b, const Point& c)
{
	return abs(((b.x - a.x) * (c.y - a.y))
			- ((c.x - a.x) * (b.y - a.y)));
}




POSITION checkPointOfTriangle(const vector<Edge>& triangle, const Point& point)
{
	// calculate triangles with different angle
	vector<int> areas;

	for (const auto& item : triangle) {
		areas.emplace_back(Area(item.org, item.dest, point));
	}
	int sumArea = 0;
	for (const auto& item : areas) {
		sumArea += item;
	}
	int AreaTrio = Area(triangle[0].org, triangle[1].org, triangle[2].org);
		
	if (sumArea == AreaTrio) {
		for (const auto& it : areas)
			if (it == 0)
				return BORDER;
		return INSIDE;
	}
	else
		return OUTSIDE;

}


void PointTriangle(const vector<Edge>& triangle, const vector<Point>& points)
{
	for (const auto& point : points) {
		POSITION pos = checkPointOfTriangle(triangle, point);
		switch (pos)
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
	// create triangle
	vector<Edge> triangle;
	
	int Ax, Ay, Bx, By, Cx, Cy;
	cin >> Ax >> Ay >> Bx >> By >> Cx >> Cy;
	triangle.emplace_back(Point(Ax, Ay), Point(Bx, By));
	triangle.emplace_back(Point(Bx, By), Point(Cx, Cy));
	triangle.emplace_back(Point(Cx, Cy), Point(Ax, Ay));
	
	// create query points
	size_t N = 0;
	cin >> N;
	if (N < 1 || N > 1000)
		return 0;

	vector<Point> points;
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		points.emplace_back(Point(x, y));
	}

	PointTriangle(triangle, points);

	return 0;
}
