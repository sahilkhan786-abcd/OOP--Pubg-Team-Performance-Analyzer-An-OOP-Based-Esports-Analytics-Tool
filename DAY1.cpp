// Sahil
// OOP Project - PUBG Team Performance Analyzer
// Day 1 - MatchStats class and base Player class

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

    // will be overridden by each role later, just a placeholder for now
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

// quick test to check the classes work before adding roles tomorrow
int main()
{
    Player p1("Sahil");

    MatchStats m1(8, 650, 12, 2);
    MatchStats m2(5, 400, 10, 1);

    p1.addMatch(m1);
    p1.addMatch(m2);

    cout << "Player: " << p1.getName() << "\n";
    cout << "Matches recorded: " << p1.getMatchCount() << "\n";

    for (int i = 0; i < p1.getMatchCount(); i++)
    {
        MatchStats m = p1.getMatch(i);
        cout << "Match " << (i + 1) << " -> ";
        m.display();
        cout << "\n";
    }

    cout << "Total players created: " << Player::getTotalPlayers() << "\n";

    return 0;
}