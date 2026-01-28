class CrewAssignmentToShip
{
public:
    void assignCrewToShips(vector<Ship *> &ships, vector<CrewMember *> &crew)
    {

        // ========== STEP 1: Organize crew by type ==========
        vector<CrewMember *> pilots;
        vector<CrewMember *> gunners;
        vector<CrewMember *> torpedoHandlers;

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

        // ========== STEP 2: Give every ship 1 pilot ==========
        int pilotIndex = 0;

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

        // ========== STEP 3: Give second pilot to big ships ==========
        for (int i = 0; i < ships.size(); i++)
        {
            Ship *currentShip = ships[i];
            string shipType = currentShip->getTypeName();

            if (shipType == "Corazzata" || shipType == "Fregatte")
            {
                if (pilotIndex < pilots.size())
                {
                    CrewMember *nextPilot = pilots[pilotIndex];
                    currentShip->assignCrewMember(nextPilot);
                    pilotIndex++;
                }
            }
        }

        // ========== STEP 4: Spread gunners evenly ==========
        int gunnerIndex = 0;

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

        // ========== STEP 5: Spread torpedo handlers evenly ==========
        int torpedoIndex = 0;

        while (torpedoIndex < torpedoHandlers.size())
        {
            bool assignedAnyTorpedo = false;

            for (int i = 0; i < ships.size(); i++)
            {
                if (torpedoIndex < torpedoHandlers.size())
                {
                    Ship *currentShip = ships[i];
                    string shipType = currentShip->getTypeName();

                    if (shipType == "Corazzata" || shipType == "Fregatte")
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

        // ========== STEP 6: Activate weapons ==========
        for (int i = 0; i < ships.size(); i++)
        {
            Ship *currentShip = ships[i];
            currentShip->assignWeaponOperators();
        }
    }
};
