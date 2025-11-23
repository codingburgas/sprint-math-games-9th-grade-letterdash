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
    
    cout << R"(Menu:
    1: Add New Word
    2: Play Game
    0: Quit

    You pick: )";
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
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

// "Enter a word" function
void wordInput(string& word) {
    string bannedSymbols = "0123456789!@#$%^&*()_+-=<>?/\\|[]{},.;:'\" ";

    cout << "The word must not contain numeric characters, symbols, or spaces\n";
    cout << "Enter a word: ";
    while (true) {
        try {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "> ";

            getline(cin, word);


            if (word.empty()) throw runtime_error("The word must not be empty");

            for (size_t i = 0; i < word.size(); i++) {
                if (bannedSymbols.find(word[i]) != string::npos) {
                    throw std::runtime_error("The word must not contain numeric characters, symbols, or spaces");
                }

            }
            break;
        }
        catch (const exception& e) {
            cout << e.what() << "\n";
        }
    }
}

//Function to start playing 
void playGame(string& word){
    if(word.empty()){
        // cout << "\033[3J\033[H\033[2J";
        system("clear");
        cout << "THE WORD MUST NOT BE EMPTY\n\n";
        return;
    }
    char charToGuess;
    vector<char> foundChars;
    int maxWrong = 0;
    string wordOutput(word.size(), '_');

    while (true) {
       cout << "Take a guess!\n>";
       bool found = false;
        

       cin >> charToGuess;
       cleanCin();  
       
       for(size_t i = 0; i < word.size();i++){
       
        if(word[i] =='0') continue;


        if(word[i] == charToGuess ){
            found = true;
            wordOutput[i] = charToGuess;
            word[i] = '0';

            foundChars.push_back(charToGuess);
        }
       }
  
       if(wordOutput.find('_') == string::npos){
        // cout << "\033[3J\033[H\033[2J";
        system("clear");
        winner();
        cout << "\n";
        
        return;      
       }

       if(!found){
        maxWrong++;
            if(maxWrong == 8){
                // cout << "\033[3J\033[H\033[2J";
                system("clear");
                looser();
                cout << hangmanStage[7];
                return;   
            }   
        cout << hangmanStage[maxWrong-1];
        cout << "There is no \'" << charToGuess << "\' in the word" << endl;
       }
       
       for(size_t i = 0; i < wordOutput.size();i++ )
       {
        cout << wordOutput[i];
       }
       
       

       found = false;
       cout << "\n";
    }
}

int main() {
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
            Menu:
            1: Add New Word
            2: Play Game
            0: Quit

            You pick: )";

    int option;
    while (true) {

        cin >> option;

        switch (option) {
        case 1:
            wordInput(word);
            system("clear");
            break;

        case 2:

            word = "";
            break;

        case 0:
            return 0;

        default:
            cout << "Invalid option\n";
            break;

        }
    }
    return 0;
}
