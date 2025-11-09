// Prototype_Game_Spawner.cpp  (C++17)
// Build: cl /std:c++17 Prototype_Game_Spawner.cpp  OR  g++ -std=c++17 Prototype_Game_Spawner.cpp -o spawner

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <random>

// ====== Utility for IDs and RNG ======
static int NextId = 1;
static std::mt19937 rng{ 12345 };
static int rand_range(int a, int b) { std::uniform_int_distribution<int> d(a, b); return d(rng); }

// ====== Prototype base ======
class Enemy {
public:
    virtual ~Enemy() = default;
    virtual std::unique_ptr<Enemy> clone() const = 0; // key
    virtual void spawnAt(int x, int y) { x_ = x; y_ = y; id_ = NextId++; }
    virtual void info() const = 0;

protected:
    // shared attributes
    int id_ = 0;
    int x_ = 0, y_ = 0;
};

// ====== A small owned subobject to show deep copy ======
struct Brain {
    int aiLevel;
    std::string behavior;
    std::unique_ptr<Brain> clone() const { return std::make_unique<Brain>(*this); }
};

// ====== Concrete prototypes ======
class Orc : public Enemy {
public:
    Orc(int hp = 100, int atk = 12, std::vector<std::string> abilities = { "Roar" })
        : hp_(hp), atk_(atk), abilities_(std::move(abilities)), brain_(std::make_unique<Brain>(Brain{ 1,"Aggressive" })) {
    }

    Orc(const Orc& o) : hp_(o.hp_), atk_(o.atk_), abilities_(o.abilities_) {
        brain_ = o.brain_->clone(); // deep copy owned object
    }

    std::unique_ptr<Enemy> clone() const override { return std::make_unique<Orc>(*this); }

    void addAbility(std::string a) { abilities_.push_back(std::move(a)); }
    void setAI(int lvl, std::string beh) { brain_->aiLevel = lvl; brain_->behavior = std::move(beh); }

    void info() const override {
        std::cout << "Orc  #" << id_ << " @(" << x_ << "," << y_ << ") "
            << "HP=" << hp_ << " ATK=" << atk_
            << " AI(lv=" << brain_->aiLevel << "," << brain_->behavior << ") Abil=[";
        for (size_t i = 0; i < abilities_.size(); ++i) { std::cout << abilities_[i] << (i + 1 < abilities_.size() ? "," : ""); }
        std::cout << "]\n";
    }

private:
    int hp_, atk_;
    std::vector<std::string> abilities_;
    std::unique_ptr<Brain> brain_;
};

class Dragon : public Enemy {
public:
    Dragon(int hp = 300, int atk = 35, std::string element = "Fire")
        : hp_(hp), atk_(atk), element_(std::move(element)), brain_(std::make_unique<Brain>(Brain{ 3,"Territorial" })) {
    }

    Dragon(const Dragon& d) : hp_(d.hp_), atk_(d.atk_), element_(d.element_) {
        brain_ = d.brain_->clone();
    }

    std::unique_ptr<Enemy> clone() const override { return std::make_unique<Dragon>(*this); }

    void empower(int bonus) { atk_ += bonus; }
    void setElement(std::string e) { element_ = std::move(e); }

    void info() const override {
        std::cout << "Dragon#" << id_ << " @(" << x_ << "," << y_ << ") "
            << "HP=" << hp_ << " ATK=" << atk_
            << " Elem=" << element_
            << " AI(lv=" << brain_->aiLevel << "," << brain_->behavior << ")\n";
    }

private:
    int hp_, atk_;
    std::string element_;
    std::unique_ptr<Brain> brain_;
};

// ====== Prototype Registry / Spawner ======
class EnemySpawner {
public:
    template<class T, class...Args>
    void registerProto(const std::string& key, Args&&...args) {
        protos_[key] = std::unique_ptr<Enemy>(new T(std::forward<Args>(args)...));
    }

    std::unique_ptr<Enemy> spawn(const std::string& key, int x, int y) const {
        auto it = protos_.find(key);
        if (it == protos_.end()) throw std::runtime_error("no prototype: " + key);
        auto e = it->second->clone();
        e->spawnAt(x, y);
        return e;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Enemy>> protos_;
};

// ====== Demo ======
int main() {
    EnemySpawner spawner;

    // Register base enemy prototypes (expensive setup could happen here once)
    spawner.registerProto<Orc>("orc-basic", 120, 15, std::vector<std::string>{"Roar", "Charge"});
    spawner.registerProto<Dragon>("dragon-fire", 350, 40, "Fire");

    // Spawn a wave using clones
    std::vector<std::unique_ptr<Enemy>> wave;
    for (int i = 0; i < 3; ++i) {
        auto o = spawner.spawn("orc-basic", rand_range(-5, 5), rand_range(-5, 5));
        // Customize one of them to show clone independence
        if (i == 1) static_cast<Orc*>(o.get())->addAbility("Berserk");
        wave.push_back(std::move(o));
    }

    auto d1 = spawner.spawn("dragon-fire", 10, 2);
    static_cast<Dragon*>(d1.get())->empower(15); // modify the clone, not the prototype
    wave.push_back(std::move(d1));

    // Show originals unaffected by edits: get fresh copies and print
    auto protoOrc = spawner.spawn("orc-basic", 0, 0); // fresh clone of prototype defaults
    auto protoDragon = spawner.spawn("dragon-fire", 0, 0);

    std::cout << "=== Spawned Wave ===\n";
    for (auto& e : wave) e->info();

    std::cout << "\n=== Fresh clones from unchanged prototypes ===\n";
    protoOrc->info();
    protoDragon->info();

    return 0;
}
