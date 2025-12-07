#include <cstdio>
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include <cstdlib>
#include <ctime>

#define NOMINMAX
#include <windows.h>

using namespace std;

// Function declarations
void cleanCin();
void optionOutput();
void winner();
void looser();
void wordInput(string& word);
void playGame(string& word);
int chooseLevel();
bool timeIsUp(DWORD start, int limitSec);
void singlePlayer(string& word);
void multiPlayer();
void multiplayerMenu();

// Clean input 
void cleanCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Menu output
void optionOutput() {
    cout << "\n====================\n";
    cout << "        MENU        \n";
    cout << "====================\n";
    cout << "1: Single Player\n";
    cout << "2: Multiplayer\n";
    cout << "0: Quit\n";
    cout << "Your choice: ";
}

// Difficulty & Timer 
int chooseLevel() {
    cout << "\nChoose difficulty:\n";
    cout << "1. Easy (10 attempts)\n";
    cout << "2. Medium (8 attempts)\n";
    cout << "3. Hard (6 attempts)\n";
    cout << "Your choice: ";

    char c;
    cin >> c;
    cleanCin();

    switch (c) {
    case '1': return 10;
    case '3': return 6;
    default:  return 8;
    }
}

bool timeIsUp(DWORD start, int limitSec) {
    DWORD now = GetTickCount();
    return (now - start) / 1000 >= limitSec;
}

//  Hangman stages 
const char* hangmanStage[] = {
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

//  Winner/Looser 
void winner() {
    cout << "\n CONGRATULATIONS! YOU WON! \n\n";
    cout << R"(                   
           _______                                                     _______
\     /   |       |    |       |          \          /\          /    |       |     |\     |
 \   /    |       |    |       |           \        /  \        /     |       |     | \    |
  \ /     |       |    |       |            \      /    \      /      |       |     |  \   |
   |      |       |    |       |             \    /      \    /       |       |     |   \  |
   |      |       |    |       |              \  /        \  /        |       |     |    \ |
   |      |_______|    |_______|               \/          \/         |_______|     |     \|
)";
}

void looser() {
    cout << "\n GAME OVER! YOU LOST! \n\n";
    cout << R"(                   
           _______                                     ________       _______     ___________             
\     /   |       |    |       |          |           |        |     |                 |
 \   /    |       |    |       |          |           |        |     |                 |
  \ /     |       |    |       |          |           |        |     |______           |
   |      |       |    |       |          |           |        |            |          |
   |      |       |    |       |          |           |        |            |          |
   |      |_______|    |_______|          |_______    |________|      ______|          |
)";
}

// Word input 
void wordInput(string& word) {
    const char* bannedSymbols = "0123456789!@#$%^&*()_+-=<>?/\\|[]{},.;:'\" ";
    word.clear();

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD originalMode;
    GetConsoleMode(hStdin, &originalMode);

    SetConsoleMode(hStdin, originalMode & ~ENABLE_ECHO_INPUT);

    cout << "The word must not contain numbers, symbols, or spaces\n> ";

    INPUT_RECORD record;
    DWORD events;
    char ch;

    while (true) {
        ReadConsoleInput(hStdin, &record, 1, &events);

        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
            ch = record.Event.KeyEvent.uChar.AsciiChar;

            if (ch == '\r') {
                cout << endl;
                break;
            }
            if (ch == '\b') {
                if (!word.empty()) {
                    word.pop_back();
                    cout << "\b \b";
                }
            }
            else if (isalpha((unsigned char)ch)) {
                word.push_back(ch);
                cout << '*';
            }
        }
    }

    if (word.empty()) {
        cout << " You didn’t type anything!\n";
        wordInput(word);
    }

    for (size_t i = 0; i < word.size(); ++i) {
        const char c = word[i];
        for (const char* p = bannedSymbols; *p != '\0'; ++p) {
            if (c == *p) {
                cout << " Invalid word! Only letters allowed!\n";
                word.clear();
                wordInput(word);
                break;
            }
        }
        if (word.empty()) break;
    }

    SetConsoleMode(hStdin, originalMode);
}

//  Play game 
void playGame(string& word) {
    SetConsoleOutputCP(CP_UTF8);

    int maxAttempts = chooseLevel();
    DWORD startTimer = GetTickCount();
    int timeLimitSec = 30;

    char charToGuess;
    string foundChars;
    int maxWrong = 0;
    string wordOutput(word.size(), '_');

    while (true) {

        if (timeIsUp(startTimer, timeLimitSec)) {
            system("cls");
            cout << "\  TIME IS UP!\n";
            looser();
            return;
        }

        system("cls");
        cout << hangmanStage[min(maxWrong, 7)] << "\n";
        cout << "Time left: " << timeLimitSec - ((GetTickCount() - startTimer) / 1000) << "s\n";
        cout << "Attempts left: " << (maxAttempts - maxWrong) << "\n";

        cout << "Word: ";
        for (size_t i = 0; i < wordOutput.size(); ++i) cout << wordOutput[i] << " ";
        cout << "\nGuessed letters: ";
        for (size_t i = 0; i < foundChars.size(); ++i) cout << foundChars[i] << " ";
        cout << "\nTake a guess: ";

        bool found = false;
        cin >> charToGuess;
        cleanCin();

        if (foundChars.find(charToGuess) != string::npos) {
            cout << " You already tried '" << charToGuess << "'!\n";
            getchar();
            continue;
        }

        foundChars.push_back(charToGuess);

        for (size_t i = 0; i < word.size(); i++) {
            if (word[i] == '0') continue;

            if (word[i] == charToGuess) {
                found = true;
                wordOutput[i] = charToGuess;
                word[i] = '0';
            }
        }

        if (wordOutput.find('_') == string::npos) {
            system("cls");
            winner();
            return;
        }

        if (!found) {
            maxWrong++;
            if (maxWrong >= maxAttempts) {
                system("cls");
                looser();
                cout << hangmanStage[7];
                cout << "\nWord was: " << wordOutput << "\n";
                return;
            }
        }
    }
}

//  Single Player 
void singlePlayer(string& word) {
	const char* words[] = { "cat", "dog", "bird", "seal", "monkey", "butterfly", "jellyfish", "apple", "summer", "window", "garden", "music", "computer",
	"programming", "elephant", "football", "library", "pyramid", "yellow", "pencil", "river", "mountain", "chocolate", "airplane", " rainbow", " telescope", 
    " volcano", " astronaut", " dinosaur", " submarine", "chicken" };
    const int count = sizeof(words) / sizeof(words[0]);
    int idx = rand() % count;
    string selected = words[idx];
    playGame(selected);
}

//  Multiplayer Menu 
void multiplayerMenu() {
    string word1, word2;
    int score1 = 0, score2 = 0;

    while (true) {
        cout << "\n====================\n";
        cout << "   MULTIPLAYER MENU  \n";
        cout << "====================\n";
        cout << "1: One enters word, other guesses\n";
        cout << "2: Two-player duel (take turns)\n";
        cout << "0: Back to main menu\n";
        cout << "Your choice: ";

        char choice;
        cin >> choice;
        cleanCin();

        switch (choice) {
        case '1':
            wordInput(word1);
            if (!word1.empty()) {
                system("cls");
                playGame(word1);
                word1.clear();
            }
            break;

        case '2':
            cout << "Player 1, enter your word:\n";
            wordInput(word1);
            cout << "Player 2, enter your word:\n";
            wordInput(word2);

            if (!word1.empty() && !word2.empty()) {
                system("cls");
                cout << "Player 2 guessing Player 1's word:\n";
                playGame(word1);
                score2++;

                system("cls");
                cout << "Player 1 guessing Player 2's word:\n";
                playGame(word2);
                score1++;

                cout << "\nScores:\nPlayer 1: " << score1 << "\nPlayer 2: " << score2 << "\n";
                cout << "Press enter to continue...\n";
                getchar();
            }
            break;

        case '0':
            return;

        default:
            cout << "Invalid option\n";
            break;
        }
    }
}

//  Main Multiplayer 
void multiPlayer() {
    multiplayerMenu();
}

//  Main 
int main() {

    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned)time(NULL));
    string word;

    cout << R"(
             _____________
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
    )";

    optionOutput();

    char option;
    while (true) {
        option = getchar();
        cleanCin();

        switch (option) {
        case '1':
            singlePlayer(word);
            break;

        case '2':
            multiPlayer();
            break;

        case '0':
            return 0;

        default:
            cout << "Invalid option\n";
            break;
        }
        optionOutput();
    }
    return 0;
}
