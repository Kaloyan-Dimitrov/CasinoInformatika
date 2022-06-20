#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <ctime>
#include <filesystem>
using namespace std;


struct User
{
    bool admin = false;
    string username;
    string password;
    string fname;
    string lname;
    string EGN;
    string birthdate;
    int initialBalance = 100; // initial balance
    int currentBalance = 100; // current balance
    int gameHistory[1000];
    int gamesPlayed = 0;
};
struct Casino
{
    string name;
    int numUsers;
};

Casino casinos[10];
string casinosNames[10];
int casinosCount = 0;
int currentCasinoIndex = -1;

User users[100];
int currentUserIndex = -1;

string availableGames[] = { "Roulette", "Dice" };
int numGames = 2;

// helper functions for options chooser
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // used for goto
COORD CursorPosition; // used for goto
COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}
void gotoXY(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}
int chooser(string text, string options[], int optionsLength) {

    int menu_item = 0, y;
    bool choosing = true;
    int startingY = GetConsoleCursorPosition(console).Y + 1;
    gotoXY(0, startingY); cout << text;
    gotoXY(0, startingY + 1); cout << "->";
    startingY += 2;
    y = startingY;
    gotoXY(0, startingY); cout << "->";
    while (choosing)
    {
        for (int i = 0; i < optionsLength; i++) {
            gotoXY(4, i + startingY);  cout << i + 1 << ") " << options[i];
        }

        system("pause>nul"); // the >nul bit causes it the print no message

        if (GetAsyncKeyState(VK_DOWN) && y < startingY + optionsLength - 1) //down button pressed
        {
            gotoXY(0, y); cout << "  ";
            y++;
            gotoXY(0, y); cout << "->";
            menu_item++;
            continue;
        }

        if (GetAsyncKeyState(VK_UP) && y > startingY) //up button pressed
        {
            gotoXY(0, y); cout << "  ";
            y--;
            gotoXY(0, y); cout << "->";
            menu_item--;
            continue;
        }

        if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed
            cout << "\nYou chose: \"" << options[menu_item] << "\"" << endl;
            choosing = false;
        }
    }
    return menu_item;
}


// casino helpers
void readCasinos(string filename) {
    ifstream casinosFile;
    casinosFile.open(".//casinos.txt");
    casinosFile >> casinosCount;
    for (int i = 0; i < casinosCount; i++) {
        casinosFile >> casinos[i].name;
        casinosFile >> casinos[i].numUsers;
        casinosNames[i] = casinos[i].name;
    }
    casinosFile.close();
}
void saveCasinos() {
    ofstream casinosFile;
    casinosFile.open(".//casinos.txt");
    casinosFile << casinosCount << endl;
    for (int i = 0; i < casinosCount; i++) {
        casinosFile << casinos[i].name << " ";
        casinosFile << casinos[i].numUsers << endl;
    }
    casinosFile.close();
}
int createCasino() {
    Casino newCasino;
    newCasino.numUsers = 0;
    cout << "Give your casino a suitable one-word (can't contain intervals) name: ";
    cin >> newCasino.name;

    cout << "And that's it! Your new casino is created!";

    namespace fs = filesystem;
    fs::create_directories("./" + newCasino.name);

    casinos[casinosCount] = newCasino;
    casinosCount++;
    
    return casinosCount - 1;
}


// user helpers
int createUser() {
    User newUser;

    string options[] = { "player", "admin" };
    newUser.admin = chooser("Do you want this user to be a player or an admin?", options, 2);
   
    cout << "First name: ";
    cin >> newUser.fname;
    cout << "Last name: ";
    cin >> newUser.lname;
    cout << "EGN: ";
    cin >> newUser.EGN;
    int year;
    cout << "Year of birth (ex. 2002): ";
    cin >> year;
    if (2022 - year < 18) {
        cout << "You are too young for this establishment!! Goodbye!";
        return -1;
    }
    cout << "Whole date of birth (ex. 12.02.2000): ";
    cin >> newUser.birthdate;

    cout << "Enter your initial balance (ex. 200): ";
    cin >> newUser.initialBalance;
    newUser.currentBalance = newUser.initialBalance;
    cout << "Create a username for your account (ex. TheDestroyer230): ";
    cin >> newUser.username;
    cout << "Type in a password for your account: ";
    cin >> newUser.password;

    casinos[currentCasinoIndex].numUsers++;
    users[casinos[currentCasinoIndex].numUsers - 1] = newUser;

    return casinos[currentCasinoIndex].numUsers - 1;
}
void saveUsers() {
    ofstream usersFile;
    usersFile.open(".//" + casinos[currentCasinoIndex].name + ".//users.txt");
    for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
        usersFile << users[i].admin << " ";
        usersFile << users[i].username << " ";
        usersFile << users[i].password << " ";
        usersFile << users[i].fname << " ";
        usersFile << users[i].lname << " ";
        usersFile << users[i].EGN << " ";
        usersFile << users[i].birthdate << " ";
        usersFile << users[i].initialBalance << " ";
        usersFile << users[i].currentBalance << " ";
        usersFile << users[i].gamesPlayed << " ";
        for (int j = 0; j < users[i].gamesPlayed; j++) {
            usersFile << users[i].gameHistory[j] << " ";
        }
        usersFile << endl;
    }
    usersFile.close();
}
void readUsers() {
    ifstream usersFile;
    usersFile.open(".//" + casinos[currentCasinoIndex].name + ".//users.txt");
    for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
        usersFile >> users[i].admin;
        usersFile >> users[i].username;
        usersFile >> users[i].password;
        usersFile >> users[i].fname;
        usersFile >> users[i].lname;
        usersFile >> users[i].EGN;
        usersFile >> users[i].birthdate;
        usersFile >> users[i].initialBalance;
        usersFile >> users[i].currentBalance;
        usersFile >> users[i].gamesPlayed;
        for (int j = 0; j < users[i].gamesPlayed; j++) {
            usersFile >> users[i].gameHistory[j];
        }
    }
    usersFile.close();
}
int userLogin() {
    cout << "Login"<< endl;
    string username, password;
    cout << "Enter your username for this casino: ";
    cin >> username;
    int userIndex = -1;
    for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
        if (users[i].username == username) {
            userIndex = i;
            break;
        }
    }
    while (userIndex == -1) {
        cout << "We couldn't find a user with the given username." << endl;
        string options[] = { "Try Again to login", "Create a new account" };
        int registr = chooser("Would you like to try again or maybe register as a new user?", options, 2);
        if (registr) return createUser();
        cout << "Enter your username for this casino: ";
        cin >> username;
        int userIndex = -1;
        for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
            if (users[i].username == username) {
                userIndex = i;
                break;
            }
        }
    }
    cout << "Enter your password: ";
    cin >> password;
    while (password != users[userIndex].password) {
        cout << "The passwords don't match. ";
        string options[] = { "Enter password again", "Create a new account" };
        int registr = chooser("Would you like to try again or maybe register as a new user?", options, 2);
        if (registr) return createUser();
        cout << "Enter your password: ";
        cin >> password;
    }
    return userIndex;
}
void printUser(User u) {
    cout << "First name: ";
    cout << u.fname << endl;
    /// DOPULNI DA IZVEJDA VS DETAILI



    if (u.admin || u.gamesPlayed == 0) return;
    cout << "Average loss: ";
    float avl = 0;
    for (int i = 0; i < u.gamesPlayed; i++) {
        avl += u.gameHistory[i];
    }
    avl /= u.gamesPlayed;
    cout << avl << endl;
}

// games
void roulette(User *player)
{
    int *curBal = &((*player).currentBalance);
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
                *curBal += bet;
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
                *curBal += bet;
                cout << "Congrats! You won " << bet << endl;
                return;
            }
        }
    }
    if (colour == "Green")
    {
        if (roll == green)
        {
            *curBal = *curBal + bet * 10;
            cout << "Congrats! You won " << bet * 10 << endl;
            return;
        }
    }
    *curBal = *curBal - bet;
    cout << "You lost " << bet << " :(. Try again, surely you will have better luck!" << endl;
}
void dice(User *player)
{
    int* curBal = &((*player).currentBalance);
    int* gamesPlayed = &((*player).gamesPlayed);
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
        *curBal = (*curBal) + bet * 10;
        cout << "Congrats! You won " << bet << endl;
        users[currentUserIndex].gameHistory[*gamesPlayed] = 0 + bet * 10;
    }
    else
    {
        *curBal = (*curBal) - bet;
        cout << "You lost " << bet << " :(. Try again, surely you will have better luck!" << endl;
        users[currentUserIndex].gameHistory[*gamesPlayed] = 0 - bet;
    }
    *gamesPlayed = *gamesPlayed + 1;
}


int main()
{
    readCasinos(".//casinos.txt");
    cout << "Hello!" << endl;
    cout << "Welcome to Online Casino Manager 2000!!" << endl;
    if (casinosCount < 10) {
        string options[] = { "Play in existing", "Create a new one" };
        int createNew = chooser("Would you like to play in an existing casino, or create a new one?", options, 2);
        if (createNew) currentCasinoIndex = createCasino();
    }
    if (currentCasinoIndex == -1) currentCasinoIndex = chooser("Choose a casino:", casinosNames, casinosCount);

    if (casinos[currentCasinoIndex].numUsers < 1) {
        cout << "There aren't any registered users in this casino yet. (To login there must be at least one user registered)\n";
        cout << "Create user:" << endl;
        currentUserIndex = createUser();
        if (currentUserIndex == -1) return 0;
    }
    else {
        readUsers();


        string options[] = { "Login", "Register" };
        int registr = chooser("Do you want to login or to register?", options, 2);
        if (registr) currentUserIndex = createUser();


        else currentUserIndex = userLogin();
        if (currentUserIndex == -1) return 0;
    }
    if (users[currentUserIndex].admin) {
        string options[] = { "Print all users", "Find user by username", "Sort users by current balance and print them", "Sort users by average loss" };
        int action = chooser("What would you like to do?", options, 4);
        switch (action) {
            case 0: {
                for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                    printUser(users[i]);
                    cout << "-----------------------" << endl;
                }
                break;
            }
            case 1: {
                string username;
                cout << "Search for username: ";
                cin >> username;
                int usersFound = 0;
                for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                    if (users[i].username.find(username) != string::npos) {
                        printUser(users[i]);
                        cout << "-----------------------" << endl;
                        usersFound++;
                    }
                }

                while (usersFound == 0) {
                    cout << "We couldn't find a user with the given username." << endl;
                    string options[] = { "Yes", "No" };
                    int notSearch = chooser("Would you like to try again?", options, 2);
                    if (notSearch) break;
                    cout << "Search for username: ";
                    cin >> username;
                    for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                        if (users[i].username.find(username) != string::npos) {
                            printUser(users[i]);
                            cout << "-----------------------" << endl;
                            usersFound++;
                        }
                    }
                }
                break;
            }
            case 2: { // Sort users by current balance and print them
                for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                    for (int j = i + 1; j < casinos[currentCasinoIndex].numUsers; j++)
                    {
                        if (users[j].currentBalance < users[i].currentBalance) {
                            User temp = users[i];
                            users[i] = users[j];
                            users[j] = temp;
                        }
                    }
                }
                for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                    printUser(users[i]);
                    cout << "-----------------------" << endl;
                }
                break;
            }
            case 3: { // sort users by average loss and print them
                for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                    for (int j = i + 1; j < casinos[currentCasinoIndex].numUsers; j++)
                    {
                        float javl = 0, iavl = 0;
                        for (int k = 0; k < users[i].gamesPlayed; k++) {
                            iavl += users[i].gameHistory[k];
                        }
                        iavl /= users[i].gamesPlayed;

                        for (int k = 0; k < users[j].gamesPlayed; k++) {
                            javl += users[j].gameHistory[k];
                        }
                        javl /= users[j].gamesPlayed;

                        if (javl < iavl) {
                            User temp = users[i];
                            users[i] = users[j];
                            users[j] = temp;
                        }
                    }
                }
                for (int i = 0; i < casinos[currentCasinoIndex].numUsers; i++) {
                    printUser(users[i]);
                    cout << "-----------------------" << endl;
                }
                break;
            }
        }
    }

    else {
        string options[] = { "Play a game", "View my profile", "Input money", "Withdraw money" };
        int action = chooser("What would you like to do?", options, 4);
        switch (action) {
            case 0: {
                int game = chooser("Which game would you like to play?", availableGames, numGames);
                switch (game) {
                    case 0: {
                        roulette(&users[currentUserIndex]);
                        break;
                    }
                    case 1: {
                        dice(&users[currentUserIndex]);
                        break;
                    }
                }
                break;
            }
            case 1: { // view profile
                printUser(users[currentUserIndex]);
                break;
            }
            case 2: { // deposit money
                int money = 0;
                cout << "Input how much money you would like to deposit: ";
                cin >> money;
                users[currentUserIndex].currentBalance += money;
                break;
            }
            case 3: { // withdraw money
                int money = 0;
                cout << "Input how much money you would like to withdraw: ";
                cin >> money;
                users[currentUserIndex].currentBalance -= money;
                break;
            }
        }

    }
    saveUsers();
    saveCasinos();
    cout << "Bye! Type anything and press enter to exit.\n";
    int m;
    cin >> m;
    return 0;
}