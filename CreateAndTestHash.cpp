#include "QuadraticProbing.h"
#include "LinearProbing.h"
#include "DoubleHashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


template <typename HashTableType>
TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  cout << "TestFunctionForHashTables..." << endl;
  cout << "Words filename: " << words_filename << endl;
  cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  
  
  fstream f(words_filename);
  string s;

  //insert words into table
  while(f >> s){
    hash_table.Insert(s);
  }

  int c = hash_table.collisions;
  int n = hash_table.num_elements;
  int t = hash_table.getTableSize();

  cout << "Collisions: " << c << endl; 
  cout << "Number of items: " << n << endl;
  cout << "Size of hash table: " << t << endl; 
  cout << "Load factor: " << (float)(n)/(float)(t) << endl; 
  cout << "Avg. number of collisions: " << (float)(c)/(float)(n) << endl; 

  //find if words in query are in table, print out probes.

  fstream query(query_filename);
  int probes = 0;
  string word;

  while(query >> word){
    if(hash_table.Contains(word,probes)){
      cout << word << " found " << probes << endl;
    }else{
      cout << word << " not found " << probes << endl;
    }

    probes = 0;
  }

}

// Sample main for program CreateAndTestHash
int
main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);

  if (param_flag == "linear") {
    HashTableLinear<string> linear_probing_table;
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
  } else if (param_flag == "double") {
    HashTableDouble<string> double_probing_table;
    TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
  } else {
    cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}
