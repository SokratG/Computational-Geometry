#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>

using std::cout;
using std::cin;
using std::endl;
using std::vector;




class Point
{
public:
	int x;
	int y;
public:
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	bool operator==(const Point& p) const {
		return (x == p.x && y == p.y);
	}
	bool operator!=(const Point& p) const {
		return (x != p.x || y != p.y);
	}
	Point& operator=(const Point& p) {
		if (*this == p)
			return *this;
		x = p.x;
		y = p.y;
		return *this;
	}
};

class Vertex
{
public:
	Point point;
	Vertex* next;
	Vertex* prev;
	bool isEar;
	Vertex(Point _point) : point(_point) {}
	bool operator!=(const Vertex& v) const {
		return this->point != v.point;
	}
	bool operator==(const Vertex& v) const {
		return this->point == v.point;
	}
};


long long Area2(const Point& a, const Point& b, const Point& c)
{
	return (((long long)b.x - a.x) * ((long long)c.y - a.y))
		- (((long long)c.x - a.x) * ((long long)b.y - a.y));
}

bool isLeft(const Point& a, const Point& b, const Point& c)
{
	long long res = Area2(a, b, c);

	return (res > 0);
}

bool isLeftCollinear(const Point& a, const Point& b, const Point& c)
{
	long long res = Area2(a, b, c);

	return (res >= 0);
}

class Polygon
{
private:
	Vertex* head;
	size_t size;
public:
	Polygon(const vector<Point>& _points) {
		assert(_points.size() > 3);
			
		size = _points.size();
		this->head = createVertices(_points);
	}

	Vertex* Head() const {
		return head;
	}

	void Delete(Vertex* v) {

		//
		Vertex* prev = v->prev;
		Vertex* next = v->next;
		prev->next = next;
		next->prev = prev;

		if (head == v)
			head = next;

		prev->isEar = CheckEar(prev);
		next->isEar = CheckEar(next);

		v->prev = v->next = nullptr;
		delete v;
		--size;
	}

	Vertex* createVertices(const vector<Point>& _points)
	{
		Vertex* head = nullptr; Vertex* tail = nullptr;
		for (Point p : _points) {
			Vertex* vert = new Vertex(p);
			if (tail != nullptr) {
				tail->next = vert;
				vert->prev = tail;
			}
			else
				head = vert;
			tail = vert;
		}

		tail->next = head;
		head->prev = tail;

		Vertex* vertex = head;
		do {
			vertex->isEar = CheckEar(vertex);
			vertex = vertex->next;
		} while (vertex != head);

		return head;
	}
	bool CheckEar(Vertex* vert) {
		Point a = vert->prev->point;
		Point b = vert->next->point;

		Vertex* current = vert;
		do {
			Point c = vert->point;
			Point d = vert->next->point;
			if (a != c && a != d && b != c && b != d 
				&& intersect(a, b, c, d)) {
				return false;
			}
			vert = vert->next;
		} while (current != vert);

		return isInternalDiagonal(*vert->prev, *vert->next);
	}
	size_t Size() const{
		return this->size;
	}
	/*~Polygon() {
		while (head) {
			Vertex* temp = head;
			head = temp->next;
			if (temp)
				delete temp;
		} 
	}*/
private:
	bool intersect(const Point& a, const Point& b, const Point& c, const Point& d) {
		return (isLeft(a, b, c) != isLeft(a, b, d)) 
				&& (isLeft(c, d, a) != isLeft(c, d, b));
	}
	bool isInternalDiagonal(const Vertex& v1,const Vertex& v2) {
		Point a = v1.point;
		Point b = v2.point;
		Point _prev = v1.prev->point;
		Point _next = v1.next->point;

		if (isLeftCollinear(a, _next, _prev))
			return isLeft(a, b, _prev) && isLeftCollinear(b, a, _next);

		return !(isLeftCollinear(a, b, _next) && isLeftCollinear(b, a, _prev));
	}
};



void Ear_Cutting(const vector<Point>& points)
{
	vector<Point> result;
	Polygon pol(points);
	Vertex* vert = pol.Head();

	while (pol.Size() > 3) {
		Vertex* next = vert->next;
		if (vert->isEar) {
			result.emplace_back(vert->prev->point);
			result.emplace_back(vert->point);
			result.emplace_back(vert->next->point);
			pol.Delete(vert);
		}
		vert = next;
	}

	if (pol.Size() == 3) {
		result.emplace_back(pol.Head()->point);
		result.emplace_back(pol.Head()->next->point);
		result.emplace_back(pol.Head()->next->next->point);
	}

	// print result
	cout << result.size() / 3 << endl;
	size_t _size_list = result.size();
	int i = 0;
	while (i < _size_list) {
		cout << result[i].x << " " << result[i].y << " ";
		i++;
		cout << result[i].x << " " << result[i].y << " ";
		i++;
		cout << result[i].x << " " << result[i].y << endl;
		++i;
	}
	

}






int main(int argc, char* argv[])
{
	std::ios_base::sync_with_stdio(false);

	size_t N = 0;
	cin >> N;
	vector<Point> points;
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		points.emplace_back(Point(x, y));
	}

	Ear_Cutting(points);

	//system("pause");

	return 0;
}