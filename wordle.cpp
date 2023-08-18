/*
Description: Wordle is a web-based word game developed by Welsh-born software engineer Josh Wardle.
Players have six attempts to guess a five-letter word; feedback is given for each guess, in the form of
colored tiles, indicating when letters match or occupy the correct position. Yellow highlighted characters (in this case '&')
indicate that the letter is in the solution but not in that position. Green highlighted characters
indicate (in this case '!') that the letter is in the correct location of the solution. Grey highlighted characters (in this case '-') indicate
that the letter is not in the solution at all.
*/

// Compile command: g++ -std=c++20 -o wordle wordle.cpp
// Run command: ./wordle

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;



//Global constants (let L = letter and P = position)
const int ALLOWED_GUESSES = 6;
const int TOTAL_WORDS = 2315;
const string CORRECT_LP_SYMBOL = "!";
const string CORRECT_L_SYMBOL = "&";
const string INCORRECT_L_SYMBOL = "-";



//Function prototypes
void importWords (string word_array[]);
string generateWord (string word_array[], int array_size);
void verifyInput (string target, string input, string word_array[]);
void displayGame (string display[][6], int row_size, string target, string input, int hint_index, int input_index);
void runGame (string word_array[]);



int main () {

    string word_list[TOTAL_WORDS] = {""};

    importWords (word_list);
    runGame (word_list);

    return 0;
}



// Function name: importWords
// Pre-condition: There must exist a text file of a list of words to fill the word_array. The array word_array starts as an empty array.
// Post-condition: The array word_array is filled with words from an imported text file.
void importWords (string word_array[]) {
    // This portion is for importing the file and creating an array filled with the list of words from the file
    string extracted_word;
    int index_counter = 0; //Counter for populating each index of the empty array with each word from the imported file
    
    ifstream myfile("vocab_data.txt");

    if (myfile.is_open()) {
        while (myfile >> extracted_word) {
            word_array[index_counter] = extracted_word; //Fills the array with the words from the imported file
            index_counter++;
        }
    } else {
        cout << "Unable to open file" << endl;
    }
    // This portion is for debugging
    // for (int i = 0; i < TOTAL_WORDS; i++) {
    //     cout << word_array[i] << endl;
    // }
    myfile.close();

}



// Function name: generateWord
// Pre-condition: The time library ctime must be included. The array word_array can't be empty.
// Post-condition: A mystery word will be selected by random.
string generateWord (string word_array[], int array_size) {

    // This portion is the pseudo-random number generator that is used to determine what the target word will be.
    const int MIN = 0;
    const int MAX = array_size;
    int randomly_determined_number = 0;
    string randomly_determined_word;

    srand(time(NULL));
    // cout << time(NULL) << endl; // This line is for debugging

    randomly_determined_number = rand() % (MAX - MIN + 1) + MIN;
    randomly_determined_word = word_array[randomly_determined_number];

    return randomly_determined_word;
}



// Function name: verifyInput
// Pre-condition: The string target (mystery word) must already be determined. There must be a player present to input something to assign to string input (guessed word). The array word_array must be populated.
// Post-condition: Either the player's guessed word matches the mystery word and wins the game, or the player could run out of guesses and lose the game - in eather cases, the player will be prompted.
void verifyInput (string target, string input, string word_array[]) {

    string guess_display[ALLOWED_GUESSES * 2][6] = {{""}, {"-", "-", "-", "-", "-", "\n"}, {""}, {"-", "-", "-", "-", "-", "\n"}, {""}, {"-", "-", "-", "-", "-", "\n"}, {""}, {"-", "-", "-", "-", "-", "\n"}, {""}, {"-", "-", "-", "-", "-", "\n"}, {""}, {"-", "-", "-", "-", "-", "\n"}};
    
    int number_of_guesses = 0; //Counter for the number of guesses
    int hint_display_index = 1;
    int input_display_index = 0;

    do {
        cout << "What word would you like to guess?" << endl;
        cout << "Guesses left: " << ALLOWED_GUESSES - number_of_guesses << "/6" << endl;
        cout << "\nYour word: ";
        cin >> input;
        cout << endl;



        //Validates whether the user's input datatype is a string, five letters long, and exists in the imported word list
        bool exists_in_word_list = false;        
        do {
            for (int i = 0; i < TOTAL_WORDS; i++) {
                if (input == word_array[i]) {
                    exists_in_word_list = true;
                }
            }
            if (exists_in_word_list == false) {
                cout << "That word doesn't belong in the word list.\n" << endl;
                cin.clear();
                cin.ignore(256, '\n');

                cout << "Your word: ";
                cin >> input;
                cout << endl;
            }
        }
        while (cin.fail() || (exists_in_word_list == false) || (input.length() < 5 || input.length() > 5));
        number_of_guesses++;



        displayGame (guess_display, ALLOWED_GUESSES * 2, target, input, hint_display_index, input_display_index);
        input_display_index += 2;
        hint_display_index += 2;
    }
    while ((number_of_guesses < ALLOWED_GUESSES) && (input != target));



    if (input == target) {
        cout << "You've succesfully guessed the mystery word!\n" << endl;
    } else {
        cout << "You ran out of guesses.\n" << endl;
    }
}



// Function name: runGame
// Pre-condition: The array word_array must be populated with words. The player must be present in order to interact with the game.
// Post-condition: The game can either run again with a new mystery word loaded or end based on the choice the player makes from the options prompt.
void runGame (string word_array[]) {

   // This portion allows for the user to play again or quit the game
    int choice = 1;
    do {
        string mystery_word;
        string input_word;

        mystery_word = generateWord (word_array, TOTAL_WORDS);

        verifyInput (mystery_word, input_word, word_array);

        cout << "Mystery word: " << mystery_word << endl; //Displays the mystery word to the player after the player has won or run out of attempts.
        cout << "\nWhat would you like to do?" << endl;
        cout << "1. Play again" << endl;
        cout << "2. Quit game" << endl;
        cout << "Enter your choice (1 or 2): ";
        cin >> choice;

        while (choice < 1 || choice > 2) {
            cout << "Please enter a valid choice (1 or 2): ";
            cin >> choice;
        }
    }
    while (choice != 2);

    cout << "\nThanks for playing!" << endl;
}



// Function name: displayGame
// Pre-condition: In Wordle, the player can only have 6 guesses for a mystery word, this means that constant int ALLOWED_GUESSES has to have a value of six.
// Post-condition: The array display will be filled with words that the player has guessed up to the number of guesses that they needed to match the mystery word or up to the max number of guesses. The array display will also be filled with hints as to which letters are correct position and which are correct and in the correct position.
void displayGame (string display[][6], int row_size, string target, string input, int hint_index, int input_index) {

    //This portion displays the guessed words from the user as well as the hints towards their similarity through symbols.
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (input[j] == target[i] && (j != i)) {
                display[hint_index][j] = CORRECT_L_SYMBOL;
            } else if ((input[j] == target[i]) && (j == i)) {
                display[hint_index][i] = CORRECT_LP_SYMBOL;
            }
        }
    }
    
    display[input_index][0] = input;
    
    for (int i = 0; i < (ALLOWED_GUESSES * 2); i++) {
        for (int j = 0; j < 6; j++) {
            cout << display[i][j];
        }
        cout << endl;
    }
}