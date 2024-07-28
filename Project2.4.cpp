#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <cctype>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

// Function Prototypes
void showMsg(string &name);
float getStartMoney();
void showInstr();
void playGame(string name, float &cash, bool *jack = nullptr);
void placeBet(float &bet, float cash);
void rollDice(int &die1, int &die2, int &sum);
void processBet(int bet, int sum, float &cash, float betAmt);
void processBet(int bet, int sum, float &cash, float betAmt, string name);
void processBet(int bet, int sum, float &cash, float betAmt, string name, bool *jack);
bool checkJack(int sum, int bet, bool &jack);
bool playAgain();
void showGameCount();
void showSummary(float startCash, float endCash);
void endGame(float startCash, float endCash, const string &name);

// Main Function
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    string name;
    float cash, startCash;
    bool jack = false;

    showMsg(name);
    cash = getStartMoney();
    startCash = cash; // Track the initial amount of money
    showInstr();

    do {
        playGame(name, cash, &jack);
        showGameCount();
        if (cash <= 0) {
            cout << "You have run out of money. Would you like to add more money to continue playing? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                cash = getStartMoney();
                startCash += cash; // Add to the initial amount of money
            } else {
                cout << "Thanks for playing!" << endl;
                endGame(startCash, cash, name); // Call endGame function
                return 0;
            }
        }
    } while (cash > 0 && playAgain());

    endGame(startCash, cash, name); // Call endGame function
    return 0;
}

// Function Definitions

void showMsg(string &name) {
    cout << fixed << setprecision(2);
    cout << "Welcome! What's your name?  ";
    cin >> name;
    name[0] = toupper(name[0]);
    cout << " Well, " << name << ", it's nice to meet you. This is how you play the game.";
    cout << " This game is called 7 Up 7 Down. The dealer is going to roll a pair of dice" << endl;
    cout << "and you're going to try to predict and bet on whether the number you rolled is lower than 7, higher than 7, or it equals 7. " << endl;
}

float getStartMoney() {
    float cash;
    cout << "You must start off with $1 or more to play." << endl << endl;
    cout << "First of all, how much money do you want to start with?" << endl;
    cout << "$";
    cin >> cash;

    while (cash < 1) {
        cout << "Not enough money to play. Please enter the correct amount: $";
        cin >> cash;
    }
    return cash;
}

void showInstr() {
    cout << "Now that you have some money to bet with. Let's start." << endl << endl;
    cout << "Choose wisely. " << endl << endl;
}

void playGame(string name, float &cash, bool *jack) {
    int numBets;
    cout << "How many bets would you like to place? ";
    cin >> numBets;

    // Use a 2D array to store dice results and bets
    int** res = new int*[numBets];
    for (int i = 0; i < numBets; ++i) {
        res[i] = new int[2]; // Store dice results for each bet
    }
    int* bets = new int[numBets];
    float* betAmt = new float[numBets];

    for (int i = 0; i < numBets; i++) {
        if (cash < 1) {
            cout << "You don't have enough money to continue betting. Please add more money." << endl;
            cash = getStartMoney();
        }

        cout << "Bet " << (i + 1) << ":" << endl;

        cout << "Choose one of the following options, either 1 to 3. " << endl;
        cout << "Choice 1: Number is lower than 7." << endl;
        cout << "Choice 2: Number is higher than 7. " << endl;
        cout << "Choice 3: Number is equal to 7." << endl;
        cin >> bets[i];

        while (bets[i] < 1 || bets[i] > 3) {
            cout << "Invalid bet. Please choose a number from 1-3: ";
            cin >> bets[i];
        }

        cout << endl << "How much would you like to bet?\t\tCurrent Balance $" << cash << endl;
        placeBet(betAmt[i], cash);

        for (int j = 1; j <= 3; j++) {
            cout << "Loading..." << endl;
        }
        cout << "...Dice Rolling..." << endl;

        int die1, die2;
        rollDice(die1, die2, res[i][0]);
        res[i][1] = die1 + die2; // Store total dice result

        // Call the overloaded function with different parameters
        processBet(bets[i], res[i][1], cash, betAmt[i], name, jack);
        // Uncomment the following line to test the overloaded functions without name or jackpot
        // processBet(bets[i], res[i][1], cash, betAmt[i]);
    }

    // Clean up 2D array
    for (int i = 0; i < numBets; ++i) {
        delete[] res[i];
    }
    delete[] res;
    delete[] bets;
    delete[] betAmt;
}

void placeBet(float &bet, float cash) {
    cin >> bet;
    while (bet > cash || bet <= 0) {
        cout << "Invalid bet. Please enter an amount between 1 and " << cash << ": $";
        cin >> bet;
    }
}

void rollDice(int &die1, int &die2, int &sum) {
    die1 = rand() % 6 + 1; // [1-6]
    die2 = rand() % 6 + 1; // [1-6]
    sum = die1 + die2; // Sum of the two dice
    cout << "Dice 1: " << die1 << ", Dice 2: " << die2 << ", Total: " << sum << endl;
}

void processBet(int bet, int sum, float &cash, float betAmt) {
    // Default implementation without name or jackpot
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum <= 6) {
                cout << "Congratulations, you win!" << endl;
                cash += betAmt * 2;
            } else {
                cout << "Sorry, you lost!" << endl;
                cash -= betAmt;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum >= 8) {
                cout << "Congratulations, you win!" << endl;
                cash += betAmt * 2;
            } else {
                cout << "Sorry, you lost!" << endl;
                cash -= betAmt;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum == 7) {
                cout << "Wow, you're super lucky! You won!" << endl;
                cash += (betAmt * 4);
            } else {
                cout << "Sorry, you lost." << endl;
                cash -= betAmt;
            }
            break;
    }
    cout << "Current Money $" << cash << endl << endl;
}

void processBet(int bet, int sum, float &cash, float betAmt, string name) {
    // Overloaded version with name
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum <= 6) {
                cout << "Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                cash += betAmt * 2;
            } else {
                cout << "Sorry, you lost! Try again." << endl;
                cash -= betAmt;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum >= 8) {
                cout << " Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                cash += betAmt * 2;
            } else {
                cout << "Sorry " << name << ", you lost! Give it another shot, maybe you'll win this time." << endl;
                cash -= betAmt;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum == 7) {
                cout << "Wow " << name << ", you're super lucky! You won! Are you a wizard?!" << endl;
                cash += (betAmt * 4);
            } else {
                cout << "Sorry " << name << ", you lost." << endl;
                cash -= betAmt;
            }
            break;
    }
    cout << "Current Money $" << cash << endl << endl;
}

void processBet(int bet, int sum, float &cash, float betAmt, string name, bool *jack) {
    // Overloaded version with name and jackpot
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum <= 6) {
                cout << "Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                cash += betAmt * 2;
                if (checkJack(sum, bet, *jack)) {
                    cout << "Jackpot! Extra $100 added!" << endl;
                    cash += 100;
                }
            } else {
                cout << "Sorry, you lost!" << endl;
                cash -= betAmt;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum >= 8) {
                cout << " Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                cash += betAmt * 2;
                if (checkJack(sum, bet, *jack)) {
                    cout << "Jackpot! Extra $100 added!" << endl;
                    cash += 100;
                }
            } else {
                cout << "Sorry " << name << ", you lost! Give it another shot, maybe you'll win this time." << endl;
                cash -= betAmt;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << sum << "!" << endl;
            if (sum == 7) {
                cout << "Wow " << name << ", you're super lucky! You won! Are you a wizard?!" << endl;
                cash += (betAmt * 4);
                if (checkJack(sum, bet, *jack)) {
                    cout << "Jackpot! Extra $100 added!" << endl;
                    cash += 100;
                }
            } else {
                cout << "Sorry " << name << ", you lost." << endl;
                cash -= betAmt;
            }
            break;
    }
    cout << "Current Money $" << cash << endl << endl;
}

bool checkJack(int sum, int bet, bool &jack) {
    if (sum == 7 && bet == 3) {
        jack = true;
        return true;
    }
    return false;
}

bool playAgain() {
    char ans;
    cout << "Would you like to play again? (y/n): ";
    cin >> ans;
    return (ans == 'y' || ans == 'Y');
}

void showGameCount() {
    static int gameCnt = 0;
    gameCnt++;
    cout << "Games played so far: " << gameCnt << endl;
}

void showSummary(float startCash, float endCash) {
    cout << "Starting Money: $" << startCash << endl;
    cout << "Final Money: $" << endCash << endl;
}

void endGame(float startCash, float endCash, const string &name) {
    showSummary(startCash, endCash);
    cout << "Thank you for playing, " << name << "! See you next time!" << endl;
}

