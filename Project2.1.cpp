/*
 * Author: Maricela Rodriguez
 * Date: July 18 at 5:38pm
 * Purpose: 7 Up and 7 Down
 */
//System Libraries Here
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <cctype>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

// Function Prototypes
void displayWelcomeMessage(string &name);
float getStartingMoney();
void gameInstructions();
void playGame(string name, float &money, bool &jackpot);
void placeBet(float &dealer, float money);
void rollDice(int &dice1, int &dice2, int &result);
void processBet(int bet, int result, float &money, float dealer, string name, bool &jackpot);
bool checkJackpot(int result, int bet, bool &jackpot);
bool playAgain();

// Main Function
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    string name;
    float money;
    bool jackpot = false;

    displayWelcomeMessage(name);
    money = getStartingMoney();
    gameInstructions();

    do {
        playGame(name, money, jackpot);
    } while (playAgain() && money >= 1);

    cout << "Thanks for playing!" << endl;
    return 0;
}

// Function Definitions

void displayWelcomeMessage(string &name) {
    cout << fixed << setprecision(2);
    cout << "Welcome! What's your name?  ";
    cin >> name;
    name[0] = toupper(name[0]);
    cout << " Well, " << name << ", it's nice to meet you. This is how you play the game.";
    cout << " This game is called 7 Up 7 Down. The dealer is going to roll a pair of dice" << endl;
    cout << "and you're going to try to predict and bet on whether the number you rolled is lower than 7, higher than 7, or it equals 7. " << endl;
}

float getStartingMoney() {
    float money;
    cout << "You must start off with $1 or more to play." << endl << endl;
    cout << "First of all, how much money do you want to start with?" << endl;
    cout << "$";
    cin >> money;
    money = static_cast<float>(money);

    while (1 > money) {
        cout << "Not enough money to play. Please enter the correct amount." << endl;
        cin >> money;
        money = static_cast<float>(money);
    }
    return money;
}

void gameInstructions() {
    cout << "Now that you have some money to bet with. Let's start." << endl << endl;
    cout << "Choose wisely. " << endl << endl;
}

void playGame(string name, float &money, bool &jackpot) {
    int dice1, dice2, result, bet;
    float dealer;

    rollDice(dice1, dice2, result);

    cout << "Choose one of the following options, either 1 to 3. " << endl;
    cout << "Choice 1: Number is lower than 7." << endl;
    cout << "Choice 2: Number is higher than 7. " << endl;
    cout << "Choice 3: Number is equal to 7." << endl;
    cin >> bet;

    cout << endl << "How much would you like to bet?\t\tCurrent Balance $" << money << endl;
    placeBet(dealer, money);

    while (bet < 1 || bet > 3) {
        cout << "Invalid bet. Please choose a number from 1-3: ";
        cin >> bet;
    }

    for (int i = 1; i <= 3; i++) {
        cout << "Loading..." << endl;
    }
    cout << "...Dice Rolling..." << endl;

    processBet(bet, result, money, dealer, name, jackpot);
}

void placeBet(float &dealer, float money) {
    cin >> dealer;
    while (dealer > money) {
        cout << "Invalid bet not enough money. Please enter the correct amount." << setw(4) << "$" << money << endl << "$";
        cin >> dealer;
    }
}

void rollDice(int &dice1, int &dice2, int &result) {
    dice1 = rand() % 6 + 1; // [1-6]
    dice2 = rand() % 6 + 1; // [1-6]
    result = dice1 + dice2; // Sum of the two dice
}

void processBet(int bet, int result, float &money, float dealer, string name, bool &jackpot) {
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result <= 6 && bet == 1) {
                cout << "Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                money += dealer * 2;
            } else if (result >= 7 && bet == 1) {
                cout << "Sorry, you lost! Try again." << endl;
                money -= dealer;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result >= 8 && bet == 2) {
                cout << " Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                money += dealer * 2;
            } else if (result < 8 && bet == 2) {
                cout << "Sorry " << name << ", you lost! Give it another shot, maybe you'll win this time." << endl;
                money -= dealer;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (checkJackpot(result, bet, jackpot)) {
                cout << "Wow, you're super lucky! You won, " << name << "!" << endl;
                money += (dealer * 4);
            } else {
                cout << "Well this is awkward... Sorry to break it to you " << name << ", you lost." << endl;
                money -= dealer;
            }
            break;
    }
    cout << "Current Money $" << money << endl;
}

bool checkJackpot(int result, int bet, bool &jackpot) {
    if (result == 7 && bet == 3) {
        if (jackpot) {
            jackpot = false;
            return true;
        } else {
            jackpot = true;
            return true;
        }
    }
    return false;
}

bool playAgain() {
    char exit;
    cout << endl << " Would you like to play the game again? Press (y/n): ";
    cin >> exit;
    cout << endl << endl;
    return exit == 'y' || exit == 'Y';
}
