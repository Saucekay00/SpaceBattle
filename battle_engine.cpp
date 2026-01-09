#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>

enum class WeaponType {LightCanon, Torpedo};

struct Weapon {
    WeaponType type{};
    int damage {0};
    std::string name;
};

class Ship {
public:

    virtual ~Ship() = default;
    
    virtual std::string label() const = 0;
    virtual int isAlive() const = 0;
    virtual bool canActThisRound() const = 0;

    virtual std::vector<Weapon> getActiveWeapons() const = 0;
    virtual double chanceToBeHit(WeaponType wt) const = 0;
    virtual int hp() const = 0; 
    virtual void applyDamage(int damage) = 0;
};

class DummyShip : public Ship {
    std::string name;
    int hp;
    double hitLightCanon;
    double hitTorpedo;
    std::vector<Weapon> weapons;

public:
    DummyShip(std::string n, int h, double hlc, double ht, std::vector<Weapon> weapons)
        : name(std::move(n)), hp(h), hitLightCanon(hlc), hitTorpedo(ht), weapons(std::move(weapons)) {}

    std::string label() const override {
        return name;
    }

    int isAlive() const override {
        return hp > 0;
    }

    bool canActThisRound() const override {
        return isAlive();
    }

    std::vector<Weapon> getActiveWeapons() const override {
        return weapons;
    }

    double chanceToBeHit(WeaponType wt) const override {
        switch (wt) {
            case WeaponType::LightCanon:
                return hitLightCanon;
            case WeaponType::Torpedo:
                return hitTorpedo;
            default:
                return 0.0;
        }
    }

    int hp() const override {
        return hp;
    }

    void applyDamage(int damage) override {
        hp -= damage;
        if (hp < 0) hp = 0;
    }
};

