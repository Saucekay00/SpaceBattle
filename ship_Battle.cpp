#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Person
{
private:
    string personID;
    string name;
    string role;

public:
    Person(string id, string personName, string PersonRole)
        : personID(id), name(personName), role(PersonRole) {}

    string getID() const { return personID; }
    string getName() const { return name; }
    string getRole() const { return role; }
};

class Ship
{
protected:
    string shipID;
    string name;
    int hitPoints;
    int maxHitPoints;
    int maxPilots;
    int maxGunners;
    int maxTorpedoHandlers;

    // Weapons
    int lightCannonCount;
    int lightCannonPower;
    int torpedoCount;
    int torpedoPower;

    // Crew
    vector<Person *> pilots;
    vector<Person *> gunners;
    vector<Person *> torpedoHandlers;

public:
    Ship(string id, string shipName, int maxHP)
        : shipID(id), name(shipName),
          hitPoints(maxHP), maxHitPoints(maxHP),
          maxPilots(0), maxGunners(0), maxTorpedoHandlers(0),
          lightCannonCount(0), lightCannonPower(0),
          torpedoCount(0), torpedoPower(0)
    {
    }

    virtual ~Ship() {}

    bool addPilot(Person *p)
    {
        if (pilots.size() < maxPilots)
        {
            pilots.push_back(p);
            return true;
        }
        return false;
    }

    bool addGunner(Person *p)
    {
        if (gunners.size() < maxGunners)
        {
            gunners.push_back(p);
            return true;
        }
        return false;
    }

    bool addTorpedoHandler(Person *p)
    {
        if (torpedoHandlers.size() < maxTorpedoHandlers)
        {
            torpedoHandlers.push_back(p);
            return true;
        }
        return false;
    }

    void takeDamage(int damage)
    {
        hitPoints = hitPoints - damage;
        if (hitPoints < 0)
            hitPoints = 0;
    }

    bool isDestroyed() const
    {
        return hitPoints <= 0;
    }

    bool isAlive() const
    {
        return hitPoints > 0;
    }

    void printHP() const
    {
        cout << "Ship HP:" << hitPoints << "/" << maxHitPoints << endl;
    }

    bool canFly() const
    {
        return !pilots.empty();
    }

    int getPilotCount() const
    {
        return pilots.size();
    }

    int getActiveLightCannons() const
    {
        int gunnerCount = gunners.size();
        if (gunnerCount < lightCannonCount)
            return gunnerCount;
        return lightCannonCount;
    }

    int getActiveTorpedos() const
    {
        int handlerCount = torpedoHandlers.size();
        if (handlerCount < torpedoCount)
            return handlerCount;
        return torpedoCount;
    }

    int getLightShots() const
    {
        return getActiveLightCannons();
    }

    int getTorpedoShots() const
    {
        return getActiveTorpedos();
    }

    int getLightCannonPower() const
    {
        return lightCannonPower;
    }

    int getTorpedoPower() const
    {
        return torpedoPower;
    }

    double applyPilotPenalty(double baseChance) const
    {
        if (maxPilots > 1 && pilots.size() < maxPilots)
        {
            return 1.0 - ((1.0 - baseChance) * 0.75);
        }
        return baseChance;
    }

    double getAdjustedLightHitChance() const
    {
        return applyPilotPenalty(getLightHitChance());
    }

    double getAdjustedTorpedoHitChance() const
    {
        return applyPilotPenalty(getTorpedoHitChance());
    }

    string getShipID() const
    {
        return shipID;
    }

    string getName() const
    {
        return name;
    }

    int getHitPoints() const
    {
        return hitPoints;
    }

    int getHP() const
    {
        return hitPoints;
    }

    int getMaxHitPoints() const
    {
        return maxHitPoints;
    }

    virtual double getLightHitChance() const = 0;
    virtual double getTorpedoHitChance() const = 0;
    virtual string getShipType() const = 0;
    virtual string getFaction() const = 0;

    // Method needed for battle system
    string getLabel() const
    {
        return getFaction() + " " + name;
    }

    bool operator<(const Ship &other) const
    {
        return hitPoints < other.hitPoints;
    }

    struct SimpleWeapon
    {
        bool isLightCannon;
        int damage;
    };

    vector<SimpleWeapon> getActiveWeapons() const
    {
        vector<SimpleWeapon> weapons;

        // Add active light cannons
        int activeLightCannons = getActiveLightCannons();
        for (int i = 0; i < activeLightCannons; ++i)
        {
            weapons.push_back({true, lightCannonPower});
        }

        // Add active torpedoes
        int activeTorpedoes = getActiveTorpedos();
        for (int i = 0; i < activeTorpedoes; ++i)
        {
            weapons.push_back({false, torpedoPower});
        }

        return weapons;
    }

    double getAdjustedHitChance(bool isLightCannon) const
    {
        if (isLightCannon)
        {
            return getAdjustedLightHitChance();
        }
        else
        {
            return getAdjustedTorpedoHitChance();
        }
    }
};

// ZAPEZOID SHIPS

class Guerriero : public Ship
{
public:
    Guerriero(string id, string shipName) : Ship(id, shipName, 123)
    {
        maxPilots = 1;
        maxGunners = 1;
        lightCannonCount = 1;
        lightCannonPower = 96;
        maxTorpedoHandlers = 0;
        torpedoCount = 0;
        torpedoPower = 0;
    }

    double getLightHitChance() const override
    {
        return 0.26;
    }

    double getTorpedoHitChance() const override
    {
        return 0.06;
    }

    string getShipType() const override
    {
        return "Guerriero";
    }

    string getFaction() const override
    {
        return "Zapezoid";
    }
};

class Medio : public Ship
{
public:
    Medio(string id, string shipName) : Ship(id, shipName, 214)
    {
        maxPilots = 1;
        maxGunners = 2;
        lightCannonCount = 2;
        lightCannonPower = 134;
        maxTorpedoHandlers = 0;
        torpedoCount = 0;
        torpedoPower = 0;
    }

    double getLightHitChance() const override
    {
        return 0.31;
    }

    double getTorpedoHitChance() const override
    {
        return 0.11;
    }

    string getShipType() const override
    {
        return "Medio";
    }

    string getFaction() const override
    {
        return "Zapezoid";
    }
};

class Corazzata : public Ship
{
public:
    Corazzata(string id, string shipName) : Ship(id, shipName, 1031)
    {
        maxPilots = 2;
        maxGunners = 10;
        lightCannonCount = 10;
        lightCannonPower = 164;
        maxTorpedoHandlers = 4;
        torpedoCount = 4;
        torpedoPower = 293;
    }

    double getLightHitChance() const override
    {
        return 0.50;
    }

    double getTorpedoHitChance() const override
    {
        return 0.25;
    }

    string getShipType() const override
    {
        return "Corazzata";
    }

    string getFaction() const override
    {
        return "Zapezoid";
    }
};

// ROGOATUSKAN SHIPS

class Jager : public Ship
{
public:
    Jager(string id, string shipName) : Ship(id, shipName, 112)
    {
        maxPilots = 1;
        maxGunners = 1;
        lightCannonCount = 1;
        lightCannonPower = 101;
        maxTorpedoHandlers = 0;
        torpedoCount = 0;
        torpedoPower = 0;
    }

    double getLightHitChance() const override
    {
        return 0.24;
    }

    double getTorpedoHitChance() const override
    {
        return 0.05;
    }

    string getShipType() const override
    {
        return "Jager";
    }

    string getFaction() const override
    {
        return "Rogoatuskan";
    }
};

class Kreuzer : public Ship
{
public:
    Kreuzer(string id, string shipName) : Ship(id, shipName, 212)
    {
        maxPilots = 1;
        maxGunners = 2;
        lightCannonCount = 2;
        lightCannonPower = 132;
        maxTorpedoHandlers = 0;
        torpedoCount = 0;
        torpedoPower = 0;
    }

    double getLightHitChance() const override
    {
        return 0.29;
    }

    double getTorpedoHitChance() const override
    {
        return 0.10;
    }

    string getShipType() const override
    {
        return "Kreuzer";
    }

    string getFaction() const override
    {
        return "Rogoatuskan";
    }
};

class Fregatte : public Ship
{
public:
    Fregatte(string id, string shipName) : Ship(id, shipName, 1143)
    {
        maxPilots = 2;
        maxGunners = 11;
        lightCannonCount = 11;
        lightCannonPower = 159;
        maxTorpedoHandlers = 5;
        torpedoCount = 5;
        torpedoPower = 282;
    }

    double getLightHitChance() const override
    {
        return 0.60;
    }

    double getTorpedoHitChance() const override
    {
        return 0.30;
    }

    string getShipType() const override
    {
        return "Fregatte";
    }

    string getFaction() const override
    {
        return "Rogoatuskan";
    }
};

// BATTLE SIMULATION ENGINE

class BattleSimulator
{
private:
    vector<Ship *> zapezoidFleet;
    vector<Ship *> rogoatuskanFleet;
    mt19937 rng;

public:
    BattleSimulator(vector<Ship *> zFleet, vector<Ship *> rFleet)
        : zapezoidFleet(move(zFleet)), rogoatuskanFleet(move(rFleet))
    {
        rng.seed(static_cast<unsigned int>(time(nullptr)));
    }

    void runBattle()
    {
        int round = 1;

        cout << "Battle Start!\n";

        while (hasLivingShips(zapezoidFleet) && hasLivingShips(rogoatuskanFleet))
        {
            cout << "\n--- Round " << round << " ---\n";

            simulateRound();

            round++;
        }

        declareWinner();
    }

private:
    bool hasLivingShips(const vector<Ship *> &fleet)
    {
        for (auto *ship : fleet)
        {
            if (ship->isAlive() && ship->canFly())
            {
                return true;
            }
        }
        return false;
    }

    void simulateRound()
    {
        vector<tuple<Ship *, Ship *, Ship::SimpleWeapon, bool>> attacks;

        // Zapezoid attacks
        for (auto *attacker : zapezoidFleet)
        {
            if (!attacker->isAlive() || !attacker->canFly())
                continue;

            auto weapons = attacker->getActiveWeapons();
            for (const auto &weapon : weapons)
            {
                Ship *target = selectRandomTarget(rogoatuskanFleet);
                if (target)
                {
                    bool hits = rollHit(target, weapon.isLightCannon);
                    attacks.push_back({attacker, target, weapon, hits});
                }
            }
        }

        // Rogoatuskan attacks
        for (auto *attacker : rogoatuskanFleet)
        {
            if (!attacker->isAlive() || !attacker->canFly())
                continue;

            auto weapons = attacker->getActiveWeapons();
            for (const auto &weapon : weapons)
            {
                Ship *target = selectRandomTarget(zapezoidFleet);
                if (target)
                {
                    bool hits = rollHit(target, weapon.isLightCannon);
                    attacks.push_back({attacker, target, weapon, hits});
                }
            }
        }

        // Execute all attacks
        for (const auto &[attacker, target, weapon, hits] : attacks)
        {
            string weaponName = weapon.isLightCannon ? "Light Cannon" : "Torpedo";

            cout << attacker->getLabel() << " fires " << weaponName
                 << " at " << target->getLabel() << ". ";

            if (hits)
            {
                target->takeDamage(weapon.damage);
                cout << "Hit! " << target->getLabel() << " takes " << weapon.damage
                     << " damage. (HP left: " << target->getHP() << ")\n";

                if (!target->isAlive())
                {
                    cout << target->getLabel() << " has been destroyed!\n";
                }
            }
            else
            {
                cout << "Missed!\n";
            }

            cout << endl;
        }
    }

    Ship *selectRandomTarget(const vector<Ship *> &fleet)
    {
        vector<Ship *> aliveShips;
        for (auto *ship : fleet)
        {
            if (ship->isAlive() && ship->canFly())
            {
                aliveShips.push_back(ship);
            }
        }

        if (aliveShips.empty())
        {
            return nullptr;
        }

        uniform_int_distribution<size_t> dist(0, aliveShips.size() - 1);
        return aliveShips[dist(rng)];
    }

    bool rollHit(Ship *target, bool isLightCannon)
    {
        double hitChance = target->getAdjustedHitChance(isLightCannon);
        uniform_real_distribution<double> dist(0.0, 1.0);
        double roll = dist(rng);
        return roll < hitChance;
    }

    void declareWinner()
    {
        bool zapezoidsAlive = hasLivingShips(zapezoidFleet);
        bool rogoatuskansAlive = hasLivingShips(rogoatuskanFleet);

        cout << "\n--- Battle Over ---\n";
        if (zapezoidsAlive && !rogoatuskansAlive)
        {
            cout << "Zapezoid Fleet Wins!\n";
        }
        else if (!zapezoidsAlive && rogoatuskansAlive)
        {
            cout << "Rogoatuskan Fleet Wins!\n";
        }
        else
        {
            cout << "It's a Draw!\n";
        }
    }

    void printSurvivors(const vector<Ship *> &fleet)
    {
        for (const auto *ship : fleet)
        {
            if (ship->isAlive())
            {
                cout << ship->getLabel() << " survived with " << ship->getHP() << " HP left.\n";
            }
        }
    }
};

// ============================================
// CSV LOADING FUNCTIONS (TO BE IMPLEMENTED BY CSV TEAM MEMBER)
// ============================================

vector<Ship *> loadShips(const string &filename, const string &faction)
{
    vector<Ship *> ships;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filename << endl;
        return ships;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, type, name;

        getline(ss, id, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');

        Ship *ship = nullptr;

        if (faction == "Zapezoid")
        {
            if (type == "Guerriero")
                ship = new Guerriero(id, name);
            else if (type == "Medio")
                ship = new Medio(id, name);
            else if (type == "Corazzata")
                ship = new Corazzata(id, name);
        }
        else if (faction == "Rogoatuskan")
        {
            if (type == "Jager")
                ship = new Jager(id, name);
            else if (type == "Kreuzer")
                ship = new Kreuzer(id, name);
            else if (type == "Fregatte")
                ship = new Fregatte(id, name);
        }

        if (ship != nullptr)
        {
            ships.push_back(ship);
        }
    }

    file.close();
    return ships;
}

vector<Person *> loadCrew(const string &filename)
{
    vector<Person *> crew;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filename << endl;
        return crew;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, name, role;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');

        Person *person = new Person(id, name, role);
        crew.push_back(person);
    }

    file.close();
    return crew;
}

void assignCrewToShips(vector<Ship *> &ships, vector<Person *> &crew)
{
    // Sort ships by HP (smallest first) to spread crew
    sort(ships.begin(), ships.end(), [](Ship *a, Ship *b)
         { return a->getMaxHitPoints() < b->getMaxHitPoints(); });

    // First pass: assign at least one pilot to each ship
    for (auto *ship : ships)
    {
        for (auto it = crew.begin(); it != crew.end(); ++it)
        {
            Person *person = *it;
            if (person->getRole() == "pilot" || person->getRole() == "Pilot")
            {
                if (ship->addPilot(person))
                {
                    crew.erase(it);
                    break;
                }
            }
        }
    }

    // Second pass: distribute remaining crew
    for (auto *ship : ships)
    {
        // Add remaining pilots
        for (auto it = crew.begin(); it != crew.end();)
        {
            Person *person = *it;
            bool assigned = false;

            if (person->getRole() == "pilot" || person->getRole() == "Pilot")
            {
                if (ship->addPilot(person))
                {
                    it = crew.erase(it);
                    assigned = true;
                }
            }
            else if (person->getRole() == "gunner" || person->getRole() == "Gunner")
            {
                if (ship->addGunner(person))
                {
                    it = crew.erase(it);
                    assigned = true;
                }
            }
            else if (person->getRole() == "torpedo handler" || person->getRole() == "Torpedo Handler")
            {
                if (ship->addTorpedoHandler(person))
                {
                    it = crew.erase(it);
                    assigned = true;
                }
            }

            if (!assigned)
            {
                ++it;
            }
        }
    }
}

// ============================================
// MAIN FUNCTION
// ============================================

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cerr << "Usage: " << argv[0] << " <zShips.csv> <zCrew.csv> <rShips.csv> <rCrew.csv>\n";
        return 1;
    }

    auto zapezoidShips = loadShips(argv[1], "Zapezoid");
    auto zapezoidCrew = loadCrew(argv[2]);
    auto rogoatuskanShips = loadShips(argv[3], "Rogoatuskan");
    auto rogoatuskanCrew = loadCrew(argv[4]);

    cout << "Loaded " << zapezoidShips.size() << " Zapezoid ships and "
         << zapezoidCrew.size() << " crew members.\n";
    cout << "Loaded " << rogoatuskanShips.size() << " Rogoatuskan ships and "
         << rogoatuskanCrew.size() << " crew members.\n";

    assignCrewToShips(zapezoidShips, zapezoidCrew);
    assignCrewToShips(rogoatuskanShips, rogoatuskanCrew);

    BattleSimulator simulator(zapezoidShips, rogoatuskanShips);
    simulator.runBattle();

    // Cleanup
    for (auto *ship : zapezoidShips)
    {
        delete ship;
    }
    for (auto *ship : rogoatuskanShips)
    {
        delete ship;
    }
    for (auto *cm : zapezoidCrew)
    {
        delete cm;
    }
    for (auto *cm : rogoatuskanCrew)
    {
        delete cm;
    }

    return 0;
}