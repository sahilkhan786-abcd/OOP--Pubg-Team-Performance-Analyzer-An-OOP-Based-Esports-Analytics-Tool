// Sahil
// OOP Project - PUBG Team Performance Analyzer

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// stores stats for one match
class MatchStats
{
private:
    int kills;
    double damage;
    double survivalTime;
    int assists;

public:
    MatchStats()
    {
        kills = 0;
        damage = 0.0;
        survivalTime = 0.0;
        assists = 0;
    }

    MatchStats(int k, double d, double s, int a)
    {
        kills = k;
        damage = d;
        survivalTime = s;
        assists = a;
    }

    int getKills()
    {
        return kills;
    }

    double getDamage()
    {
        return damage;
    }

    double getSurvivalTime()
    {
        return survivalTime;
    }

    int getAssists()
    {
        return assists;
    }

    void display()
    {
        cout << "Kills: " << kills
             << ", Damage: " << fixed << setprecision(1) << damage
             << ", Survival: " << survivalTime << " min"
             << ", Assists: " << assists;
    }
};

// base class for all player roles
class Player
{
private:
    string name;
    MatchStats matchHistory[10];
    int matchCount;
    static int totalPlayers;

public:
    Player(string playerName)
    {
        name = playerName;
        matchCount = 0;
        totalPlayers++;
    }

    ~Player()
    {
    }

    void addMatch(MatchStats m)
    {
        if (matchCount < 10)
        {
            matchHistory[matchCount] = m;
            matchCount++;
        }
        else
        {
            cout << "Match history full for " << name << ".\n";
        }
    }

    string getName()
    {
        return name;
    }

    int getMatchCount()
    {
        return matchCount;
    }

    MatchStats getMatch(int index)
    {
        return matchHistory[index];
    }

    // average kills across all recorded matches
    double getAverageKills()
    {
        if (matchCount == 0)
        {
            return 0.0;
        }

        int total = 0;
        for (int i = 0; i < matchCount; i++)
        {
            total += matchHistory[i].getKills();
        }
        return (double)total / matchCount;
    }

    // average damage across all recorded matches
    double getAverageDamage()
    {
        if (matchCount == 0)
        {
            return 0.0;
        }

        double total = 0.0;
        for (int i = 0; i < matchCount; i++)
        {
            total += matchHistory[i].getDamage();
        }
        return total / matchCount;
    }

    static int getTotalPlayers()
    {
        return totalPlayers;
    }

    // every role calculates this differently
    virtual double performanceRating()
    {
        return 0.0;
    }

    virtual string getRole()
    {
        return "Player";
    }

    // used when saving to file, one letter per role keeps the file short
    virtual char getRoleCode()
    {
        return 'P';
    }
};

int Player::totalPlayers = 0;

// fragger - main kill damage dealer
class Fragger : public Player
{
public:
    Fragger(string name) : Player(name)
    {
    }

    string getRole()
    {
        return "Fragger";
    }

    char getRoleCode()
    {
        return 'F';
    }

    double performanceRating()
    {
        if (getMatchCount() == 0)
        {
            return 0.0;
        }

        double total = 0.0;
        for (int i = 0; i < getMatchCount(); i++)
        {
            MatchStats m = getMatch(i);
            total += (m.getKills() * 3.0) + (m.getDamage() * 0.05);
        }
        return total / getMatchCount();
    }
};

// assaulter - close to mid range aggression
class Assaulter : public Player
{
public:
    Assaulter(string name) : Player(name)
    {
    }

    string getRole()
    {
        return "Assaulter";
    }

    char getRoleCode()
    {
        return 'A';
    }

    double performanceRating()
    {
        if (getMatchCount() == 0)
        {
            return 0.0;
        }

        double total = 0.0;
        for (int i = 0; i < getMatchCount(); i++)
        {
            MatchStats m = getMatch(i);
            total += (m.getKills() * 2.0) + (m.getDamage() * 0.07);
        }
        return total / getMatchCount();
    }
};

// support - assists and keeping the team alive
class Support : public Player
{
public:
    Support(string name) : Player(name)
    {
    }

    string getRole()
    {
        return "Support";
    }

    char getRoleCode()
    {
        return 'S';
    }

    double performanceRating()
    {
        if (getMatchCount() == 0)
        {
            return 0.0;
        }

        double total = 0.0;
        for (int i = 0; i < getMatchCount(); i++)
        {
            MatchStats m = getMatch(i);
            total += (m.getAssists() * 4.0) + (m.getSurvivalTime() * 0.5);
        }
        return total / getMatchCount();
    }
};

// igl - calls the shots, plays safe
class IGL : public Player
{
public:
    IGL(string name) : Player(name)
    {
    }

    string getRole()
    {
        return "IGL";
    }

    char getRoleCode()
    {
        return 'I';
    }

    double performanceRating()
    {
        if (getMatchCount() == 0)
        {
            return 0.0;
        }

        double total = 0.0;
        for (int i = 0; i < getMatchCount(); i++)
        {
            MatchStats m = getMatch(i);
            total += (m.getSurvivalTime() * 0.8) + (m.getAssists() * 2.0);
        }
        return total / getMatchCount();
    }
};

// creates the right player type based on the saved role code
Player* createPlayerFromCode(char code, string name)
{
    if (code == 'F')
    {
        return new Fragger(name);
    }
    else if (code == 'A')
    {
        return new Assaulter(name);
    }
    else if (code == 'S')
    {
        return new Support(name);
    }
    else if (code == 'I')
    {
        return new IGL(name);
    }
    else
    {
        return NULL;
    }
}

// holds a full squad and generates the report
class Team
{
private:
    string teamName;
    Player* players[4];
    int playerCount;
    static int totalTeams;

public:
    Team(string name)
    {
        teamName = name;
        playerCount = 0;
        totalTeams++;
    }

    ~Team()
    {
        for (int i = 0; i < playerCount; i++)
        {
            delete players[i];
        }
    }

    bool addPlayer(Player* p)
    {
        if (playerCount >= 4)
        {
            return false;
        }
        players[playerCount] = p;
        playerCount++;
        return true;
    }

    int getPlayerCount()
    {
        return playerCount;
    }

    Player* getPlayer(int index)
    {
        return players[index];
    }

    string getName()
    {
        return teamName;
    }

    static int getTotalTeams()
    {
        return totalTeams;
    }

    Player* mvpOfTeam()
    {
        Player* best = players[0];
        for (int i = 1; i < playerCount; i++)
        {
            if (players[i]->performanceRating() > best->performanceRating())
            {
                best = players[i];
            }
        }
        return best;
    }

    Player* weakestPlayer()
    {
        Player* worst = players[0];
        for (int i = 1; i < playerCount; i++)
        {
            if (players[i]->performanceRating() < worst->performanceRating())
            {
                worst = players[i];
            }
        }
        return worst;
    }

    void displayTeamReport()
    {
        cout << "\n========================================\n";
        cout << " TEAM REPORT: " << teamName << "\n";
        cout << "========================================\n";

        if (playerCount == 0)
        {
            cout << "No players added yet.\n";
            return;
        }

        Player* mvp = mvpOfTeam();

        cout << left << setw(12) << "Name" << setw(12) << "Role"
             << setw(10) << "Avg Kills" << setw(12) << "Avg Damage" << "MVP\n";
        cout << "----------------------------------------------------\n";

        for (int i = 0; i < playerCount; i++)
        {
            cout << left << setw(12) << players[i]->getName()
                 << setw(12) << players[i]->getRole()
                 << setw(10) << fixed << setprecision(1) << players[i]->getAverageKills()
                 << setw(12) << fixed << setprecision(1) << players[i]->getAverageDamage()
                 << (players[i] == mvp ? "Yes" : "") << "\n";
        }

        Player* weak = weakestPlayer();

        cout << "----------------------------------------------------\n";
        cout << "MVP of the Team   : " << mvp->getName() << " (" << mvp->getRole() << ")\n";
        cout << "Weakest Performer : " << weak->getName() << " (" << weak->getRole() << ")\n";
        cout << "========================================\n";
    }

    // writes team name, players and their match stats to a text file
    void saveToFile(string filename)
    {
        ofstream outFile(filename.c_str());

        if (!outFile)
        {
            cout << "Could not open file for saving.\n";
            return;
        }

        outFile << teamName << "\n";
        outFile << playerCount << "\n";

        for (int i = 0; i < playerCount; i++)
        {
            outFile << players[i]->getRoleCode() << " "
                    << players[i]->getName() << " "
                    << players[i]->getMatchCount() << "\n";

            for (int j = 0; j < players[i]->getMatchCount(); j++)
            {
                MatchStats m = players[i]->getMatch(j);
                outFile << m.getKills() << " "
                        << m.getDamage() << " "
                        << m.getSurvivalTime() << " "
                        << m.getAssists() << "\n";
            }
        }

        outFile.close();
        cout << "Team data saved to " << filename << "\n";
    }

    // rebuilds the team from a previously saved file
    void loadFromFile(string filename)
    {
        ifstream inFile(filename.c_str());

        if (!inFile)
        {
            cout << "No saved file found.\n";
            return;
        }

        // clear whatever players are currently on the team before loading
        for (int i = 0; i < playerCount; i++)
        {
            delete players[i];
        }
        playerCount = 0;

        int savedPlayerCount;
        inFile >> teamName;
        inFile >> savedPlayerCount;

        for (int i = 0; i < savedPlayerCount; i++)
        {
            char roleCode;
            string name;
            int matchCount;

            inFile >> roleCode >> name >> matchCount;

            Player* p = createPlayerFromCode(roleCode, name);

            for (int j = 0; j < matchCount; j++)
            {
                int k, a;
                double d, s;
                inFile >> k >> d >> s >> a;
                p->addMatch(MatchStats(k, d, s, a));
            }

            addPlayer(p);
        }

        inFile.close();
        cout << "Team data loaded from " << filename << "\n";
    }
};

int Team::totalTeams = 0;

// creates the right player type based on menu choice
Player* createPlayerByRole(int roleChoice, string name)
{
    if (roleChoice == 1)
    {
        return new Fragger(name);
    }
    else if (roleChoice == 2)
    {
        return new Assaulter(name);
    }
    else if (roleChoice == 3)
    {
        return new Support(name);
    }
    else if (roleChoice == 4)
    {
        return new IGL(name);
    }
    else
    {
        return NULL;
    }
}

void addPlayerMenu(Team& team)
{
    if (team.getPlayerCount() >= 4)
    {
        cout << "This team already has 4 players.\n";
        return;
    }

    string name;
    int roleChoice;

    cout << "Enter player name: ";
    cin >> name;

    cout << "Select role:\n";
    cout << "  1. Fragger\n";
    cout << "  2. Assaulter\n";
    cout << "  3. Support\n";
    cout << "  4. IGL\n";
    cout << "Choice: ";
    cin >> roleChoice;

    Player* p = createPlayerByRole(roleChoice, name);

    if (p == NULL)
    {
        cout << "Invalid role choice.\n";
        return;
    }

    team.addPlayer(p);
    cout << "Player \"" << name << "\" added as " << p->getRole() << ".\n";
}

void enterMatchStatsMenu(Team& team)
{
    if (team.getPlayerCount() == 0)
    {
        cout << "No players on the team yet.\n";
        return;
    }

    cout << "\nSelect player:\n";
    for (int i = 0; i < team.getPlayerCount(); i++)
    {
        cout << "  " << (i + 1) << ". " << team.getPlayer(i)->getName()
             << " (" << team.getPlayer(i)->getRole() << ")\n";
    }
    cout << "Choice: ";

    int choice;
    cin >> choice;

    if (choice < 1 || choice > team.getPlayerCount())
    {
        cout << "Invalid selection.\n";
        return;
    }

    Player* p = team.getPlayer(choice - 1);

    int kills, assists;
    double damage, survivalTime;

    cout << "Enter kills: ";
    cin >> kills;
    cout << "Enter damage dealt: ";
    cin >> damage;
    cout << "Enter survival time (minutes): ";
    cin >> survivalTime;
    cout << "Enter assists: ";
    cin >> assists;

    MatchStats stats(kills, damage, survivalTime, assists);
    p->addMatch(stats);

    cout << "Match recorded for " << p->getName() << " -> ";
    stats.display();
    cout << "\n";
}

void printMenu()
{
    cout << "\n===== PUBG TEAM PERFORMANCE ANALYZER =====\n";
    cout << "1. Add Player\n";
    cout << "2. Enter Match Statistics\n";
    cout << "3. View Team Report\n";
    cout << "4. Save Team Data\n";
    cout << "5. Load Team Data\n";
    cout << "6. Exit\n";
    cout << "Choice: ";
}

int main()
{
    cout << "Welcome to the PUBG Team Performance Analyzer\n";
    cout << "Enter your team name: ";
    string teamName;
    cin >> teamName;

    Team team(teamName);
    string filename = "team_data.txt";

    int choice = 0;
    while (choice != 6)
    {
        printMenu();
        cin >> choice;

        if (choice == 1)
        {
            addPlayerMenu(team);
        }
        else if (choice == 2)
        {
            enterMatchStatsMenu(team);
        }
        else if (choice == 3)
        {
            team.displayTeamReport();
        }
        else if (choice == 4)
        {
            team.saveToFile(filename);
        }
        else if (choice == 5)
        {
            team.loadFromFile(filename);
        }
        else if (choice == 6)
        {
            cout << "Exiting. Total players tracked: " << Player::getTotalPlayers() << "\n";
        }
        else
        {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}