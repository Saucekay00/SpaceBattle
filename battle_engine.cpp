#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <memory>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>


enum class CrewType {Pilot, Gunner, TorpedoHandler};
enum class WeaponType {LightCanon, Torpedo};
enum class Faction {Zapezoid, Rogoatuskan};

struct Weapon {
    WeaponType type{};
    int damage {0};
    std::string name;
    bool hasOperator{false}; 

    // allow name and hasOperator to have defaults so callers can provide
    // just type and damage when creating simple weapons
    Weapon(WeaponType t, int d, std::string n = "", bool op = false)
        : type(t), damage(d), name(std::move(n)), hasOperator(op) {}
};

class CrewMember {
    protected:
    CrewType type;
    std::string name;
    std::string id;

public:
    CrewMember(CrewType t, std::string n, std::string i)
        : type(t), name(std::move(n)), id(std::move(i)) {}

    virtual ~CrewMember() = default;

    std::string getName() const {
        return name;
    }
    std::string getId() const {
        return id;
    }
    CrewType getType() const {
        return type;
    }
};

class Pilot : public CrewMember {
public:
    Pilot(std::string name, std::string id) 
        : CrewMember (CrewType::Pilot, std::move(name), std::move(id)) {}
};

class Gunner : public CrewMember {
public:
    Gunner(std::string name, std::string id) 
        : CrewMember (CrewType::Gunner, std::move(name), std::move(id)) {}
};

class TorpedoHandler : public CrewMember {
public:
    TorpedoHandler(std::string name, std::string id) 
        : CrewMember (CrewType::TorpedoHandler, std::move(name), std::move(id)) {}
};


class Ship {

protected:
    std::string name;
    std::string id;
    std::vector<Weapon> weapons;
    std::vector<CrewMember*> crew;
    int hp;
    int maxHp;
    int requiredPilots;
    int assignedPilots;

public:

    Ship(std::string n, std::string i, int hp, int reqPilots)
        : id(i), name(n), hp(hp), maxHp(hp), 
          requiredPilots(reqPilots), assignedPilots(0) {}

    virtual ~Ship() = default;

    virtual std::string label() const = 0;
    virtual std::string getTypeName() const = 0;
    virtual double getHitChanceLightCanon() const = 0;
    virtual double getHitChanceTorpedo() const = 0;
    virtual Faction getFaction() const = 0;

    std::string getID() const {
        return id;
    }
    std::string getName() const {
        return name;
    }
    int getHP() const {
        return hp;
    }
    bool isAlive() const {
        return hp > 0;
    }
    void assignCrewMember(CrewMember* cm) {
        crew.push_back(cm);
        if (cm->getType() == CrewType::Pilot) {
            assignedPilots++;
        }
    }

    bool canFly() const {
        return assignedPilots > 0;
    }

    double getAdjustedHitChance(WeaponType wt) const {
        double baseChance = 0.0;
        switch (wt) {
            case WeaponType::LightCanon:
                baseChance = getHitChanceLightCanon();
                break;
            case WeaponType::Torpedo:
                baseChance = getHitChanceTorpedo();
                break;
        }

        if (assignedPilots >= requiredPilots) {
            return baseChance;
        } else {
            double penalty = 0.1 * (requiredPilots - assignedPilots);
            return std::max(0.0, baseChance - penalty);
        }
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) hp = 0;
    }

    std::vector<Weapon> getActiveWeapons() const {
        std::vector<Weapon> activeWeapons;
        for (const auto& weapon : weapons) {
            if (weapon.hasOperator) {
                activeWeapons.push_back(weapon);
            }
        }
        return activeWeapons;
    }

    std::string getLabel() const {
        return getFaction() == Faction::Zapezoid ? "Zapezoid " + name : "Rogoatuskan " + name;
    }

    void assignWeaponOperators() {
        int gunnersAssigned = 0;
        int torpedoHandlersAssigned = 0;

        for (const auto& cm : crew) {
            if (cm->getType() == CrewType::Gunner) {
                gunnersAssigned++;
            } else if (cm->getType() == CrewType::TorpedoHandler) {
                torpedoHandlersAssigned++;
            }
        }

        for (auto& weapon : weapons) {
            if (weapon.type == WeaponType::LightCanon && gunnersAssigned > 0) {
                weapon.hasOperator = true;
                gunnersAssigned--;
            } else if (weapon.type == WeaponType::Torpedo && torpedoHandlersAssigned > 0) {
                weapon.hasOperator = true;
                torpedoHandlersAssigned--;
            } else {
                weapon.hasOperator = false;
            }
        }
    }
};

//Zapezoids i swear i cant pronounce any of these ships right

class Guerriero : public Ship {
public:
    Guerriero(std::string name, std::string id) : Ship(id, name, 123, 1) {
        weapons.push_back(Weapon(WeaponType::LightCanon, 96));
    }

    std::string getTypeName() const override {
        return "Guerriero";
    }
    double getHitChanceLightCanon() const override {
        return 0.26;
    }
    double getHitChanceTorpedo() const override {
        return 0.6;
    }
    Faction getFaction() const override {
        return Faction::Zapezoid;
    }
};

class Medio : public Ship {
public:
    Medio(std::string name, std::string id) : Ship(id, name, 214, 1) {
        weapons.push_back(Weapon(WeaponType::LightCanon, 134));
        weapons.push_back(Weapon(WeaponType::LightCanon, 134));
    }
    std::string getTypeName() const override {
        return "Medio";
    }
    double getHitChanceLightCanon() const override {
        return 0.31;
    }
    double getHitChanceTorpedo() const override {
        return 0.11; 
    }
    Faction getFaction() const override {
        return Faction::Zapezoid;
    }
};

class Corazzata : public Ship {
public:
    Corazzata(std::string name, std::string id) : Ship(id, name, 1031, 2) {
        for (int i = 0; i < 10; ++i) {
            weapons.push_back(Weapon(WeaponType::LightCanon, 164));
        }
        for (int i = 0; i < 4; ++i) {
            weapons.push_back(Weapon(WeaponType::Torpedo, 293));
        }
    }

    std::string getTypeName() const override {
        return "Corazzata";
    }
    double getHitChanceLightCanon() const override {
        return 0.50;
    }
    double getHitChanceTorpedo() const override {
        return 0.25;
    }
    Faction getFaction() const override {
        return Faction::Zapezoid;
    }
};

//Rogoatuskan i'm damn hell sure there are german built 

class Jager : public Ship {
public:
    Jager(std::string name, std::string id) : Ship(id, name, 112, 1) {
        weapons.push_back(Weapon(WeaponType::LightCanon, 101));
    }

    std::string getTypeName() const override {
        return "Jager";
    }
    double getHitChanceLightCanon() const override {
        return 0.24;
    }
    double getHitChanceTorpedo() const override {
        return 0.05;
    }
    Faction getFaction() const override {
        return Faction::Rogoatuskan;
    }
};
class Kreuzer : public Ship {
public:
    Kreuzer(std::string name, std::string id) : Ship(id, name, 212, 1) {
        weapons.push_back(Weapon(WeaponType::LightCanon, 132));
        weapons.push_back(Weapon(WeaponType::LightCanon, 132));
    }
    std::string getTypeName() const override {
        return "Kreuzer";
    }
    double getHitChanceLightCanon() const override {
        return 0.29;
    }
    double getHitChanceTorpedo() const override {
        return 0.10;
    }
    Faction getFaction() const override {
        return Faction::Rogoatuskan;
    }
};
class Fregatte : public Ship {
public:
    Fregatte(std::string name, std::string id) : Ship(id, name, 1143, 2) {
        for (int i = 0; i < 11; ++i) {
            weapons.push_back(Weapon(WeaponType::LightCanon, 159));
        }
        for (int i = 0; i < 5; ++i) {
            weapons.push_back(Weapon(WeaponType::Torpedo, 282));
        }
    }

    std::string getTypeName() const override {
        return "Fregatte";
    }
    double getHitChanceLightCanon() const override {
        return 0.60;
    }
    double getHitChanceTorpedo() const override {
        return 0.30;
    }
    Faction getFaction() const override {
        return Faction::Rogoatuskan;
    }
};

//Battle simulation engine

class BattleSimulator {
private:
    std::vector<Ship*> zapezoidFleet;
    std::vector<Ship*> rogoatuskanFleet;
    std::mt19937 rng;

public:
    BattleSimulator(std::vector<Ship*> zFleet, std::vector<Ship*> rFleet)
        : zapezoidFleet(std::move(zFleet)), rogoatuskanFleet(std::move(rFleet)) {
        rng.seed(static_cast<unsigned int>(std::time(nullptr)));
    }

    void runBattle() { 
        int round = 1;

        std::cout << "Battle Start!\n";

        while (hasLivingShips(zapezoidFleet) && hasLivingShips(rogoatuskanFleet)) {
            std::cout << "\n--- Round " << round << " ---\n";

            simulateRound();

            round++;
        }

        declareWinner();
    }

private:
    bool hasLivingShips(const std::vector<Ship*>& fleet) {
        for (auto* ship : fleet) {
            if (ship->isAlive() && ship->canFly()) {
                return true;
            }
        }
        return false;
    }
    void simulateRound() {
        std::vector<std::tuple<Ship*, Ship*, Weapon, bool>> attacks;

        for (auto* attacker : zapezoidFleet) {
            if (attacker->isAlive() && attacker->canFly()) continue;

            auto weapons = attacker->getActiveWeapons();
            for (const auto& weapon : weapons) {
                Ship* target = selectRandomTarget(rogoatuskanFleet);
                if (target) {
                    bool hits = rollHit(target, weapon.type);
                    attacks.push_back({attacker, target, weapon, hits});
                }
            }
        }

        for (auto* attacker : rogoatuskanFleet) {
            if (attacker->isAlive() && attacker->canFly()) continue;

            auto weapons = attacker->getActiveWeapons();
            for (const auto& weapon : weapons) {
                Ship* target = selectRandomTarget(zapezoidFleet);
                if (target) {
                    bool hits = rollHit(target, weapon.type);
                    attacks.push_back({attacker, target, weapon, hits});
                }
            }
        }
        
        for (const auto& [attacker, target, weapon, hits] : attacks) {
            std::string weaponName = (weapon.type == WeaponType::LightCanon) ? "Light Canon" : "Torpedo";

            std::cout << attacker->getLabel() << " fires " << weaponName 
                      << " at " << target->getLabel() << ". ";

            if (hits) {
                target->takeDamage(weapon.damage);
                std::cout << "Hit! " << target->getLabel() << " takes " << weapon.damage 
                          << " damage. (HP left: " << target->getHP() << ")\n";

                if (!target->isAlive()) {
                    std::cout << target->getLabel() << " has been destroyed!\n";
                }
            } else {
                std::cout << "Missed!\n";
            }

            std::cout << std::endl;
        }
    }

    Ship* selectRandomTarget(const std::vector<Ship*>& fleet) {
        std::vector<Ship*> aliveShips;
        for (auto* ship : fleet) {
            if (ship->isAlive() && ship->canFly()) {
                aliveShips.push_back(ship);
            }
        }

        if (aliveShips.empty()) {
            return nullptr;
        }

        std::uniform_int_distribution<std::size_t> dist(0, aliveShips.size() - 1);
        return aliveShips[dist(rng)];
    }

    bool rollHit(Ship* target, WeaponType wt) {
        double hitChance = target->getAdjustedHitChance(wt);
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        double roll = dist(rng);
        return roll < hitChance;
    }

    void declareWinner() {
        bool zapezoidsAlive = hasLivingShips(zapezoidFleet);
        bool rogoatuskansAlive = hasLivingShips(rogoatuskanFleet);

        std::cout << "\n--- Battle Over ---\n";
        if (zapezoidsAlive && !rogoatuskansAlive) {
            std::cout << "Zapezoid Fleet Wins!\n";
        } else if (!zapezoidsAlive && rogoatuskansAlive) {
            std::cout << "Rogoatuskan Fleet Wins!\n";
        } else {
            std::cout << "It's a Draw!\n";
        }
    }

    void printSurvivors(const std::vector<Ship*>& fleet) {
        for (const auto* ship : fleet) {
            if (ship->isAlive()) {
                std::cout << ship->getLabel() << " survived with " << ship->getHP() << " HP left.\n";
            }
        } 
    }
};

int main(int argc, char* argv[]) {
    if {argc != 5} {
        std::cerr << "Usage: " << argv[0] << " <zShips.csv> <zCrew.csv> <rShips.csv> <rCrew.csv>\n";
        return 1;
    }

    auto zapezoidShips = loadShips(argv[1], Faction::Zapezoid);
    auto zapezoidCrew = loadCrew(argv[2]);
    auto rogoatuskanShips = loadShips(argv[3], Faction::Rogoatuskan);
    auto rogoatuskanCrew = loadCrew(argv[4]);

    std::cout <<"Loaded " << zapezoidShips.size() << " Zapezoid ships and " 
              << zapezoidCrew.size() << " crew members.\n";
    std::cout <<"Loaded " << rogoatuskanShips.size() << " Rogoatuskan ships and " 
              << rogoatuskanCrew.size() << " crew members.\n";

    assignCrewToShips(zapezoidShips, zapezoidCrew);
    assignCrewToShips(rogoatuskanShips, rogoatuskanCrew);

    BattleSimulator simulator(zapezoidShips, rogoatuskanShips);
    simulator.runBattle();

    for (auto* ship : zapezoidShips) {
        delete ship;
    }
    for (auto* ship : rogoatuskanShips) {
        delete ship;
    }
    for (auto* cm : zapezoidCrew) {
        delete cm;
    }
    for (auto* cm : rogoatuskanCrew) {
        delete cm;
    }

    return 0;
}