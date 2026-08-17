// Sahil
// OOP Project - PUBG Team Performance Analyzer
// Day 2 - Role classes (inheritance, overriding, polymorphism)

#include <iostream>
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

// testing all 4 roles through a base class pointer to check polymorphism works
int main()
{
    Player* squad[4];

    squad[0] = new Fragger("Sahil");
    squad[1] = new Assaulter("Rohan");
    squad[2] = new Support("Bibek");
    squad[3] = new IGL("Prakash");

    squad[0]->addMatch(MatchStats(8, 650, 12, 2));
    squad[1]->addMatch(MatchStats(5, 420, 9, 1));
    squad[2]->addMatch(MatchStats(1, 300, 20, 6));
    squad[3]->addMatch(MatchStats(2, 180, 25, 3));

    cout << "----------------------------------------\n";
    for (int i = 0; i < 4; i++)
    {
        cout << squad[i]->getName() << " (" << squad[i]->getRole() << ") -> "
             << "Rating: " << fixed << setprecision(2) << squad[i]->performanceRating() << "\n";
    }
    cout << "----------------------------------------\n";

    cout << "Total players created: " << Player::getTotalPlayers() << "\n";

    for (int i = 0; i < 4; i++)
    {
        delete squad[i];
    }

    return 0;
}