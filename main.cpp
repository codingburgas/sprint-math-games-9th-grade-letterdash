#include <iostream>
#include <string>

using namespace std;

int main(){
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
