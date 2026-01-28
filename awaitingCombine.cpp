#ifndef CREW_COORDINATOR_H
#define CREW_COORDINATOR_H

#include <vector>
#include <string>
using namespace std;

class Ship;
class CrewMember;
enum class CrewType;

class CrewCoordinator
{
private:
    vector<CrewMember *> pilots;
    vector<CrewMember *> gunners;
    vector<CrewMember *> torpedoHandlers;

    int pilotIndex;
    int gunnerIndex;
    int torpedoIndex;

    void categorizeCrewByType(const vector<CrewMember *> &crew);
    void assignPrimaryPilots(vector<Ship *> &ships);
    void assignSecondaryPilots(vector<Ship *> &ships);
    void distributeGunners(vector<Ship *> &ships);
    void distributeTorpedoHandlers(vector<Ship *> &ships);
    void activateWeaponSystems(vector<Ship *> &ships);
    bool isLargeShip(const string &shipType) const;
    bool hasTorpedoCapability(const string &shipType) const;
    void reset();

public:
    CrewCoordinator();
    ~CrewCoordinator();
    void assignCrewToShips(vector<Ship *> &ships, vector<CrewMember *> &crew);
    int getUnassignedPilotCount() const;
    int getUnassignedGunnerCount() const;
    int getUnassignedTorpedoHandlerCount() const;
};

#endif // CREW_COORDINATOR_H

// IMPLEMENTATION FILE (CrewCoordinator.cpp)

#include "CrewCoordinator.h"
// #include "Ship.h"
// #include "CrewMember.h"
#include <iostream>

CrewCoordinator::CrewCoordinator()
    : pilotIndex(0), gunnerIndex(0), torpedoIndex(0)
{
}

CrewCoordinator::~CrewCoordinator()
{
}

void CrewCoordinator::reset()
{
    pilots.clear();
    gunners.clear();
    torpedoHandlers.clear();
    pilotIndex = 0;
    gunnerIndex = 0;
    torpedoIndex = 0;
}

void CrewCoordinator::categorizeCrewByType(const vector<CrewMember *> &crew)
{
    for (int i = 0; i < crew.size(); i++)
    {
        CrewMember *currentCrew = crew[i];
        CrewType type = currentCrew->getType();

        if (type == CrewType::Pilot)
        {
            pilots.push_back(currentCrew);
        }
        else if (type == CrewType::Gunner)
        {
            gunners.push_back(currentCrew);
        }
        else if (type == CrewType::TorpedoHandler)
        {
            torpedoHandlers.push_back(currentCrew);
        }
    }
}

void CrewCoordinator::assignPrimaryPilots(vector<Ship *> &ships)
{
    for (int i = 0; i < ships.size(); i++)
    {
        Ship *currentShip = ships[i];

        if (pilotIndex < pilots.size())
        {
            CrewMember *nextPilot = pilots[pilotIndex];
            currentShip->assignCrewMember(nextPilot);
            pilotIndex++;
        }
    }
}

void CrewCoordinator::assignSecondaryPilots(vector<Ship *> &ships)
{
    for (int i = 0; i < ships.size(); i++)
    {
        Ship *currentShip = ships[i];
        string shipType = currentShip->getTypeName();

        if (isLargeShip(shipType))
        {
            if (pilotIndex < pilots.size())
            {
                CrewMember *nextPilot = pilots[pilotIndex];
                currentShip->assignCrewMember(nextPilot);
                pilotIndex++;
            }
        }
    }
}

void CrewCoordinator::distributeGunners(vector<Ship *> &ships)
{
    while (gunnerIndex < gunners.size())
    {
        bool assignedAnyGunner = false;

        for (int i = 0; i < ships.size(); i++)
        {
            if (gunnerIndex < gunners.size())
            {
                Ship *currentShip = ships[i];
                CrewMember *nextGunner = gunners[gunnerIndex];
                currentShip->assignCrewMember(nextGunner);
                gunnerIndex++;
                assignedAnyGunner = true;
            }
            else
            {
                break;
            }
        }

        if (!assignedAnyGunner)
        {
            break;
        }
    }
}

void CrewCoordinator::distributeTorpedoHandlers(vector<Ship *> &ships)
{
    while (torpedoIndex < torpedoHandlers.size())
    {
        bool assignedAnyTorpedo = false;

        for (int i = 0; i < ships.size(); i++)
        {
            if (torpedoIndex < torpedoHandlers.size())
            {
                Ship *currentShip = ships[i];
                string shipType = currentShip->getTypeName();

                if (hasTorpedoCapability(shipType))
                {
                    CrewMember *nextTorpedoHandler = torpedoHandlers[torpedoIndex];
                    currentShip->assignCrewMember(nextTorpedoHandler);
                    torpedoIndex++;
                    assignedAnyTorpedo = true;
                }
            }
            else
            {
                break;
            }
        }

        if (!assignedAnyTorpedo)
        {
            break;
        }
    }
}

void CrewCoordinator::activateWeaponSystems(vector<Ship *> &ships)
{
    for (int i = 0; i < ships.size(); i++)
    {
        Ship *currentShip = ships[i];
        currentShip->assignWeaponOperators();
    }
}

bool CrewCoordinator::isLargeShip(const string &shipType) const
{
    return (shipType == "Corazzata" || shipType == "Fregatte");
}

bool CrewCoordinator::hasTorpedoCapability(const string &shipType) const
{
    return (shipType == "Corazzata" || shipType == "Fregatte");
}

void CrewCoordinator::assignCrewToShips(vector<Ship *> &ships, vector<CrewMember *> &crew)
{
    reset();
    categorizeCrewByType(crew);
    assignPrimaryPilots(ships);
    assignSecondaryPilots(ships);
    distributeGunners(ships);
    distributeTorpedoHandlers(ships);
    activateWeaponSystems(ships);
}

int CrewCoordinator::getUnassignedPilotCount() const
{
    return pilots.size() - pilotIndex;
}

int CrewCoordinator::getUnassignedGunnerCount() const
{
    return gunners.size() - gunnerIndex;
}

int CrewCoordinator::getUnassignedTorpedoHandlerCount() const
{
    return torpedoHandlers.size() - torpedoIndex;
}