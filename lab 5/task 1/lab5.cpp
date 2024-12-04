#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <iterator>
#include <algorithm>

namespace constants {
    const long double pi = 3.14159265358979323846;
    const long double equalityPrecision = 1e-6;
    const long double squarePrecision = 1e-1;
    const long double flatAngle = 180;
}

bool isEqualZero(double d) { return std::abs(d) < constants::equalityPrecision; }

struct Point {
public:
    long double x, y;

    Point(long double x = 0, long double y = 0) : x(x), y(y) {}

    void rotate(const Point& center, long double angle) {
        angle *= constants::pi / constants::flatAngle;
        long double x_tmp = x - center.x;
        long double y_tmp = y - center.y;
        x = x_tmp * std::cos(angle) - y_tmp * std::sin(angle) + center.x;
        y = x_tmp * std::sin(angle) + y_tmp * std::cos(angle) + center.y;
    }

    void reflect(const Point& center) {
        x = 2 * center.x - x;
        y = 2 * center.y - y;
    }

    void reflect(const class Line& axis);

    void scale(const Point& center, long double coefficient) {
        x = (x - center.x) * coefficient + center.x;
        y = (y - center.y) * coefficient + center.y;
    }

    long double mod() const { return std::sqrt(x * x + y * y); }

    bool operator==(const Point& B) const { 
        return isEqualZero(x - B.x) && isEqualZero(y - B.y); 
    }
    
    bool operator!=(const Point& B) const { 
        return !(*this == B); 
    }
    
    Point operator+(const Point& B) const { 
        return Point(x + B.x, y + B.y); 
    }
    
    Point operator-(const Point& B) const { 
        return Point(x - B.x, y - B.y); 
    }
    
    Point operator*(long double d) const { 
        return Point(x * d, y * d); 
    }
};

struct Point3D {
public:
    long double x, y, z;

    Point3D(long double x = 0, long double y = 0, long double z = 0) : x(x), y(y), z(z) {}
    
    bool operator==(const Point3D& B) const { 
        return isEqualZero(x - B.x) && isEqualZero(y - B.y) && isEqualZero(z - B.z); 
    }
    
    bool operator!=(const Point3D& B) const { 
        return !(*this == B); 
    }
    
    Point3D operator+(const Point3D& B) const { 
        return Point3D(x + B.x, y + B.y, z + B.z); 
    }
    
    Point3D operator-(const Point3D& B) const { 
        return Point3D(x - B.x, y - B.y, z - B.z); 
    }
    
    Point3D operator*(long double d) const { 
        return Point3D(x * d, y * d, z * d); 
    }
};

long double scalar(const Point& A, const Point& B) { return A.x * B.x + A.y * B.y; }
long double vect(const Point A, const Point& B) { return A.x * B.y - A.y * B.x; }
long double dist(const Point& A, const Point& B) { return std::sqrt(scalar(A - B, A - B)); }
long double area(const Point& A, const Point& B, const Point& C) {
    Point CA = A - C;
    Point CB = B - C;
    return 0.5 * std::abs(vect(CA, CB));
}

class Line {
public:
    long double a;
    long double b;
    long double c;

    Line(const Point& A, const Point& B) : a(A.y - B.y), b(B.x - A.x), c(A.x * B.y - A.y * B.x) {}
    Line(long double k, long double m) : a(k), b(-1), c(m) {}
    Line(const Point& A, long double k) : a(k), b(-1), c(A.y - k * A.x) {}

    bool operator==(const Line& m) const {
        if (!isEqualZero(a * m.b - b * m.a)) { return false; }
        return isEqualZero(a * m.c - c * m.a);
    }
};

void Point::reflect(const Line& axis) {
    long double denom = axis.a * axis.a + axis.b * axis.b;
    x = (axis.b * axis.b * x - axis.a * axis.b * y - axis.a * axis.c) / denom;
    y = (-axis.a * axis.b * x + axis.a * axis.a * y - axis.b * axis.c) / denom;
}

class Body2D {
public:
    virtual long double perimeter() const = 0;
    virtual long double area() const = 0;
    virtual Point getCenter() const = 0;
    
    virtual std::vector<Point>::const_iterator getVerticesBegin() const = 0;
    virtual std::vector<Point>::const_iterator getVerticesEnd() const = 0;
    
    virtual ~Body2D() = default;
};

class Body3D {
public:
    virtual long double volume() const = 0;
    virtual long double surfaceArea() const = 0;
    virtual Point3D getCenter() const = 0;
    
    virtual std::vector<Point3D>::const_iterator getVerticesBegin3D() const = 0;
    virtual std::vector<Point3D>::const_iterator getVerticesEnd3D() const = 0;
    
    virtual ~Body3D() = default;
};

class Shape : public Body2D {
public:
    virtual bool operator==(const Shape& another) const = 0;
    virtual bool operator!=(const Shape& another) const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(const Point& point) const = 0;
    virtual void rotate(const Point& center, long double angle) = 0;
    virtual void reflect(const Point& center) = 0;
    virtual void reflect(const Line& axis) = 0;
    virtual void scale(const Point& center, long double coefficient) = 0;
    virtual ~Shape() = default;
};

class Ellipse : public Shape {
protected:
    Point F1;
    Point F2;
    long double a;
    long double c;
    long double b;
public:
    Ellipse(const Point& A, const Point& B, long double d)
        : F1(A), F2(B), a(d / 2), c(dist(F1, F2) / 2), b(std::sqrt(a * a - c * c)) {}
    
    long double area() const override { return constants::pi * a * b; }
    
    long double perimeter() const override { 
        return constants::pi * (3 * (a + b) - std::sqrt((3 * a + b) * (a + 3 * b)));
    }
    
    Point getCenter() const override { 
        return Point((F1.x + F2.x) / 2, (F1.y + F2.y) / 2); 
    }
    
    std::vector<Point>::const_iterator getVerticesBegin() const override {
        static std::vector<Point> empty;
        return empty.begin();
    }
    
    std::vector<Point>::const_iterator getVerticesEnd() const override { 
        return getVerticesBegin(); 
    }
    
    void rotate(const Point& center, long double angle) override {
        F1.rotate(center, angle);
        F2.rotate(center, angle);
    }
    
    void reflect(const Point& center) override {
        F1.reflect(center);
        F2.reflect(center);
    }
    
    void reflect(const Line& axis) override {
        F1.reflect(axis);
        F2.reflect(axis);
    }
    
    void scale(const Point& center, long double coefficient) override {
        F1.scale(center, coefficient);
        F2.scale(center, coefficient);
        a = std::abs(a * coefficient);
        b = std::abs(b * coefficient);
        c = std::abs(c * coefficient);
    }
    
    bool containsPoint(const Point& point) const override { 
        return (dist(point, F1) + dist(point, F2) <= 2 * a + constants::equalityPrecision); 
    }
    
    bool isCongruentTo(const Shape& another) const override {
        const Ellipse* p = dynamic_cast<const Ellipse*>(&another);
        if (p == nullptr) { return false; }
        return (isEqualZero(p->a - a) && isEqualZero(p->b - b));
    }
    
    bool isSimilarTo(const Shape& another) const override {
        const Ellipse* p = dynamic_cast<const Ellipse*>(&another);
        if (p == nullptr) { return false; }
        return isEqualZero(a * p->b - b * p->a);
    }
    
    bool operator==(const Shape& another) const override {
        const Ellipse* p = dynamic_cast<const Ellipse*>(&another);
        if (p == nullptr) { return false; }
        return (F1 == p->F1 && F2 == p->F2 && isEqualZero(a - p->a));
    }
    
    bool operator!=(const Shape& another) const override {
        return !(*this == another);
    }
};

class Parallelogram : public Shape {
private:
    std::vector<Point> vertices;
public:
    Parallelogram(const Point& A, const Point& B, const Point& C)
        : vertices{A, B, C, B + C - A} {}
    
    long double perimeter() const override {
        if (vertices.size() < 4) return 0;
        return 2 * (dist(vertices[0], vertices[1]) + dist(vertices[1], vertices[2]));
    }
    
    long double area() const override {
        if (vertices.size() < 4) return 0;
        Point AB = vertices[1] - vertices[0];
        Point AD = vertices[3] - vertices[0];
        return std::abs(vect(AB, AD));
    }
    
    Point getCenter() const override { 
        return Point((vertices[0].x + vertices[2].x) / 2, (vertices[0].y + vertices[2].y) / 2);
    }
    
    std::vector<Point>::const_iterator getVerticesBegin() const override { 
        return vertices.begin(); 
    }
    
    std::vector<Point>::const_iterator getVerticesEnd() const override { 
        return vertices.end(); 
    }
    
    void rotate(const Point& center, long double angle) override {
        for (auto& p : vertices) {
            p.rotate(center, angle);
        }
    }
    
    void reflect(const Point& center) override {
        for (auto& p : vertices) {
            p.reflect(center);
        }
    }
    
    void reflect(const Line& axis) override {
        for (auto& p : vertices) {
            p.reflect(axis);
        }
    }
    
    void scale(const Point& center, long double coefficient) override {
        for (auto& p : vertices) {
            p.scale(center, coefficient);
        }
    }
    
    bool containsPoint(const Point& point) const override {
        long double totalArea = area();
        long double sumAreas = 0;
        for (int i = 0; i < 4; ++i) {
            sumAreas += ::area(vertices[i], vertices[(i + 1) % 4], point);
        }
        return isEqualZero(totalArea - sumAreas) || sumAreas <= totalArea + constants::equalityPrecision;
    }
    
    bool isCongruentTo(const Shape& another) const override {
        const Parallelogram* p = dynamic_cast<const Parallelogram*>(&another);
        if (p == nullptr) { return false; }
        if (vertices.size() != p->vertices.size()) { return false; }

        for (int i = 0; i < 4; ++i) {
            long double side1 = dist(vertices[i], vertices[(i + 1) % 4]);
            long double side2 = dist(p->vertices[i], p->vertices[(i + 1) % 4]);
            if (!isEqualZero(side1 - side2)) return false;
        }
        return true;
    }
    
    bool isSimilarTo(const Shape& another) const override {
        const Parallelogram* p = dynamic_cast<const Parallelogram*>(&another);
        if (p == nullptr) { return false; }
        if (vertices.size() != p->vertices.size()) { return false; }

        long double ratio = dist(vertices[0], vertices[1]) / dist(p->vertices[0], p->vertices[1]);
        for (int i = 1; i < 4; ++i) {
            long double currentRatio = dist(vertices[i], vertices[(i + 1) % 4]) / dist(p->vertices[i], p->vertices[(i + 1) % 4]);
            if (!isEqualZero(ratio - currentRatio)) return false;
        }
        return true;
    }
    
    bool operator==(const Shape& another) const override {
        const Parallelogram* p = dynamic_cast<const Parallelogram*>(&another);
        if (p == nullptr) { return false; }
        if (vertices.size() != p->vertices.size()) { return false; }

        for (int shift = 0; shift < 4; ++shift) {
            bool match = true;
            for (int i = 0; i < 4; ++i) {
                if (!(vertices[i] == p->vertices[(i + shift) % 4])) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }

        for (int shift = 0; shift < 4; ++shift) {
            bool match = true;
            for (int i = 0; i < 4; ++i) {
                if (!(vertices[i] == p->vertices[(shift - i + 4) % 4])) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }
    
    bool operator!=(const Shape& another) const override {
        return !(*this == another);
    }
};

class Sphere : public Body3D {
private:
    Point3D center;
    long double radius;
public:
    Sphere(const Point3D& center, long double radius) : center(center), radius(radius) {}
    
    long double volume() const override { 
        return (4.0 / 3.0) * constants::pi * std::pow(radius, 3); 
    }
    
    long double surfaceArea() const override { 
        return 4 * constants::pi * std::pow(radius, 2); 
    }
    
    Point3D getCenter() const override { 
        return center; 
    }
    
    std::vector<Point3D>::const_iterator getVerticesBegin3D() const override {
        static std::vector<Point3D> empty;
        return empty.begin(); 
    }
    
    std::vector<Point3D>::const_iterator getVerticesEnd3D() const override { 
        return getVerticesBegin3D(); 
    }
    
    void move(const Point3D& newCenter) {
        center = newCenter;
    }
    
    void scale(long double coefficient) {
        radius *= coefficient;
    }
};

class Parallelepiped : public Body3D {
private:
    std::vector<Point3D> vertices;
    Point3D center;
public:
    Parallelepiped(const Point3D& A, const Point3D& B, const Point3D& C, const Point3D& D) {
        vertices.push_back(A);
        vertices.push_back(B);
        vertices.push_back(C);
        vertices.push_back(D);
        long double height = 1.0;
        Point3D E = A + Point3D(0, 0, height);
        Point3D F = B + Point3D(0, 0, height);
        Point3D G = C + Point3D(0, 0, height);
        Point3D H = D + Point3D(0, 0, height);
        vertices.push_back(E);
        vertices.push_back(F);
        vertices.push_back(G);
        vertices.push_back(H);
        center = Point3D((A.x + C.x) / 2, (A.y + C.y) / 2, (A.z + E.z) / 2);
    }
    
    long double volume() const override { 
        Point AB = Point(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y);
        Point AD = Point(vertices[3].x - vertices[0].x, vertices[3].y - vertices[0].y);
        long double baseArea = std::abs(vect(AB, AD));
        long double height = std::abs(vertices[4].z - vertices[0].z);
        return baseArea * height;
    }
    
    long double surfaceArea() const override { 
        long double l = dist3D(vertices[0], vertices[1]);
        long double w = dist3D(vertices[0], vertices[3]);
        long double h = dist3D(vertices[0], vertices[4]);
        return 2 * (l*w + l*h + w*h);
    }
    
    Point3D getCenter() const override { 
        return center; 
    }
    
    std::vector<Point3D>::const_iterator getVerticesBegin3D() const override { 
        return vertices.begin(); 
    }
    
    std::vector<Point3D>::const_iterator getVerticesEnd3D() const override { 
        return vertices.end(); 
    }
    
    void move(const Point3D& newCenter) {
        Point3D shift = newCenter - center;
        for (auto& p : vertices) {
            p = p + shift;
        }
        center = newCenter;
    }
    
    void scale(long double coefficient) {
        for (auto& p : vertices) {
            p = center + (p - center) * coefficient;
        }
    }
    
private:
    static long double dist3D(const Point3D& A, const Point3D& B) {
        return std::sqrt(std::pow(A.x - B.x, 2) + 
                         std::pow(A.y - B.y, 2) + 
                         std::pow(A.z - B.z, 2));
    }
};

int main() {
    Point A(0, 0);
    Point B(4, 0);
    Point C(5, 3);
    Parallelogram pg(A, B, C);
    
    std::cout << "Parallelogram:\n";
    std::cout << "Perimeter: " << pg.perimeter() << "\n";
    std::cout << "Area: " << pg.area() << "\n";
    Point pgCenter = pg.getCenter();
    std::cout << "Center: (" << pgCenter.x << ", " << pgCenter.y << ")\n";
    std::cout << "Vertices:\n";
    for(auto it = pg.getVerticesBegin(); it != pg.getVerticesEnd(); ++it) {
        std::cout << "(" << it->x << ", " << it->y << ")\n";
    }
    
    std::cout << "-------------------------\n";
    
    Point F1(0, 0);
    Point F2(4, 0);
    long double majorAxis = 6.0;
    Ellipse el(F1, F2, majorAxis);
    
    std::cout << "Ellipse:\n";
    std::cout << "Perimeter: " << el.perimeter() << "\n";
    std::cout << "Area: " << el.area() << "\n";
    Point elCenter = el.getCenter();
    std::cout << "Center: (" << elCenter.x << ", " << elCenter.y << ")\n";
    
    std::cout << "-------------------------\n";
    
    Point3D spCenter3D(0, 0, 0);
    long double radius = 5.0;
    Sphere sp(spCenter3D, radius);
    
    std::cout << "Sphere:\n";
    std::cout << "Volume: " << sp.volume() << "\n";
    std::cout << "Surface Area: " << sp.surfaceArea() << "\n";
    Point3D spCenter = sp.getCenter();
    std::cout << "Center: (" << spCenter.x << ", " << spCenter.y << ", " << spCenter.z << ")\n";
    
    std::cout << "-------------------------\n";
    
    Point3D P1(0, 0, 0);
    Point3D P2(4, 0, 0);
    Point3D P3(4, 3, 0);
    Point3D P4(0, 3, 0);
    Parallelepiped pp(P1, P2, P3, P4);
    
    std::cout << "Parallelepiped:\n";
    std::cout << "Volume: " << pp.volume() << "\n";
    std::cout << "Surface Area: " << pp.surfaceArea() << "\n";
    Point3D ppCenter = pp.getCenter();
    std::cout << "Center: (" << ppCenter.x << ", " << ppCenter.y << ", " << ppCenter.z << ")\n";
    std::cout << "Vertices:\n";
    for(auto it = pp.getVerticesBegin3D(); it != pp.getVerticesEnd3D(); ++it) {
        std::cout << "(" << it->x << ", " << it->y << ", " << it->z << ")\n";
    }
    
    return 0;
}