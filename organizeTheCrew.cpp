
// Step 1: Organize crew members by their type

void assignCrewToShips(vector<Ship *> &ships, vector<CrewMember *> &crew)
{

    // Create 3 empty lists to hold different types of crew
    vector<CrewMember *> pilots;
    vector<CrewMember *> gunners;
    vector<CrewMember *> torpedoHandlers;

    // Go through each crew member one by one
    for (int i = 0; i < crew.size(); i++)
    {

        // Get the current crew member
        CrewMember *currentCrew = crew[i];

        // Ask: "What type are you?"
        CrewType type = currentCrew->getType();

        // Put them in the right list
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
