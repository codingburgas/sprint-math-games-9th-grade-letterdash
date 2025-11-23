#include <iostream>
#include <string>
#include <limits>

using namespace std;

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
