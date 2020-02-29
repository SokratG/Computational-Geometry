// ComputationalGeometry.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stack>
#include <tuple>

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
	int x;
	int y;
public:
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	int length() {
		return sqrt(x * x + y * y);
	}
	void Swap(Point& p) { Point temp = p; p = *this; *this = temp;}
	bool operator==(const Point& p) const;
	bool operator<(const Point& p) const;
};


int Area(const Point& a, const Point& b, const Point& c)
{
	return ((b.x - a.x) * (c.y - a.y))
			- ((c.x - a.x) * (b.y - a.y));
}


size_t underTop(stack<size_t>& S)
{
	size_t temp = S.top(); S.pop();
	size_t under = S.top(); S.push(temp);
	return under;
}

vector<Point> BuildConvexPolygon(vector<Point>& points)
{
	size_t Size = points.size();
	int Ymin = points[0].y; int min = 0;
	// Find the bottommost point 
	for (size_t i = 0; i < Size; ++i) {
		int y = points[i].y;
		if ((y < Ymin) || (y == Ymin && points[i].x > points[min].x))
		{
			Ymin = y; min = i;
		}
	}

	Point pivotPoint = Point(points[min]);
	vector<AngleIndex> AnglePoint;
	for (size_t i = 0; i < Size; ++i) {
		if (i != min) {
			Point P(points[i]);
			int dx = P.x - pivotPoint.x; int dy = P.y - pivotPoint.y;
			AnglePoint.emplace_back(std::make_tuple(atan2(dy, dx), i));
		}
	}

	sort(AnglePoint.begin(), AnglePoint.end());

	vector<size_t> index;
	for (size_t i = 0; i < AnglePoint.size(); ++i) {
		index.emplace_back(get<1>(AnglePoint[i]));
	}

	stack<size_t> S;
	S.push(*(index.end() - 1));
	S.push(min);

	int i = 0;
	while (i < index.size()) {
		size_t prev = underTop(S);
		size_t top = S.top();
		size_t curr = index[i];
		if (Area(points[prev], points[top], points[curr]) > 0) {
			S.push(curr);
			++i;
		}
		else
			S.pop();			
	}

	
	size_t Size_S = S.size();
	S.pop();
	stack<size_t> T;
	while (!S.empty()) {
		T.push(S.top());
		S.pop();
	}

	vector<size_t> res_index;
	while (!T.empty()) {
		res_index.emplace_back(T.top());
		T.pop();
	}

	size_t I1 = *(res_index.end() - 1);
	size_t I2 = res_index[0];
	size_t I3 = res_index[1];

	if (Area(points[I1], points[I2], points[I3]) == 0)
		res_index = { res_index.begin() + 1, res_index.end() };

	vector<Point> res;
	for (const auto& i : res_index) {
		res.emplace_back(points[i]);
	}

	return res;
}


int main(int argc, char* argv[])
{

	std::ios_base::sync_with_stdio(false);

	int N = 0;
	cin >> N;
	if (N < 3 || N > 5000)
		return 0;

	vector<Point> SetPoint;
	for (size_t i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		SetPoint.emplace_back(Point(x, y));
	}


	vector<Point> CH{ BuildConvexPolygon(SetPoint) };

	int M = CH.size();
	cout << M << endl;
	for (size_t i = 0; i < M; ++i) {
		cout << CH[i].x << " " << CH[i].y << " ";
	}


	return 0;
}
