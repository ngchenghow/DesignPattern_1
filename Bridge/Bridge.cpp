// BridgePattern_ShapeDraw.cpp  (C++17)
#include <iostream>
#include <memory>
#include <string>

// =============== Implementor ===============
struct DrawingAPI {
    virtual ~DrawingAPI() = default;
    virtual void drawCircle(float x, float y, float radius) = 0;
    virtual void drawRectangle(float x, float y, float w, float h) = 0;
};

// ---------- Concrete Implementors ----------
class OpenGLAPI : public DrawingAPI {
public:
    void drawCircle(float x, float y, float radius) override {
        std::cout << "[OpenGL] Drawing circle at (" << x << "," << y
            << ") radius=" << radius << "\n";
    }
    void drawRectangle(float x, float y, float w, float h) override {
        std::cout << "[OpenGL] Drawing rectangle at (" << x << "," << y
            << ") w=" << w << " h=" << h << "\n";
    }
};

class DirectXAPI : public DrawingAPI {
public:
    void drawCircle(float x, float y, float radius) override {
        std::cout << "[DirectX] Render circle at (" << x << "," << y
            << ") radius=" << radius << "\n";
    }
    void drawRectangle(float x, float y, float w, float h) override {
        std::cout << "[DirectX] Render rectangle at (" << x << "," << y
            << ") w=" << w << " h=" << h << "\n";
    }
};

// =============== Abstraction ===============
class Shape {
protected:
    std::shared_ptr<DrawingAPI> api_;
public:
    Shape(std::shared_ptr<DrawingAPI> api) : api_(std::move(api)) {}
    virtual ~Shape() = default;
    virtual void draw() = 0;
    virtual void resize(float pct) = 0;
};

// ---------- Refined Abstractions ----------
class CircleShape : public Shape {
    float x_, y_, radius_;
public:
    CircleShape(float x, float y, float r, std::shared_ptr<DrawingAPI> api)
        : Shape(std::move(api)), x_(x), y_(y), radius_(r) {
    }

    void draw() override { api_->drawCircle(x_, y_, radius_); }
    void resize(float pct) override { radius_ *= pct; }
};

class RectangleShape : public Shape {
    float x_, y_, w_, h_;
public:
    RectangleShape(float x, float y, float w, float h, std::shared_ptr<DrawingAPI> api)
        : Shape(std::move(api)), x_(x), y_(y), w_(w), h_(h) {
    }

    void draw() override { api_->drawRectangle(x_, y_, w_, h_); }
    void resize(float pct) override { w_ *= pct; h_ *= pct; }
};

// =============== Client ===============
int main() {
    auto opengl = std::make_shared<OpenGLAPI>();
    auto directx = std::make_shared<DirectXAPI>();

    CircleShape circle(5, 5, 10, opengl);
    RectangleShape rect(0, 0, 6, 4, directx);

    circle.draw();
    rect.draw();

    std::cout << "Resizing circle 50% and rectangle 200%\n";
    circle.resize(0.5f);
    rect.resize(2.0f);

    circle.draw();
    rect.draw();

    return 0;
}
