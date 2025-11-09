// ChainOfResponsibility_Example.cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <iomanip>

struct Request {
    double amount{};
    std::string description;
};

// ------------ Handler Base ------------
class Handler {
public:
    virtual ~Handler() = default;

    // Build the chain: returns the raw pointer of the newly attached next
    Handler* setNext(std::unique_ptr<Handler> next) {
        next_ = std::move(next);
        return next_.get();
    }

    // Entry point to handle; if current can't, it forwards to next
    void handle(const Request& r) {
        if (process(r)) return;
        if (next_) {
            next_->handle(r);
        }
        else {
            std::cout << "[REJECTED]  No approver for: " << r.description
                << " (RM " << r.amount << ")\n";
        }
    }

protected:
    virtual bool process(const Request& r) = 0;

private:
    std::unique_ptr<Handler> next_;
};

// ------------ Concrete Handlers ------------
class TeamLead final : public Handler {
protected:
    bool process(const Request& r) override {
        if (r.amount <= 1000.0) {
            std::cout << "[APPROVED]  TeamLead approved: " << r.description
                << " (RM " << r.amount << ")\n";
            return true;
        }
        // can't handle; pass on
        std::cout << "[PASS]      TeamLead passes: " << r.description
            << " (RM " << r.amount << ")\n";
        return false;
    }
};

class Manager final : public Handler {
protected:
    bool process(const Request& r) override {
        if (r.amount <= 5000.0) {
            std::cout << "[APPROVED]  Manager approved: " << r.description
                << " (RM " << r.amount << ")\n";
            return true;
        }
        std::cout << "[PASS]      Manager passes: " << r.description
            << " (RM " << r.amount << ")\n";
        return false;
    }
};

class Director final : public Handler {
protected:
    bool process(const Request& r) override {
        if (r.amount <= 50000.0) {
            std::cout << "[APPROVED]  Director approved: " << r.description
                << " (RM " << r.amount << ")\n";
            return true;
        }
        std::cout << "[PASS]      Director passes: " << r.description
            << " (RM " << r.amount << ")\n";
        return false;
    }
};

class CEO final : public Handler {
protected:
    bool process(const Request& r) override {
        // Top of chain; approves anything above others' limits up to a policy cap
        if (r.amount <= 200000.0) {
            std::cout << "[APPROVED]  CEO approved: " << r.description
                << " (RM " << r.amount << ")\n";
            return true;
        }
        std::cout << "[PASS]      CEO cannot approve per policy cap: " << r.description
            << " (RM " << r.amount << ")\n";
        return false; // end of chain; will be rejected by base handle()
    }
};

// ------------ Demo ------------
int main() {
    // Build chain: TeamLead -> Manager -> Director -> CEO
    auto chain = std::make_unique<TeamLead>();
    Handler* tail = chain.get();
    tail = tail->setNext(std::make_unique<Manager>());
    tail = tail->setNext(std::make_unique<Director>());
    tail = tail->setNext(std::make_unique<CEO>());

    std::vector<Request> requests = {
        {  120.0,  "Stationery purchase" },
        {  999.9,  "Team lunch" },
        { 1800.0,  "New dev laptop (deposit)" },
        { 4200.0,  "Conference fees" },
        { 22000.0, "Small server upgrade" },
        {120000.0, "Data center annual contract" },
        {350000.0, "New office renovation" } // beyond policy cap -> reject
    };

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& r : requests) {
        chain->handle(r);
    }
    return 0;
}
