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
void playGame(string name, float &money, bool *jackpot = nullptr);
void placeBet(float &dealer, float money);
void rollDice(int &dice1, int &dice2, int &result);
void processBet(int bet, int result, float &money, float dealer);
void processBet(int bet, int result, float &money, float dealer, string name);
void processBet(int bet, int result, float &money, float dealer, string name, bool *jackpot);
bool checkJackpot(int result, int bet, bool &jackpot);
bool playAgain();
void displayGameCount();
void displayFinalSummary(float startingMoney, float finalMoney);
void exitGame(float startingMoney, float finalMoney, const string &name);

// Main Function
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    string name;
    float money, startingMoney;
    bool jackpot = false;

    displayWelcomeMessage(name);
    money = getStartingMoney();
    startingMoney = money; // Track the initial amount of money
    gameInstructions();

    do {
        playGame(name, money, &jackpot);
        displayGameCount();
        if (money <= 0) {
            cout << "You have run out of money. Would you like to add more money to continue playing? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                money = getStartingMoney();
                startingMoney += money; // Add to the initial amount of money
            } else {
                cout << "Thanks for playing!" << endl;
                exitGame(startingMoney, money, name); // Call exitGame function
                return 0;
            }
        }
    } while (money > 0 && playAgain());

    exitGame(startingMoney, money, name); // Call exitGame function
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

    while (money < 1) {
        cout << "Not enough money to play. Please enter the correct amount: $";
        cin >> money;
    }
    return money;
}

void gameInstructions() {
    cout << "Now that you have some money to bet with. Let's start." << endl << endl;
    cout << "Choose wisely. " << endl << endl;
}

void playGame(string name, float &money, bool *jackpot) {
    int numBets;
    cout << "How many bets would you like to place? ";
    cin >> numBets;

    // Use a 2D array to store dice results and bets
    int** results = new int*[numBets];
    for (int i = 0; i < numBets; ++i) {
        results[i] = new int[2]; // Store dice results for each bet
    }
    int* bets = new int[numBets];
    float* dealers = new float[numBets];

    for (int i = 0; i < numBets; i++) {
        if (money < 1) {
            cout << "You don't have enough money to continue betting. Please add more money." << endl;
            money = getStartingMoney();
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

        cout << endl << "How much would you like to bet?\t\tCurrent Balance $" << money << endl;
        placeBet(dealers[i], money);

        for (int j = 1; j <= 3; j++) {
            cout << "Loading..." << endl;
        }
        cout << "...Dice Rolling..." << endl;

        int dice1, dice2;
        rollDice(dice1, dice2, results[i][0]);
        results[i][1] = dice1 + dice2; // Store total dice result

        // Call the overloaded function with different parameters
        processBet(bets[i], results[i][1], money, dealers[i], name, jackpot);
        // Uncomment the following line to test the overloaded functions without name or jackpot
        // processBet(bets[i], results[i][1], money, dealers[i]);
    }

    // Clean up 2D array
    for (int i = 0; i < numBets; ++i) {
        delete[] results[i];
    }
    delete[] results;
    delete[] bets;
    delete[] dealers;
}

void placeBet(float &dealer, float money) {
    cin >> dealer;
    while (dealer > money || dealer <= 0) {
        cout << "Invalid bet. Please enter an amount between 1 and " << money << ": $";
        cin >> dealer;
    }
}

void rollDice(int &dice1, int &dice2, int &result) {
    dice1 = rand() % 6 + 1; // [1-6]
    dice2 = rand() % 6 + 1; // [1-6]
    result = dice1 + dice2; // Sum of the two dice
    cout << "Dice 1: " << dice1 << ", Dice 2: " << dice2 << ", Total: " << result << endl;
}

void processBet(int bet, int result, float &money, float dealer) {
    // Default implementation without name or jackpot
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result <= 6) {
                cout << "Congratulations, you win!" << endl;
                money += dealer * 2;
            } else {
                cout << "Sorry, you lost!" << endl;
                money -= dealer;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result >= 8) {
                cout << "Congratulations, you win!" << endl;
                money += dealer * 2;
            } else {
                cout << "Sorry, you lost!" << endl;
                money -= dealer;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result == 7) {
                cout << "Wow, you're super lucky! You won!" << endl;
                money += (dealer * 4);
            } else {
                cout << "Sorry, you lost." << endl;
                money -= dealer;
            }
            break;
    }
    cout << "Current Money $" << money << endl << endl;
}

void processBet(int bet, int result, float &money, float dealer, string name) {
    // Overloaded version with name
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result <= 6) {
                cout << "Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                money += dealer * 2;
            } else {
                cout << "Sorry, you lost! Try again." << endl;
                money -= dealer;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result >= 8) {
                cout << " Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                money += dealer * 2;
            } else {
                cout << "Sorry " << name << ", you lost! Give it another shot, maybe you'll win this time." << endl;
                money -= dealer;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result == 7) {
                cout << "Wow " << name << ", you're super lucky! You won! Are you a wizard?!" << endl;
                money += (dealer * 4);
            } else {
                cout << "Sorry " << name << ", you lost." << endl;
                money -= dealer;
            }
            break;
    }
    cout << "Current Money $" << money << endl << endl;
}

void processBet(int bet, int result, float &money, float dealer, string name, bool *jackpot) {
    // Overloaded version with name and jackpot
    switch (bet) {
        case 1:
            cout << "Your bet is that the total you rolled is smaller than 7. Let's find out!" << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result <= 6) {
                cout << "Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                money += dealer * 2;
                if (checkJackpot(result, bet, *jackpot)) {
                    cout << "Jackpot! Extra $100 added!" << endl;
                    money += 100;
                }
            } else {
                cout << "Sorry, you lost!" << endl;
                money -= dealer;
            }
            break;
        case 2:
            cout << "Your bet is that the number rolled is bigger than 7. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result >= 8) {
                cout << " Congratulations " << name << ", you win! Are you some sort of wizard?!" << endl;
                money += dealer * 2;
                if (checkJackpot(result, bet, *jackpot)) {
                    cout << "Jackpot! Extra $100 added!" << endl;
                    money += 100;
                }
            } else {
                cout << "Sorry " << name << ", you lost! Give it another shot, maybe you'll win this time." << endl;
                money -= dealer;
            }
            break;
        case 3:
            cout << "Your bet is that you rolled a 7, let's see. " << endl << endl;
            cout << "You rolled a " << result << "!" << endl;
            if (result == 7) {
                cout << "Wow " << name << ", you're super lucky! You won! Are you a wizard?!" << endl;
                money += (dealer * 4);
                if (checkJackpot(result, bet, *jackpot)) {
                    cout << "Jackpot! Extra $100 added!" << endl;
                    money += 100;
                }
            } else {
                cout << "Sorry " << name << ", you lost." << endl;
                money -= dealer;
            }
            break;
    }
    cout << "Current Money $" << money << endl << endl;
}

bool checkJackpot(int result, int bet, bool &jackpot) {
    if (result == 7 && bet == 3) {
        jackpot = true;
        return true;
    }
    return false;
}

bool playAgain() {
    char answer;
    cout << "Would you like to play again? (y/n): ";
    cin >> answer;
    return (answer == 'y' || answer == 'Y');
}

void displayGameCount() {
    static int gameCount = 0;
    gameCount++;
    cout << "Games played so far: " << gameCount << endl;
}

void displayFinalSummary(float startingMoney, float finalMoney) {
    cout << "Starting Money: $" << startingMoney << endl;
    cout << "Final Money: $" << finalMoney << endl;
}

void exitGame(float startingMoney, float finalMoney, const string &name) {
    displayFinalSummary(startingMoney, finalMoney);
    cout << "Thank you for playing, " << name << "! See you next time!" << endl;
}
