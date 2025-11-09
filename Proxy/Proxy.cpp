// Proxy_ProtectionExample.cpp (C++17 / Visual Studio 2022)
// Demonstrates the Proxy pattern (Protection Proxy)
// Proxy controls access to sensitive operations based on user role.

#include <iostream>
#include <string>
#include <memory>

// Common interface
struct IDatabase {
    virtual ~IDatabase() = default;
    virtual void ReadData() = 0;
    virtual void WriteData(const std::string& data) = 0;
};

// The real object that performs actual work
class RealDatabase final : public IDatabase {
public:
    void ReadData() override {
        std::cout << "[RealDatabase] Reading data from the database...\n";
    }

    void WriteData(const std::string& data) override {
        std::cout << "[RealDatabase] Writing '" << data << "' to the database.\n";
    }
};

// The proxy that controls access based on user role
class DatabaseProxy final : public IDatabase {
public:
    DatabaseProxy(std::string user, std::string role)
        : user_(std::move(user)), role_(std::move(role)) {
        real_db_ = std::make_unique<RealDatabase>();
    }

    void ReadData() override {
        std::cout << "[Proxy] User '" << user_ << "' requests READ access.\n";
        real_db_->ReadData();
    }

    void WriteData(const std::string& data) override {
        std::cout << "[Proxy] User '" << user_ << "' requests WRITE access.\n";
        if (role_ != "admin") {
            std::cout << "[Proxy] Access denied! Only admin can write.\n";
            return;
        }
        real_db_->WriteData(data);
    }

private:
    std::string user_;
    std::string role_;
    std::unique_ptr<RealDatabase> real_db_;
};

int main() {
    std::cout << "=== Proxy Pattern: Protection Proxy ===\n\n";

    DatabaseProxy userDB("Alice", "user");
    userDB.ReadData();
    userDB.WriteData("confidential info");

    std::cout << "\n---\n\n";

    DatabaseProxy adminDB("Bob", "admin");
    adminDB.ReadData();
    adminDB.WriteData("system settings");

    return 0;
}
