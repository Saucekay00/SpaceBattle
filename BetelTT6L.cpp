//Betelguese TT6L 
//SHANJIF CAKRAVRTHI A/L KUPPAN @ SIVA KUMAR    251UC250J7
//RATNA KUMAARI A/P R MURUGANANTHAN             251UC2513K
//SIVAGAMI A/P SELVARAJOO                       243UC247BU
//KARIN PRAKASH                                 243UC245VE

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <cctype>
#include <iomanip>

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

    int lightCannonCount;
    int lightCannonPower;
    int torpedoCount;
    int torpedoPower;

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

    int getMaxPilots() const { return maxPilots; }
    int getMaxGunners() const { return maxGunners; }
    int getMaxTorpedoHandlers() const { return maxTorpedoHandlers; }

    const vector<Person*>& getPilots() const { return pilots; }
    const vector<Person*>& getGunners() const { return gunners; }
    const vector<Person*>& getTorpedoHandlers() const { return torpedoHandlers; }

    bool hasTorpedoes() const { return torpedoCount > 0 && maxTorpedoHandlers > 0; }
    bool pilotsCanShootLightCannons = false;
    

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
        int operatorCount;
        if (pilotsCanShootLightCannons)
            operatorCount = pilots.size();
        else
            operatorCount = gunners.size();
        if (operatorCount < lightCannonCount)
            return operatorCount;
        return min(lightCannonCount, operatorCount);
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

    const Person* getLightCannonOperator(int i) const
    {
        if (pilotsCanShootLightCannons)
        {
            if (i >= 0 && (size_t)i < pilots.size()) return pilots[i];
        }
        else
        {
            if (i >= 0 && (size_t)i < gunners.size()) return gunners[i];
        }
        return nullptr;
    }


    virtual double getLightHitChance() const = 0;
    virtual double getTorpedoHitChance() const = 0;
    virtual string getShipType() const = 0;
    virtual string getFaction() const = 0;

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

        int activeLightCannons = getActiveLightCannons();
        for (int i = 0; i < activeLightCannons; ++i)
        {
            weapons.push_back({true, lightCannonPower});
        }

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

//Zapezoid

class Guerriero : public Ship
{
public:
    Guerriero(string id, string shipName) : Ship(id, shipName, 123)
    {
        maxPilots = 1;
        maxGunners = 0;
        pilotsCanShootLightCannons = true;
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

//Rogoatuskans

class Jager : public Ship
{
public:
    Jager(string id, string shipName) : Ship(id, shipName, 112)
    {
        maxPilots = 1;
        maxGunners = 0;
        pilotsCanShootLightCannons = true;
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

static inline std::string trim_copy(std::string s);

//Shanjif 

class BattleSimulator
{
private:
    vector<Ship *> zapezoidFleet;
    vector<Ship *> rogoatuskanFleet;
    vector<Person *> unassignedZ;
    vector<Person *> unassignedR;
    mt19937 rng;

    static string joinNames(const vector<Person*> &v)
    {
        if (v.empty()) return "None";
        string s;
        for (size_t i = 0; i < v.size(); i++)
        {
            if (i) s += ", ";
            s += v[i]->getName();
        }
        return s;
    }

    void printShipDetails(const Ship *s) const
    {
        cout << "[" << s->getShipID() << "] "
             << left << setw(20) << s->getName()
             << " (" << s->getShipType() << ")\n";

        cout << "      HP: " << s->getHP() << "/" << s->getMaxHitPoints() << "\n";

        cout << "      Pilots (" << s->getPilotCount() << "/" << s->getMaxPilots() << "): "
             << joinNames(s->getPilots()) << "\n";

        cout << "      Gunners (" << (int)s->getGunners().size() << "/" << s->getMaxGunners() << "): "
             << joinNames(s->getGunners()) << "\n";

        if (s->hasTorpedoes())
        {
            cout << "      Torp Handlers (" << (int)s->getTorpedoHandlers().size()
                 << "/" << s->getMaxTorpedoHandlers() << "): "
                 << joinNames(s->getTorpedoHandlers()) << "\n";
        }

        cout << "--------------------------------------------------\n";
    }

    bool hasLivingShips(const vector<Ship *> &fleet) const
    {
        for (auto *ship : fleet)
        {
            if (ship->isAlive() && ship->canFly()) return true;
        }
        return false;
    }

    Ship *selectRandomTarget(const vector<Ship *> &fleet)
    {
        vector<Ship *> alive;
        for (Ship *s : fleet)
        {
            if (s->isAlive() && s->canFly()) alive.push_back(s);
        }
        if (alive.empty()) return nullptr;
        uniform_int_distribution<size_t> dist(0, alive.size() - 1);
        return alive[dist(rng)];
    }

    bool rollHit(Ship *attacker, bool isLightCannon)
    {
        double hitChance = attacker->getAdjustedHitChance(isLightCannon);
        uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng) < hitChance;
    }

    void printRoundStatus() const
    {
        cout << "\n--- Round Status Report ---\n";
        cout << "Zapezoids:\n";
        for (auto *s : zapezoidFleet)
        {
            cout << " [" << s->getShipID() << "] " << s->getShipType() << " " << s->getName() << " ";
            if (!s->isAlive()) cout << "-- DESTROYED --\n";
            else cout << "(" << s->getHP() << "/" << s->getMaxHitPoints() << ")\n";
        }

        cout << "Rogoatuskans:\n";
        for (auto *s : rogoatuskanFleet)
        {
            cout << " [" << s->getShipID() << "] " << s->getShipType() << " " << s->getName() << " ";
            if (!s->isAlive()) cout << "-- DESTROYED --\n";
            else cout << "(" << s->getHP() << "/" << s->getMaxHitPoints() << ")\n";
        }
    }

public:
    BattleSimulator(const vector<Ship *> &zFleet,
                    const vector<Ship *> &rFleet,
                    const vector<Person *> &unZ,
                    const vector<Person *> &unR)
        : zapezoidFleet(zFleet), rogoatuskanFleet(rFleet), unassignedZ(unZ), unassignedR(unR)
    {
        rng.seed((unsigned)time(nullptr));
    }

    void printFleetConfiguration() const
    {
        cout << "\n========================================\n";
        cout << "       FLEET CONFIGURATION REPORT       \n";
        cout << "========================================\n\n";

        cout << "--- ZAPEZOID FLEET ---\n";
        for (auto *s : zapezoidFleet) printShipDetails(s);

        cout << "--- ROGOATUSKAN FLEET ---\n";
        for (auto *s : rogoatuskanFleet) printShipDetails(s);

        if (!unassignedZ.empty())
        {
            cout << "\n[Unassigned ZAPEZOID Crew]\n";
            for (auto *p : unassignedZ)
                cout << " - " << p->getName() << " (" << trim_copy(p->getRole()) << ")\n";
        }

        if (!unassignedR.empty())
        {
            cout << "\n[Unassigned ROGOATUSKAN Crew]\n";
            for (auto *p : unassignedR)
                cout << " - " << p->getName() << " (" << trim_copy(p->getRole()) << ")\n";
        }

        cout << "\n========================================\n";
        cout << "           BATTLE COMMENCING            \n";
        cout << "========================================\n\n";
    }

    void runBattle()
    {
        int round = 1;

        while (hasLivingShips(zapezoidFleet) && hasLivingShips(rogoatuskanFleet))
        {
            cout << ">>> ROUND " << round << " <<<\n";

            for (Ship *att : zapezoidFleet)
            {
                if (!att->isAlive() || !att->canFly()) continue;

                int shots = att->getLightShots();
                for (int i = 0; i < shots; i++)
                {
                    Ship *tgt = selectRandomTarget(rogoatuskanFleet);
                    if (!tgt) break;

                    const Person* op = att->getLightCannonOperator(i);
                    string crewName = op ? op->getName() : "Unknown";

                    bool hit = rollHit(att, true);

                    cout << "   " << att->getShipType() << " " << att->getName()
                         << "'s Cannon crew " << crewName
                         << " fires at " << tgt->getShipType() << " " << tgt->getName() << "... ";

                    if (hit)
                    {
                        tgt->takeDamage(att->getLightCannonPower());
                        cout << "HIT! (" << att->getLightCannonPower() << " dmg)\n";
                    }
                    else
                    {
                        cout << "MISS.\n";
                    }
                }

                int torps = att->getTorpedoShots();
                for (int i = 0; i < torps; i++)
                {
                    Ship *tgt = selectRandomTarget(rogoatuskanFleet);
                    if (!tgt) break;

                    string crewName = att->getTorpedoHandlers()[i]->getName();
                    bool hit = rollHit(att, false);

                    cout << "   " << att->getShipType() << " " << att->getName()
                         << "'s Torpedo crew " << crewName
                         << " fires at " << tgt->getShipType() << " " << tgt->getName() << "... ";

                    if (hit)
                    {
                        tgt->takeDamage(att->getTorpedoPower());
                        cout << "HIT! (" << att->getTorpedoPower() << " dmg)\n";
                    }
                    else
                    {
                        cout << "MISS.\n";
                    }
                }
            }

            for (Ship *att : rogoatuskanFleet)
            {
                if (!att->isAlive() || !att->canFly()) continue;

                int shots = att->getLightShots();
                for (int i = 0; i < shots; i++)
                {
                    Ship *tgt = selectRandomTarget(zapezoidFleet);
                    if (!tgt) break;

                    const Person* op = att->getLightCannonOperator(i);
                    string crewName = op ? op->getName() : "Unknown";
                    
                    bool hit = rollHit(att, true);

                    cout << "   " << att->getShipType() << " " << att->getName()
                         << "'s Cannon crew " << crewName
                         << " fires at " << tgt->getShipType() << " " << tgt->getName() << "... ";

                    if (hit)
                    {
                        tgt->takeDamage(att->getLightCannonPower());
                        cout << "HIT! (" << att->getLightCannonPower() << " dmg)\n";
                    }
                    else
                    {
                        cout << "MISS.\n";
                    }
                }

                int torps = att->getTorpedoShots();
                for (int i = 0; i < torps; i++)
                {
                    Ship *tgt = selectRandomTarget(zapezoidFleet);
                    if (!tgt) break;

                    string crewName = att->getTorpedoHandlers()[i]->getName();
                    bool hit = rollHit(att, false);

                    cout << "   " << att->getShipType() << " " << att->getName()
                         << "'s Torpedo crew " << crewName
                         << " fires at " << tgt->getShipType() << " " << tgt->getName() << "... ";

                    if (hit)
                    {
                        tgt->takeDamage(att->getTorpedoPower());
                        cout << "HIT! (" << att->getTorpedoPower() << " dmg)\n";
                    }
                    else
                    {
                        cout << "MISS.\n";
                    }
                }
            }

            printRoundStatus();
            cout << "\n";
            round++;
        }

        cout << "========================================\n";
        cout << "              FINAL RESULT              \n";
        cout << "========================================\n";

        bool zAlive = hasLivingShips(zapezoidFleet);
        bool rAlive = hasLivingShips(rogoatuskanFleet);

        if (zAlive && !rAlive) cout << "*** ZAPEZOIDS WIN! ***\n";
        else if (!zAlive && rAlive) cout << "*** ROGOATUSKANS WIN! ***\n";
        else cout << "*** DRAW! ***\n";
    }
};

static inline std::string trim_copy(std::string s) {
    auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

static inline std::string normalize_role(std::string s) {
    s = trim_copy(s);

    for (char &c : s) c = (char)std::tolower((unsigned char)c);

    std::string out;
    out.reserve(s.size());
    bool prevSpace = false;
    for (unsigned char ch : s) {
        if (std::isspace(ch)) {
            if (!prevSpace) out.push_back(' ');
            prevSpace = true;
        } else {
            out.push_back((char)ch);
            prevSpace = false;
        }
    }
    return trim_copy(out);
}

static inline void trim_inplace(std::string &s) {
    while (!s.empty() && (s.back()=='\r' || s.back()=='\n' || s.back()==' ' || s.back()=='\t'))
        s.pop_back();
    size_t i = 0;
    while (i < s.size() && (s[i]==' ' || s[i]=='\t')) i++;
    s.erase(0, i);
}

class CrewCoordinator
{
private:
    vector<Person *> pilots;
    vector<Person *> gunners;
    vector<Person *> torpedoHandlers;

    int pilotIndex = 0;
    int gunnerIndex = 0;
    int torpedoIndex = 0;

    static string roleKey(string s)
    {
        s = trim_copy(s);
        string out;
        for (unsigned char ch : s)
        {
            if (!isspace(ch)) out.push_back((char)tolower(ch));
        }
        return out; 
    }

    void categorizeCrewByType(const vector<Person *> &crew)
    {
        for (Person *p : crew)
        {
            string key = roleKey(p->getRole());

            if (key == "pilot") pilots.push_back(p);
            else if (key == "gunner") gunners.push_back(p);
            else if (key == "torpedohandler") torpedoHandlers.push_back(p);
        }
    }

    bool isLargeShip(const string &shipType) const
    {
        return (shipType == "Corazzata" || shipType == "Fregatte");
    }

    bool hasTorpedoCapability(const string &shipType) const
    {
        return (shipType == "Corazzata" || shipType == "Fregatte");
    }

    void reset()
    {
        pilots.clear();
        gunners.clear();
        torpedoHandlers.clear();
        pilotIndex = gunnerIndex = torpedoIndex = 0;
    }

public:
    vector<Person *> assignCrewToShips(vector<Ship *> &ships, vector<Person *> &crew)
    {
        reset();
        categorizeCrewByType(crew);

        for (Ship *s : ships)
        {
            if (pilotIndex < (int)pilots.size())
            {
                if (s->addPilot(pilots[pilotIndex])) pilotIndex++;
            }
        }

        for (Ship *s : ships)
        {
            if (!isLargeShip(s->getShipType())) continue;
            if (pilotIndex < (int)pilots.size())
            {
                if (s->addPilot(pilots[pilotIndex])) pilotIndex++;
            }
        }

        bool changed = true;
        while (changed && gunnerIndex < (int)gunners.size())
        {
            changed = false;
            for (Ship *s : ships)
            {
                if (gunnerIndex >= (int)gunners.size()) break;
                if (s->addGunner(gunners[gunnerIndex]))
                {
                    gunnerIndex++;
                    changed = true;
                }
            }
        }

        changed = true;
        while (changed && torpedoIndex < (int)torpedoHandlers.size())
        {
            changed = false;
            for (Ship *s : ships)
            {
                if (torpedoIndex >= (int)torpedoHandlers.size()) break;
                if (!hasTorpedoCapability(s->getShipType())) continue;

                if (s->addTorpedoHandler(torpedoHandlers[torpedoIndex]))
                {
                    torpedoIndex++;
                    changed = true;
                }
            }
        }

        vector<Person *> unassigned;
        for (int i = pilotIndex; i < (int)pilots.size(); i++) unassigned.push_back(pilots[i]);
        for (int i = gunnerIndex; i < (int)gunners.size(); i++) unassigned.push_back(gunners[i]);
        for (int i = torpedoIndex; i < (int)torpedoHandlers.size(); i++) unassigned.push_back(torpedoHandlers[i]);

        return unassigned;
    }
};

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
        string id, role, name, type;

        getline(ss, id, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');

        trim_inplace(id);
        trim_inplace(type);
        trim_inplace(name);

        std::transform(type.begin(), type.end(), type.begin(),
                    [](unsigned char c){ return std::tolower(c); });

        Ship *ship = nullptr;

        if (faction == "Zapezoid")
        {
            if (type == "guerriero")      ship = new Guerriero(id, name);
            else if (type == "medio")     ship = new Medio(id, name);
            else if (type == "corazzata") ship = new Corazzata(id, name);
        }
        else if (faction == "Rogoatuskan")
        {
            if (type == "jager")          ship = new Jager(id, name);
            else if (type == "kreuzer")   ship = new Kreuzer(id, name);
            else if (type == "fregatte")  ship = new Fregatte(id, name);
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
    CrewCoordinator coordinator;
    coordinator.assignCrewToShips(ships, crew);
}

//Shanjif
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cerr << "Usage: " << argv[0]
             << " <zShips.csv> <zCrew.csv> <rShips.csv> <rCrew.csv>\n";
        return 1;
    }

    cout << "Loading data files...\n";

    vector<Ship*> zShips = loadShips(argv[1], "Zapezoid");
    vector<Person*> zCrew = loadCrew(argv[2]);

    vector<Ship*> rShips = loadShips(argv[3], "Rogoatuskan");
    vector<Person*> rCrew = loadCrew(argv[4]);

    CrewCoordinator coordinator;
    vector<Person*> unassignedZ = coordinator.assignCrewToShips(zShips, zCrew);
    vector<Person*> unassignedR = coordinator.assignCrewToShips(rShips, rCrew);

    BattleSimulator sim(zShips, rShips, unassignedZ, unassignedR);
    sim.printFleetConfiguration();
    sim.runBattle();

    for (Ship* s : zShips) delete s;
    for (Ship* s : rShips) delete s;
    for (Person* p : zCrew) delete p;
    for (Person* p : rCrew) delete p;

    return 0;
}
