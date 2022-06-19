#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;
struct User
{
    bool admin = false;
    string username;
    string password;
    string name;
    string EGN;
    string birthdate;
    int initialBalance = 100; // initial balance
    int currentBalance = 100; // current balance
    int gameHistory[1000];
    int gamesPlayed = 0;
};
struct Casino
{
    User users[10000];
    string name;
    string dateOpened;
    int numUsers;
};
int main()
{

}
void roulette(User player)
{
    int black[18] = { 2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35 };
    int red[18] = { 1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36 };
    int green = 0;
    int bet;
    cout << "Input bet" << endl;
    cin >> bet;
    string colour;
    cout << "Input colour (type \"Black\", \"Red\" or \"Green\", otherwise you will lose your bet)" << endl;
    getline(cin, colour);
    int roll;
    srand(time(NULL));
    roll = rand() % 37;
    if (colour == "Black")
    {
        for (int i = 0; i < 18; i++)
        {
            if (roll == black[i])
            {
                curBal += bet;
                cout << "Congrats! You won " << bet << endl;
                return;
            }
        }
    }
    if (colour == "Red")
    {
        for (int i = 0; i < 18; i++)
        {
            if (roll == red[i])
            {
                curBal += bet;
                cout << "Congrats! You won " << bet << endl;
                return;
            }
        }
    }
    if (colour == "Green")
    {
        if (roll == green)
        {
            curBal = curBal + bet * 10;
            cout << "Congrats! You won " << bet * 10 << endl;
            return;
        }
    }
    curBal -= bet;
    cout << "You lost " << bet << " :(. Try again, surely you will have better luck!" << endl;
}
void dice(User player)
{
    srand(time(NULL));
    int roll1, roll2;
    roll1 = rand() % 6 + 1;
    roll2 = rand() % 6 + 1;
    cout << "Input bet" << endl;
    int bet;
    cin >> bet;
    cout << "Input number" << endl;
    int number;
    cin >> number;
    if (number == roll1 + roll2)
    {
        curBal = curBal + bet * 10;
        cout << "Congrats! You won " << bet << endl;
    }
    else
    {
        balance -= bet;
        cout << "You lost " << bet << " :(. Try again, surely you will have better luck!" << endl;
    }
}
