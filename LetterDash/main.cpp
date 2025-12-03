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

//declarations of all functions


//clean input buffer
void cleanCin();

//prints options
void optionOutput();

//prints you won
void winner();

//prints you lost
void looser();

//word input for multyplayer
void wordInput(string& word);

//the logic of game
void playGame(string& word);


//FUNCTION DESCRIPTION
// clean input buffer
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

//functions display if you won of lost with big ascii letters
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
    cout << R"(                   _______                                     ________      _______     ____________              
        \     /   |       |    |       |          |           |        |     |                 |
         \   /    |       |    |       |          |           |        |     |                 |
          \ /     |       |    |       |          |           |        |     |______           |
           |      |       |    |       |          |           |        |            |          |
           |      |       |    |       |          |           |        |            |          |
           |      |_______|    |_______|          |_______    |________|      ______|          |
 void wordInput(string& word) {

    )";
}

// "Enter a word" function
void wordInput(string& word) {
    const char* bannedSymbols = "0123456789!@#$%^&*()_+-=<>?/\\|[]{},.;:'\" ";
    word.clear();

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD originalMode;
    GetConsoleMode(hStdin, &originalMode);

    // Disable echo input (don't disable line input)
    SetConsoleMode(hStdin, originalMode & ~ENABLE_ECHO_INPUT);

    cout << "The word must not contain numbers, symbols, or spaces\n> ";

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
            } else if (isalpha((unsigned char)ch)) { // accept only letters
                word.push_back(ch);
                cout << '*';
            }
        }
    }

    if (word.empty()) {
        cout << "⚠  You didn’t type anything!\n";
        wordInput(word); // retry
    }

    // Validate banned symbols just in case
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

    //  Restore console mode
    SetConsoleMode(hStdin, originalMode);
}

//Function to start playing 
void playGame(string& word){
    SetConsoleOutputCP(CP_UTF8);
    char toEnter;
    char charToGuess;
    string foundChars;
    int maxWrong = 0;
    string wordOutput(word.size(), '_');

    while (true) {
        system("cls");

        cout << hangmanStage[maxWrong] << "\n";
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
                cout << " Correct! '" << charToGuess << "' is in the word!\n";
                wordOutput[i] = charToGuess;
                word[i] = '0';
                toEnter = getchar();
            }
        }

        //if there is no more underscores, the player has won
        if(wordOutput.find('_') == string::npos){
            system("cls");
            winner();
            return;
        }

        if(!found){
            maxWrong++;
            cout << " Wrong! '" << charToGuess << "' is not in the word!\n";
            cout << " You have " << 8 - maxWrong << " attempts left!\n";
            toEnter = getchar();
            //if the player has reached max wrong attempts, they lose
            if(maxWrong == 8){
                system("cls");
                looser();
                cout << hangmanStage[7];
                cout << "Word was: " << wordOutput << "\n";
                return;
            }
       }
       found = false;
    }

}

// single player and multiplayer helper functions
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

    cout << "\n====================\n";
    cout << "       MENU\n";
    cout << "====================\n";
    cout << "1: Single Player\n";
    cout << "2: Multiplayer\n";
    cout << "0: Quit\n";
    cout << "Your choice: ";

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
