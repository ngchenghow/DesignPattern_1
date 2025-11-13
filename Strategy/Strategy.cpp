#include <iostream>
#include <memory>

// 1. Strategy interface: different algorithms share this interface
class OperationStrategy
{
public:
    virtual ~OperationStrategy() = default;
    virtual int execute(int a, int b) const = 0;
};

// 2. Concrete strategies: different ways to do the operation

class AddStrategy : public OperationStrategy
{
public:
    int execute(int a, int b) const override
    {
        return a + b;
    }
};

class SubtractStrategy : public OperationStrategy
{
public:
    int execute(int a, int b) const override
    {
        return a - b;
    }
};

class MultiplyStrategy : public OperationStrategy
{
public:
    int execute(int a, int b) const override
    {
        return a * b;
    }
};

// 3. Context: uses a Strategy, but does not care which one specifically
class CalculatorContext
{
public:
    void setStrategy(std::unique_ptr<OperationStrategy> s)
    {
        strategy = std::move(s);
    }

    int doOperation(int a, int b) const
    {
        if (!strategy)
        {
            std::cout << "No strategy set!\n";
            return 0;
        }
        return strategy->execute(a, b);
    }

private:
    std::unique_ptr<OperationStrategy> strategy;
};

int main()
{
    CalculatorContext context;

    // Use AddStrategy
    context.setStrategy(std::make_unique<AddStrategy>());
    std::cout << "10 + 5 = " << context.doOperation(10, 5) << "\n";

    // Switch to SubtractStrategy
    context.setStrategy(std::make_unique<SubtractStrategy>());
    std::cout << "10 - 5 = " << context.doOperation(10, 5) << "\n";

    // Switch to MultiplyStrategy
    context.setStrategy(std::make_unique<MultiplyStrategy>());
    std::cout << "10 * 5 = " << context.doOperation(10, 5) << "\n";

    return 0;
}
