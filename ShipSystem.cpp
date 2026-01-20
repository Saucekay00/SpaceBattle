#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Person
{
private:
    string personID;
    string name;
    string role;

public:
    Person(string id, string personName, string PersonRole) : personID(id), name(personName), role(PersonRole) {}

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
    Ship(string id, string shipName, int maxHP) : shipID(id), name(shipName),
                                                  hitPoints(maxHP), maxHitPoints(maxHP),
                                                  maxPilots(0), maxGunners(0), maxTorpedoHandlers(0),
                                                  lightCannonCount(0), lightCannonPower(0),
                                                  torpedoCount(0), torpedoPower(0)
    {
    }

    virtual ~Ship()
    {
    }

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
        return hitPoints == 0;
    }

    void printHP() const
    {
        cout << "Ship HP:" << hitPoints << "/" << maxHitPoints << endl;
    }

    bool canFly() const
    {
        return !pilots.empty(); // ( pilots.empty() means check if its empty and !pilots.empty() means check if it has any pilot)
    }

    int getPilotCount() const
    {
        return pilots.size();
    }

    // the below is to calculate how many weapons can fire by checking the available crew
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

    //--------------------------------------------------------------
    int getLightCannonPower() const
    {
        return lightCannonPower;
    }

    int getTorpedoPower() const
    {
        return torpedoPower;
    }

    double applyPilotPenalty(double baseChance) const // private or public
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

    int getHitPoints() const
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

    bool operator<(const Ship &other) const
    {
        return hitPoints < other.hitPoints;
    }
};

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
};

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
};
