#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <exception>

#include <windows.h>
#include <cstdlib>
#include <limits>

using namespace std;

//function that displays the menu
void optionOutput(){
    cout << "\n====================\n";
    cout << "        MENU        \n";
    cout << "====================\n";
    cout << "1: Add New Word\n";
    cout << "2: Play Game\n";
    cout << "0: Quit\n";
    cout << "Your choice: ";
}

//hangman stages
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



//functions display if you won of lost with big ascii letters
void winner(){
    cout << "\n🎉 CONGRATULATIONS! YOU WON! 🎉\n\n";
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
    cout << "\n💀 GAME OVER! YOU LOST! 💀\n\n";
    cout << R"(                   _______                                     ________      _______     ____________              
        \     /   |       |    |       |          |           |        |     |                 |
         \   /    |       |    |       |          |           |        |     |                 |
          \ /     |       |    |       |          |           |        |     |______           |
           |      |       |    |       |          |           |        |            |          |
           |      |       |    |       |          |           |        |            |          |
           |      |_______|    |_______|          |_______    |________|      ______|          |
 
    )";
}


//clean input buffer
void cleanCin(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// "Enter a word" function
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
                } else if (isalpha(ch)) { // accept only letters
                    word.push_back(ch);
                    cout << '*';
                }
            }
        }

        if (word.empty()) {
            throw runtime_error(u8"⚠️  You didn’t type anything!");
        }

        // Validate banned symbols just in case
        for (char c : word) {
            if (bannedSymbols.find(c) != string::npos) {
                throw runtime_error(u8"🚫 Invalid word! Only letters allowed!");
            }
        }

    } catch (const exception& e) {
        cout << e.what() << "\n";
        word.clear();
        wordInput(word); // retry
    }

    // ✅ Restore console mode even if exception occurred
    SetConsoleMode(hStdin, originalMode);
}
//Function to start playing 
void playGame(string& word){
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

        if(foundChars.find(charToGuess) != string::npos){
            cout << u8"⚠️  You already tried '" << charToGuess << "'!\n";  
            cleanCin();
            continue;
        }

        cleanCin();
        foundChars.push_back(charToGuess);

        for(size_t i = 0; i < word.size(); i++){
            if(word[i] =='0') continue;

            if(word[i] == charToGuess ){
                found = true;
                cout << u8"✅ Correct! '" << charToGuess << "' is in the word!\n";
                wordOutput[i] = charToGuess;
                word[i] = '0';
                system("pause");
            }
        }

        //if there is no more underscores, the player has won
        if(wordOutput.find('_') == string::npos){
            system("cls");
            winner();
            // cout << "Word was: " << wordOutput << "\n";
            return;      
        }

        if(!found){
            maxWrong++;
            cout << u8"❌ Wrong! '" << charToGuess << "' is not in the word!\n";    
            cout << u8"💀 You have " << 8 - maxWrong << " attempts left!\n";
            system("pause");
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

int main() {

    SetConsoleOutputCP(CP_UTF8);

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
        
    
         
         ====================     
                MENU
         ====================
        1: Add New Word
        2: Play Game
        0: Quit

        Your choice:  )";

    int option;
    while (true) {
    cin >> option;
    cleanCin(); 

    switch (option) {
        case 1:
            wordInput(word);
            // restore console mode after hidden input
            system("cls");
            break;

        case 2:
            playGame(word);
            word = "";
            break;

        case 0:
            return 0;

        default:
            cout << "Invalid option\n";
            break;
    }
    optionOutput();
    }
    return 0;
}
