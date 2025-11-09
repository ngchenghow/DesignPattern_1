
#include <iostream>
#include <memory>

// ----- Abstract Product Interfaces -----
class Weapon {
public:
    virtual void use() = 0;
    virtual ~Weapon() = default;
};

class Armor {
public:
    virtual void equip() = 0;
    virtual ~Armor() = default;
};

// ----- Concrete Products: Warrior -----
class Sword : public Weapon {
public:
    void use() override {
        std::cout << "Swinging a sword!\n";
    }
};

class Shield : public Armor {
public:
    void equip() override {
        std::cout << "Equipping a shield!\n";
    }
};

// ----- Concrete Products: Mage -----
class Staff : public Weapon {
public:
    void use() override {
        std::cout << "Casting a spell with the staff!\n";
    }
};

class Robe : public Armor {
public:
    void equip() override {
        std::cout << "Wearing a magic robe!\n";
    }
};

// ----- Abstract Factory -----
class CharacterFactory {
public:
    virtual std::unique_ptr<Weapon> createWeapon() = 0;
    virtual std::unique_ptr<Armor> createArmor() = 0;
    virtual ~CharacterFactory() = default;
};

// ----- Concrete Factories -----
class WarriorFactory : public CharacterFactory {
public:
    std::unique_ptr<Weapon> createWeapon() override {
        return std::make_unique<Sword>();
    }
    std::unique_ptr<Armor> createArmor() override {
        return std::make_unique<Shield>();
    }
};

class MageFactory : public CharacterFactory {
public:
    std::unique_ptr<Weapon> createWeapon() override {
        return std::make_unique<Staff>();
    }
    std::unique_ptr<Armor> createArmor() override {
        return std::make_unique<Robe>();
    }
};

// ----- Client Code -----
void enterBattle(CharacterFactory& factory) {
    auto weapon = factory.createWeapon();
    auto armor = factory.createArmor();

    armor->equip();
    weapon->use();
}

// ----- Main -----
int main() {
    std::cout << "== Warrior enters the battlefield ==\n";
    WarriorFactory warriorFactory;
    enterBattle(warriorFactory);

    std::cout << "\n== Mage enters the battlefield ==\n";
    MageFactory mageFactory;
    enterBattle(mageFactory);

    return 0;
}