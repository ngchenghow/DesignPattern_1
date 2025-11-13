#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// ==================== Observer Interface ====================
class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(int newState) = 0;
};

// ==================== Subject (Observable) ====================
class Subject
{
public:
    void attach(Observer* obs)
    {
        observers.push_back(obs);
    }

    void detach(Observer* obs)
    {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }

    void setState(int value)
    {
        state = value;
        notify();
    }

    int getState() const
    {
        return state;
    }

private:
    void notify()
    {
        for (Observer* obs : observers)
        {
            obs->update(state);
        }
    }

private:
    int state = 0;
    std::vector<Observer*> observers;
};

// ==================== Concrete Observers ====================
class ConsoleObserver : public Observer
{
public:
    ConsoleObserver(const std::string& name) : name(name) {}

    void update(int newState) override
    {
        std::cout << "[ConsoleObserver " << name
            << "] received new state: " << newState << "\n";
    }

private:
    std::string name;
};

class DoubleValueObserver : public Observer
{
public:
    DoubleValueObserver(const std::string& name) : name(name) {}

    void update(int newState) override
    {
        std::cout << "[DoubleValueObserver " << name
            << "] state * 2 = " << (newState * 2) << "\n";
    }

private:
    std::string name;
};

// ==================== Demo ====================
int main()
{
    Subject subject;

    ConsoleObserver   obs1("A");
    ConsoleObserver   obs2("B");
    DoubleValueObserver obs3("C");

    // Attach observers
    subject.attach(&obs1);
    subject.attach(&obs2);
    subject.attach(&obs3);

    std::cout << "== Set state = 10 ==\n";
    subject.setState(10);

    std::cout << "\nDetach observer B\n";
    subject.detach(&obs2);

    std::cout << "== Set state = 20 ==\n";
    subject.setState(20);

    return 0;
}
