// BuilderPattern_Demo.cpp (C++17)
// Compile: g++ BuilderPattern_Demo.cpp -std=gnu++17 -O2 (or VS2022)

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <utility>

// ========== Product ==========
struct PC {
    std::string cpu;
    int ramGB = 0;
    std::string gpu;
    std::vector<std::string> storages; // e.g., "1TB NVMe", "2TB HDD"
    bool wifi = false;
    bool bluetooth = false;

    void Print() const {
        std::cout << "PC{ cpu=" << cpu
            << ", ram=" << ramGB << "GB"
            << ", gpu=" << gpu
            << ", wifi=" << (wifi ? "yes" : "no")
            << ", bt=" << (bluetooth ? "yes" : "no")
            << ", storages=[";
        for (size_t i = 0; i < storages.size(); ++i) {
            std::cout << storages[i] << (i + 1 < storages.size() ? ", " : "");
        }
        std::cout << "] }\n";
    }
};

// ========== Builder Interface ==========
class IPCBuilder {
public:
    virtual ~IPCBuilder() = default;
    virtual IPCBuilder& SetCPU(std::string v) = 0;
    virtual IPCBuilder& SetRAM(int gb) = 0;
    virtual IPCBuilder& SetGPU(std::string v) = 0;
    virtual IPCBuilder& AddStorage(std::string v) = 0;
    virtual IPCBuilder& EnableWiFi(bool on = true) = 0;
    virtual IPCBuilder& EnableBluetooth(bool on = true) = 0;
    virtual PC Build() = 0; // may validate
};

// ========== Concrete Builder (classic) ==========
class PCBuilder : public IPCBuilder {
    PC pc_;
public:
    IPCBuilder& SetCPU(std::string v) override { pc_.cpu = std::move(v); return *this; }
    IPCBuilder& SetRAM(int gb) override { pc_.ramGB = gb; return *this; }
    IPCBuilder& SetGPU(std::string v) override { pc_.gpu = std::move(v); return *this; }
    IPCBuilder& AddStorage(std::string v) override { pc_.storages.push_back(std::move(v)); return *this; }
    IPCBuilder& EnableWiFi(bool on = true) override { pc_.wifi = on; return *this; }
    IPCBuilder& EnableBluetooth(bool on = true) override { pc_.bluetooth = on; return *this; }

    PC Build() override {
        // simple validation
        if (pc_.cpu.empty()) throw std::runtime_error("CPU is required");
        if (pc_.ramGB <= 0)  throw std::runtime_error("RAM must be > 0");
        if (pc_.storages.empty()) throw std::runtime_error("At least one storage required");
        // return by value (NRVO/move)
        return pc_;
    }
    // Reset to reuse the builder (optional)
    PCBuilder& Reset() { pc_ = PC{}; return *this; }
};

// ========== Director (optional helper) ==========
class PCDirector {
public:
    static PC MakeGamingRig(IPCBuilder& b) {
        return b.SetCPU("AMD Ryzen 7 7800X3D")
            .SetRAM(32)
            .SetGPU("NVIDIA RTX 4070 Ti")
            .AddStorage("1TB NVMe")
            .AddStorage("2TB SSD")
            .EnableWiFi()
            .EnableBluetooth()
            .Build();
    }

    static PC MakeOfficeBox(IPCBuilder& b) {
        return b.SetCPU("Intel i5-13400")
            .SetRAM(16)
            .SetGPU("Integrated")
            .AddStorage("512GB NVMe")
            .EnableWiFi(false)
            .EnableBluetooth(true)
            .Build();
    }
};

// ========== Fluent "self-directed" builder (no Director) ==========
class PCFluentBuilder {
    PC pc_;
public:
    PCFluentBuilder& cpu(std::string v) { pc_.cpu = std::move(v); return *this; }
    PCFluentBuilder& ram(int gb) { pc_.ramGB = gb; return *this; }
    PCFluentBuilder& gpu(std::string v) { pc_.gpu = std::move(v); return *this; }
    PCFluentBuilder& storage(std::string v) { pc_.storages.push_back(std::move(v)); return *this; }
    PCFluentBuilder& wifi(bool on = true) { pc_.wifi = on; return *this; }
    PCFluentBuilder& bt(bool on = true) { pc_.bluetooth = on; return *this; }

    PC build() {
        if (pc_.cpu.empty()) throw std::runtime_error("CPU is required");
        if (pc_.ramGB <= 0)  throw std::runtime_error("RAM must be > 0");
        if (pc_.storages.empty()) throw std::runtime_error("At least one storage required");
        return pc_;
    }
};

int main() {
    try {
        // --- Classic: Director + Builder ---
        PCBuilder builder;
        PC gaming = PCDirector::MakeGamingRig(builder);
        gaming.Print();

        builder.Reset(); // reuse
        PC office = PCDirector::MakeOfficeBox(builder);
        office.Print();

        // --- Fluent: build directly without a Director ---
        PC silentWorkstation = PCFluentBuilder{}
            .cpu("Intel i7-14700")
            .ram(64)
            .gpu("NVIDIA RTX A2000")
            .storage("2TB NVMe")
            .wifi(true)
            .bt(true)
            .build();
        silentWorkstation.Print();

    }
    catch (const std::exception& ex) {
        std::cerr << "Build error: " << ex.what() << "\n";
    }
    return 0;
}
