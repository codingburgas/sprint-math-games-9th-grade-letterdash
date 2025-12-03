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

//clean input buffer
void cleanCin(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//function that displays the menu
void optionOutput(){
    cout << "\n====================\n";
    cout << "        MENU        \n";
    cout << "====================\n";
    cout << "1: Single Player\n";
    cout << "2: Multiplayer\n";
    cout << "0: Quit\n";
    cout << "Your choice: ";
}

//=======================
// NEW: Difficulty choice
//=======================
int chooseLevel() {
    cout << "\nChoose difficulty:\n";
    cout << "1. Easy (10 attempts)\n";
    cout << "2. Medium (8 attempts)\n";
    cout << "3. Hard (6 attempts)\n";
    cout << "Your choice: ";

    char c;
    cin >> c;
    cleanCin();

    switch(c) {
        case '1': return 10;
        case '2': return 8;
        case '3': return 6;
        default: return 8;
    }
}

//=======================
// NEW: Simple 30s timer
//=======================
bool timeIsUp(DWORD start, int limitSeconds) {
    DWORD now = GetTickCount();
    return (now - start) / 1000 >= limitSeconds;
}

//hangman stages
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

void winner(){
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

void looser(){
    cout << "\n GAME OVER! YOU LOST! \n\n";
    cout << R"(                   
           _______                                     ________      _______     ____________              
\     /   |       |    |       |          |           |        |     |                 |
 \   /    |       |    |       |          |           |        |     |                 |
  \ /     |       |    |       |          |           |        |     |______           |
   |      |       |    |       |          |           |        |            |          |
   |      |       |    |       |          |           |        |            |          |
   |      |_______|    |_______|          |_______    |________|      ______|          |
)";
}

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
            } else if (isalpha((unsigned char)ch)) {
                word.push_back(ch);
                cout << '*';
            }
        }
    }

    if (word.empty()) {
        cout << "⚠  You didn’t type anything!\n";
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

void playGame(string& word){
    SetConsoleOutputCP(CP_UTF8);
    char toEnter;
    char charToGuess;
    string foundChars;

    //===========================
    // NEW: difficulty + timer
    //===========================
    int maxAttempts = chooseLevel();
    DWORD timerStart = GetTickCount();
    int timeLimit = 30;

    int maxWrong = 0;
    string wordOutput(word.size(), '_');

    while (true) {

        // CHECK TIMER
        if (timeIsUp(timerStart, timeLimit)) {
            cout << "\n\n⏰ TIME IS UP!\n";
            looser();
            return;
        }

        system("cls");

        cout << hangmanStage[min(maxWrong, 7)] << "\n";
        cout << "Time left: " << timeLimit - (int)((GetTickCount() - timerStart)/1000) << "s\n";

        cout << "Word: ";
        for (size_t i = 0; i < wordOutput.size(); ++i) cout << wordOutput[i] << " ";
        cout << "\nGuessed letters: ";
        for (size_t i = 0; i < foundChars.size(); ++i) cout << foundChars[i] << " ";
        cout << "\nTake a guess: ";

        bool found = false;
        cin >> charToGuess;

        if(foundChars.find(charToGuess) != string::npos){
            cout << " You already tried '" << charToGuess << "'!\n";
            cleanCin();
            continue;
        }

        cleanCin();
        foundChars.push_back(charToGuess);

        for(size_t i = 0; i < word.size(); i++){
            if(word[i] =='0') continue;

            if(word[i] == charToGuess ){
                found = true;
                wordOutput[i] = charToGuess;
                word[i] = '0';
            }
        }

        if(wordOutput.find('_') == string::npos){
            system("cls");
            winner();
            return;
        }

        if(!found){
            maxWrong++;

            cout << " Wrong! '" << charToGuess << "' is not in the word!\n";
            cout << " Attempts left: " << maxAttempts - maxWrong << "\n";

            if(maxWrong >= maxAttempts){
                system("cls");
                looser();
                cout << hangmanStage[7];
                cout << "Word was: " << wordOutput << "\n";
                return;
            }
        }
    }
}

void singlePlayer(string& word) {
    const char* words[] = {"cat", "dog", "bird", "seal", "monkey", "butterfly", "jellyfish"};
    const int count = sizeof(words) / sizeof(words[0]);
    int idx = rand() % count;
    string selected = words[idx];
    playGame(selected);
}

void multiPlayer(string& word) {
    wordInput(word);
    if (!word.empty()) {
        system("cls");
        playGame(word);
        word.clear();
    }
}

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
            ________|_____
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
                multiPlayer(word);
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
