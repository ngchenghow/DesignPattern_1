// AdapterPattern_DuckTurkey.cpp
#include <iostream>
#include <memory>

// ==== Target ====
struct Duck {
    virtual ~Duck() = default;
    virtual void quack() = 0;
    virtual void fly() = 0;
};

// ==== Adaptee ====
struct Turkey {
    virtual ~Turkey() = default;
    virtual void gobble() = 0;
    virtual void shortFly() = 0; // flies only short distances
};

// ==== Concrete Target ====
struct MallardDuck : Duck {
    void quack() override { std::cout << "Mallard: Quack!\n"; }
    void fly()   override { std::cout << "Mallard: Flying far...\n"; }
};

// ==== Concrete Adaptee ====
struct WildTurkey : Turkey {
    void gobble()   override { std::cout << "Turkey: Gobble gobble!\n"; }
    void shortFly() override { std::cout << "Turkey: Short hop flight.\n"; }
};

// ==== Adapter ====
// Makes a Turkey look like a Duck (composition-based adapter)
class TurkeyAdapter : public Duck {
public:
    explicit TurkeyAdapter(std::shared_ptr<Turkey> t) : turkey_(std::move(t)) {}

    void quack() override {
        // Map Duck::quack to Turkey::gobble
        turkey_->gobble();
    }

    void fly() override {
        // Ducks fly far; turkeys only short distances.
        // Call shortFly multiple times to simulate longer flight.
        for (int i = 0; i < 3; ++i) turkey_->shortFly();
    }

private:
    std::shared_ptr<Turkey> turkey_;
};

int main() {
    std::shared_ptr<Duck> duck = std::make_shared<MallardDuck>();
    std::shared_ptr<Turkey> turkey = std::make_shared<WildTurkey>();

    // Client expects a Duck
    std::cout << "[Client uses a real Duck]\n";
    duck->quack();
    duck->fly();

    std::cout << "\n[Client uses a Turkey via Adapter]\n";
    std::shared_ptr<Duck> turkeyAsDuck = std::make_shared<TurkeyAdapter>(turkey);
    turkeyAsDuck->quack(); // actually gobble
    turkeyAsDuck->fly();   // repeated short flights

    return 0;
}
