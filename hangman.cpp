/*
    hangman.cpp
        
    Method implementations for the hangman class.
    
    assignment: CSCI 262 Project - Evil Hangman        

    author:

    last modified: 3/7/2019
*/

#include "hangman.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;


map<int, vector<string>> indexed;

vector<string> word_list;
vector<int> indexed_char;

bool remaining_words;

char guessed_char;
char correct_char;

string guessed_string = "";

int guesses_num;
int len;
int largest_family = 0;

string word;
string final_word;

// constructor
hangman::hangman() { 
    // TODO: Read in and store words from dictionary.txt
    string x;
    ifstream inFile;
    int string_size;
    vector<string> strings_at_index;

    inFile.open("dictionary.txt");
    if (!inFile) {
        cout << "Could not open dictionary file!" << "\n";
        exit(1);
    }

    while (inFile >> x) {
        strings_at_index.clear();
        string_size = x.size();
        if (indexed.count(string_size) > 0) {
            indexed.find(string_size)->second.push_back(x);
        } else {
            strings_at_index.push_back(x);
            indexed[x.size()] = strings_at_index;
        }
    }
}


// start_new_game()
//
// Setup a new game of hangman.
void hangman::start_new_game(int num_guesses, int word_len, bool get_remaining_words) {
    // TODO: Initialize game state variables
    guesses_num = num_guesses;
    len = word_len;
    remaining_words = get_remaining_words;
    word_list = indexed.find(len)->second;

    word = "";
    for (int i = 0; i < len; i++) {
        word += "-";
    }
}


// process_guess()
//
// Process a player's guess - should return true/false depending on whether
// or not the guess was in the hidden word.  If the guess is incorrect, the
// remaining guess count is decreased.
bool hangman::process_guess(char c) {

    correct_char = NULL;

    vector<int> indexes;

    vector<string> new_vect;

    vector<int> key_vector;

    map<vector<int>, vector<string>> vector_map;

    for (int i = 0; i < word_list.size(); i++) {
        indexes.clear();
        new_vect.clear();
        for (int j = 0; j < len; j++) {
            if (word_list.at(i).at(j) == c) {
                indexes.push_back(j);
            }
        }
        if (vector_map.count(indexes) > 0) {
            vector_map.find(indexes)->second.push_back(word_list.at(i));
        } else {
            new_vect.push_back(word_list.at(i));
            vector_map[indexes] = new_vect;
        }
    }

    largest_family = 0;
    for (auto j : vector_map) {
        if (j.second.size() > largest_family) {
            largest_family = j.second.size();
            key_vector = j.first;
            if (j.second.at(0).find(c)) {
                correct_char = c;
            }
        }
    }

    indexed_char = key_vector;

    word_list = vector_map.find(key_vector)->second;

        if (remaining_words) {
            cout << "Chosen Family Size: " << largest_family << "\n";
        }

        final_word = word_list.at(0);
        guessed_char = c;
        guesses_num = guesses_num - 1;
        if (guessed_char) {
            return true;
        }
        return false;
}


// get_display_word()
//
// Return a representation of the hidden word, with unguessed letters
// masked by '-' characters.
string hangman::get_display_word() {
    for (int i = 0; i < indexed_char.size(); i++) {
        word.at(indexed_char.at(i)) = correct_char;
    }

    return word;
}


// get_guesses_remaining()
//
// Return the number of guesses remaining for the player.
int hangman::get_guesses_remaining() {
    return guesses_num;
}


// get_guessed_chars()
//
// What letters has the player already guessed?  Return in alphabetic order.
string hangman::get_guessed_chars() {
    guessed_string += guessed_char;
    sort(guessed_string.begin(), guessed_string.end());
    return guessed_string;
}


// was_char_guessed()
//
// Return true if letter was already guessed.
bool hangman::was_char_guessed(char c) {
    for (int i = 0; i < guessed_string.size(); i++) {
        if (guessed_string.at(i) == c) {
            return true;
        }
    }
    return false;
}


// is_won()
//
// Return true if the game has been won by the player.
bool hangman::is_won() {
    int count = 0;
    for (int i = 0; i < word.size(); i++) {
        if (word.at(i) == '-') {
            count++;
        }
    }
    if (count > 0) {
        return false;
    } else {
        return true;
    }
}


// is_lost()
//
// Return true if the game has been lost.
bool hangman::is_lost() {
    if (guesses_num == 0) {
        return true;
    }
    return false;
}


// get_hidden_word
//
// Return the true hidden word to show the player.
string hangman::get_hidden_word() {
    return final_word;
}


