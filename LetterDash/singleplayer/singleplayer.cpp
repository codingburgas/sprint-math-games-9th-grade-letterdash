#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <exception>
#include <cstdlib>
#include <limits>
#include <ctime>

// Prevent windows.h from defining min/max macros that break std::numeric_limits::max()
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

using namespace std;

// Function to display the main menu
void optionOutput() {
    cout << "\n====================\n";
    cout << "        MENU        \n";
    cout << "====================\n";
    cout << "2: Play Game\n";
    cout << "0: Quit\n";
    cout << "Your choice: ";
}

// Hangman stages
vector<string> hangmanStage{
    R"(
         _________________
            |/      |    
            |      
            |      
            |      
            |      
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         _________________
            |/      |    
            |      ( )
            |      
            |       
            |      
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         _________________
            |/      |    
            |      ( )
            |       |
            |       |
            |        
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         _________________
            |/      |    
            |      ( )
            |       |/
            |       |
            |       
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         _________________
            |/      |    
            |      ( )
            |      \|/
            |       |
            |      
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         _________________
            |/      |    
            |      ( )
            |      \|/
            |       |
            |        \
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         _________________
            |/      |    
            |      ( )
            |      \|/
            |       |
            |      / \
            |
            _____________
           /            /
          /            /
         /____________/
    )",
    R"(
         __________________
            |/ \    |    /
            |   \__( )__/
            |      \|/
            |       |
            |      / \
            |
            _____________
           /            /
          /            /
         /____________/
    )"
};

// Clean input buffer
void cleanCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to display a winner message with ASCII art
void winner() {
    cout << "\n CONGRATULATIONS! YOU WON! \n\n";
    cout << R"(                   _______                                                     _______
        \     /   |       |    |       |          \          /\          /    |       |     |\     |
         \   /    |       |    |       |           \        /  \        /     |       |     | \    |
          \ /     |       |    |       |            \      /    \      /      |       |     |  \   |
           |      |       |    |       |             \    /      \    /       |       |     |   \  |
           |      |       |    |       |              \  /        \  /        |       |     |    \ |
           |      |_______|    |_______|               \/          \/         |_______|     |     \|
    )";
}

// Function to display a loser message with ASCII art
void looser() {
    cout << "\n GAME OVER! YOU LOST! \n\n";
    cout << R"(                   _______                                     ________      _______     ____________              
        \     /   |       |    |       |          |           |        |     |                 |
         \   /    |       |    |       |          |           |        |     |                 |
          \ /     |       |    |       |          |           |        |     |______           |
           |      |       |    |       |          |           |        |            |          |
           |      |       |    |       |          |           |        |            |          |
           |      |_______|    |_______|          |_______    |________|      ______|          |
    )";
}

// Function for the player to guess a word in single-player mode
void playSinglePlayer();
void playGame(string& word);

void playSinglePlayer() {
    // Predefined words for the single player game
    vector<string> words = { "cat", "dog", "bird", "seal", "monkey", "butterfly", "jellyfish" };
    srand(time(0));  // Seed random number generator
    string word = words[rand() % words.size()];  // Randomly choose a word from the list

    // Start game
    playGame(word);
}

// Function for the player to input a word and start the game
void wordInput(string& word) {
    string bannedSymbols = "0123456789!@#$%^&*()_+-=<>?/\\|[]{},.;:'\" ";
    word.clear();

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD originalMode;
    GetConsoleMode(hStdin, &originalMode);

    // Disable echo input (don't disable line input)
    SetConsoleMode(hStdin, originalMode & ~ENABLE_ECHO_INPUT);

    cout << "The word must not contain numbers, symbols, or spaces\n> ";

    try {
        INPUT_RECORD record;
        DWORD events;
        char ch;

        while (true) {
            ReadConsoleInput(hStdin, &record, 1, &events);

            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
                ch = record.Event.KeyEvent.uChar.AsciiChar;

                if (ch == '\r') {  // Enter
                    cout << endl;
                    break;
                }
                if (ch == '\b') {  // Backspace
                    if (!word.empty()) {
                        word.pop_back();
                        cout << "\b \b";
                    }
                }
                else if (isalpha(ch)) { // accept only letters
                    word.push_back(ch);
                    cout << '*';
                }
            }
        }

        if (word.empty()) {
            throw runtime_error(u8" You didn’t type anything!");
        }

        // Validate banned symbols just in case
        for (char c : word) {
            if (bannedSymbols.find(c) != string::npos) {
                throw runtime_error(u8" Invalid word! Only letters allowed!");
            }
        }

    }
    catch (const exception& e) {
        cout << e.what() << "\n";
        word.clear();
        wordInput(word); // retry
    }

    //  Restore console mode even if exception occurred
    SetConsoleMode(hStdin, originalMode);
}

// Function to start the game
void playGame(string& word) {
    SetConsoleOutputCP(CP_UTF8);

    char charToGuess;
    string foundChars;
    int maxWrong = 0;
    string wordOutput(word.size(), '_');

    while (true) {
        system("cls");

        cout << hangmanStage[maxWrong] << "\n";
        cout << "Word: ";
        for (char c : wordOutput) cout << c << " ";
        cout << "\nGuessed letters: ";
        for (char c : foundChars) cout << c << " ";
        cout << "\nTake a guess: ";

        bool found = false;
        cin >> charToGuess;

        if (foundChars.find(charToGuess) != string::npos) {
            cout << u8" You already tried '" << charToGuess << "'!\n";
            cleanCin();
            continue;
        }

        cleanCin();
        foundChars.push_back(charToGuess);

        for (size_t i = 0; i < word.size(); i++) {
            if (word[i] == '0') continue;

            if (word[i] == charToGuess) {
                found = true;
                cout << u8" Correct! '" << charToGuess << "' is in the word!\n";
                wordOutput[i] = charToGuess;
                word[i] = '0';  // Mark the letter as guessed
                system("pause");
            }
        }

        // Check if the word has been fully guessed
        if (wordOutput.find('_') == string::npos) {
            system("cls");
            winner();
            // cout << "Word was: " << wordOutput << "\n"; // Optional, to show the word
            return;  // End the game if the word is completely guessed
        }

        if (!found) {
            maxWrong++;
            cout << u8" Wrong! '" << charToGuess << "' is not in the word!\n";
            cout << u8" You have " << 8 - maxWrong << " attempts left!\n";
            system("pause");

            // If the player reaches the maximum number of wrong attempts, they lose
            if (maxWrong == 8) {
                system("cls");
                looser();
                cout << hangmanStage[7];  // Show the final hangman state
                cout << "Word was: " << wordOutput << "\n";
                return;  // End the game
            }
        }
    }
}

// Simple program entry point
int main() {
    SetConsoleOutputCP(CP_UTF8);

    while (true) {
        optionOutput();
        int choice;
        if (!(cin >> choice)) {
            cleanCin();
            continue;
        }
        cleanCin();

        if (choice == 0) {
            break;
        }
        else if (choice == 2) {
            playSinglePlayer();
        }
        else {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}



