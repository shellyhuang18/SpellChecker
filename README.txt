All functions are working.

Use ./CreateAndTestHash <words file name> <query words file name> <flag>
 to use the CreateAndTestHash program.
where <flag> is used to test a certain hash function:
    linear
    quadratic
    double (R = 2)

CreateAndTestHash file outputs the:
    N:   number of elements in the table
    T:   size of table
    N/T: load factor
    C:   total number of collisions
    C/N: average number of collisions

The file also takes in a text file and checks to see if the words in the file are in the hash table
or not. Each instance of a search also prints out the number of probes used.



Use ./SpellCheck <document file> <dictionary file> 
to use the SpellCheck program.

SpellCheck takes in a dictionary file that acts as a dictionary. Words with correct spellings are located inside of the file.
The dictionary is put into a hashtable.

SpellCheck also takes in a document file, which it uses to find any misspelled words. 
Mispelled words are taken and treated in the following manner in order to find any possible correct spellings:
    -Adding one character in each possible position
    -Removing one character from the word
    -Swapping adjacent characters in the word

and outputs all possible suggestions(crosses it with words it finds in the dictionary file)
