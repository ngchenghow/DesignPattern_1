// DecoratorPattern_Beverage.cpp  (C++17 / VS2022)
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>

// ----- Component -----
struct Beverage {
    virtual ~Beverage() = default;
    virtual std::string description() const = 0;
    virtual double cost() const = 0;
};

// ----- Concrete Component -----
class Espresso : public Beverage {
public:
    std::string description() const override { return "Espresso"; }
    double cost() const override { return 6.00; } // RM6.00 just as an example
};

// ----- Decorator Base -----
class AddOn : public Beverage {
protected:
    std::unique_ptr<Beverage> inner_;
public:
    explicit AddOn(std::unique_ptr<Beverage> inner) : inner_(std::move(inner)) {}
};

// ----- Concrete Decorators -----
class Milk : public AddOn {
public:
    using AddOn::AddOn;
    std::string description() const override { return inner_->description() + " + Milk"; }
    double cost() const override { return inner_->cost() + 1.20; }
};

class Mocha : public AddOn {
public:
    using AddOn::AddOn;
    std::string description() const override { return inner_->description() + " + Mocha"; }
    double cost() const override { return inner_->cost() + 1.50; }
};

class Whip : public AddOn {
public:
    using AddOn::AddOn;
    std::string description() const override { return inner_->description() + " + Whip"; }
    double cost() const override { return inner_->cost() + 0.80; }
};

// ----- Demo -----
int main() {
    std::cout << std::fixed << std::setprecision(2);

    // 1) Plain espresso
    {
        std::unique_ptr<Beverage> drink = std::make_unique<Espresso>();
        std::cout << drink->description() << " = RM" << drink->cost() << "\n";
    }

    // 2) Espresso + Milk + Mocha (built step by step)
    {
        std::unique_ptr<Beverage> drink = std::make_unique<Espresso>();
        drink = std::make_unique<Milk>(std::move(drink));
        drink = std::make_unique<Mocha>(std::move(drink));
        std::cout << drink->description() << " = RM" << drink->cost() << "\n";
    }

    // 3) Nested construction: Espresso + Mocha + Whip + Milk
    {
        std::unique_ptr<Beverage> drink =
            std::make_unique<Milk>(
                std::make_unique<Whip>(
                    std::make_unique<Mocha>(
                        std::make_unique<Espresso>())));

        std::cout << drink->description() << " = RM" << drink->cost() << "\n";
    }

    return 0;
}
