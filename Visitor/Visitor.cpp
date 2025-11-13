#include <iostream>
#include <vector>
#include <memory>

// Forward declarations of element types
class Circle;
class Rectangle;

// ---------------- Visitor Interface ----------------
class Visitor
{
public:
    virtual ~Visitor() = default;
    virtual void visitCircle(Circle& c) = 0;
    virtual void visitRectangle(Rectangle& r) = 0;
};

// ---------------- Element Interface ----------------
class Shape
{
public:
    virtual ~Shape() = default;
    // The key of Visitor pattern: accept(Visitor&)
    virtual void accept(Visitor& v) = 0;
};

// ---------------- Concrete Elements ----------------
class Circle : public Shape
{
public:
    explicit Circle(double radius) : radius_(radius) {}

    double radius() const { return radius_; }

    void accept(Visitor& v) override
    {
        // Double dispatch: calls Visitor::visitCircle
        v.visitCircle(*this);
    }

private:
    double radius_;
};

class Rectangle : public Shape
{
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    double width() const { return width_; }
    double height() const { return height_; }

    void accept(Visitor& v) override
    {
        // Double dispatch: calls Visitor::visitRectangle
        v.visitRectangle(*this);
    }

private:
    double width_;
    double height_;
};

// ---------------- Concrete Visitors ----------------
class AreaVisitor : public Visitor
{
public:
    void visitCircle(Circle& c) override
    {
        double area = 3.14159 * c.radius() * c.radius();
        std::cout << "Circle area = " << area << "\n";
    }

    void visitRectangle(Rectangle& r) override
    {
        double area = r.width() * r.height();
        std::cout << "Rectangle area = " << area << "\n";
    }
};

class DrawVisitor : public Visitor
{
public:
    void visitCircle(Circle& c) override
    {
        std::cout << "Draw circle (r = " << c.radius() << ")\n";
    }

    void visitRectangle(Rectangle& r) override
    {
        std::cout << "Draw rectangle ("
            << r.width() << " x " << r.height() << ")\n";
    }
};

// ---------------- Client Code (main) ----------------
int main()
{
    // Create some shapes
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<Circle>(5.0));

    // Create visitors
    AreaVisitor areaVisitor;
    DrawVisitor drawVisitor;

    // Use the same shapes with different visitors
    std::cout << "=== Areas ===\n";
    for (auto& s : shapes)
    {
        s->accept(areaVisitor);
    }

    std::cout << "\n=== Drawing ===\n";
    for (auto& s : shapes)
    {
        s->accept(drawVisitor);
    }

    return 0;
}
