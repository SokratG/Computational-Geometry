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


class Edge
{
public:
	Point org;
	Point dest;
public:
	Edge() : org(Point(0, 0)), dest(Point(0, 0)) {}
	Edge(const Point& _org, const Point& _dest) : org(_org), dest(_dest) {}
	Point getOrg() const { return org; }
	Point getDest() const { return dest; }
};


int CrossProduct(const Point& a, const Point& b, const Point& c)
{
	Point BA(a.x - b.x, a.y - b.y);
	Point BC(c.x - b.x, c.y - b.y);

	return ((BA.x * BC.y) - (BA.y * BC.x));
}


// check all angle in polygon, for convext polygon must the same sign to all angle
bool checkConvexPolygon(const vector<Point>& convexPolygon)
{

	size_t Size = convexPolygon.size();
	bool negative_res = false, positive_res = false;

	for (size_t i = 0; i < Size; ++i) {
		// index of points, for next check angle between edge
		size_t j = (i + 1) % Size;
		size_t k = (j + 1) % Size;

		// calculate cross product for find sign angle
		int cross_prod = CrossProduct(convexPolygon[i], convexPolygon[j], convexPolygon[k]);

		// check equal to 0
		if (cross_prod < 0)
			negative_res = true;
		if (cross_prod > 0)
			positive_res = true;

		if (negative_res && positive_res)
			return false;
	}

	// If we got this far, the polygon is convex.
	return true;
}


int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);

	vector<Point> convexPolygon;

	int N = 0;
	cin >> N;
	if (N < 3 || N > 500)
		return 0;

	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		convexPolygon.emplace_back(Point(x, y));
	}


	if (checkConvexPolygon(convexPolygon))
		cout << "CONVEX" << endl;
	else
		cout << "NOT_CONVEX" << endl;


	return 0;
}
