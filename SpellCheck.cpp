#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <algorithm>

#include "QuadraticProbing.h"
using namespace std;

//checks to see if string has any punctuation returns string without it
string checkString(string s){
    if(s[0] == '.' || s[0] == ','){
        return s.substr(1, s.length()-1);
    }
    else if(s[s.length()-1] == '.' || s[s.length()-1] == ','){
        return s.substr(0, s.length()-1);
    }
    else{
        return s;
    }
}

//implements rule A when finding a misspelled word:
//"Adding one character in each possible position"
vector<string> add(string s, HashTable<string> &table, vector<string> &poss){
    string tempString = s;

    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for(int i = 0; i <= s.length(); ++i){
        for(int j = 0; j < alphabet.length(); ++j){
            //insert every letter of the alphabet using string 'alphabet' and take out each character of it to test
            tempString.insert(i, 1 ,alphabet[j]);
            if(table.Contains(tempString)) poss.push_back(tempString);
            tempString = s;
        }       
    }
    return poss;
}

//implements rule B when finding a misspelled word:
//"Removing one character from the word"
vector<string> remove(string s, HashTable<string> &table, vector<string> &poss){
    string tempString = s;

    for(int i = 0; i < s.length(); ++i){
        //removes character from string at position i
        tempString.erase(i, 1);
        if(table.Contains(tempString)) poss.push_back(tempString);
        tempString = s;
    }
    return poss;
}

//implements rule C when finding a misspelled word:
//"Swapping adjacent characters in the word"
vector<string> swap(string s, HashTable<string> &table, vector<string> &poss){
    string tempString = s;
    for(int i = 0; i < s.length()-1; ++i){
        //use std's swap function
        std::swap(tempString[i], tempString[i+1]);
    
        if(table.Contains(tempString)) poss.push_back(tempString);

        tempString = s;
    }

    return poss;
}

int main(int argc, char **argv){
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <documentfilename> <dictionaryfilename>" << endl;
        return 0;
    }
    
    const string document_file(argv[1]);
    const string dictionary_file(argv[2]);

    //Input words from dictionary into table
    fstream dic(dictionary_file);
    string s;

    HashTable<string> table;
    while(dic >> s){
        table.Insert(s);    
    }
    //-------

    // Read document and check every word for mispells

    vector<string> misspelled;
    ifstream doc;
    doc.open(document_file);

    stringstream ss;
    string t;

    //get all misspelled words from file 
    while(getline(doc, t)){
       ss.clear();
       ss << t;
       while(ss >> t){
           //check if string has any punctuation and strip it
           t = checkString(t);
           //convert string to all lowercase so it doesn't affect its 'spelling' in the dictionary
           transform( t.begin(), t.end(), t.begin(), ptr_fun <int, int> ( tolower ) );
           if(!table.Contains(t)) misspelled.push_back(t);
       }
    }

    //all of the suggestions for misspelled words are stored in suggestions vector. Each function used to 
    //create a possible suggestion pushes its suggestions to the vector.

    vector<string> suggestions;
    for(int i = 0; i < misspelled.size(); i++){
        suggestions.clear();

        //run three rules on misspelled word
        add(misspelled[i], table, suggestions);
        remove(misspelled[i], table, suggestions);
        swap(misspelled[i], table, suggestions);

        //print out all suggestions made from rules ^^^
        for(int j = 0; j < suggestions.size(); ++j){
            cout << suggestions[j] << " ";
        }    
        cout << endl;
    }

    
    return 0;
}