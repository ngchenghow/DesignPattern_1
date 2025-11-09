
#include <iostream>
#include <memory>
using namespace std;

// 抽象产品
class Product {
public:
    virtual void show() = 0;
    virtual ~Product() = default;
};

// 具体产品 A
class ProductA : public Product {
public:
    void show() override { cout << "This is Product A\n"; }
};

// 具体产品 B
class ProductB : public Product {
public:
    void show() override { cout << "This is Product B\n"; }
};

// 工厂接口
class Factory {
public:
    virtual unique_ptr<Product> createProduct() = 0;
    virtual ~Factory() = default;
};

// 具体工厂 A
class FactoryA : public Factory {
public:
    unique_ptr<Product> createProduct() override {
        return make_unique<ProductA>();
    }
};

// 具体工厂 B
class FactoryB : public Factory {
public:
    unique_ptr<Product> createProduct() override {
        return make_unique<ProductB>();
    }
};

// 客户端代码
int main() {
    unique_ptr<Factory> factoryA = make_unique<FactoryA>();
    auto productA = factoryA->createProduct();
    productA->show();

    unique_ptr<Factory> factoryB = make_unique<FactoryB>();
    auto productB = factoryB->createProduct();
    productB->show();

    return 0;
}
