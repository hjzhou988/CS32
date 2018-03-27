//
//  main.cpp
//  anagrams
//
//  Created by Huajun Zhou on 7/6/17.
//  Copyright © 2017 Huajun Zhou. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <istream>
#include <assert.h>
using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 10; // Max words that can be read in

//Function declarations
void Loop(string::size_type i, string::size_type max, string prefix, string rest, string permutations[], int& counter, string nonredun, int& nonredunsize);
int printPermutions(string prefix, string rest, string permutations[], int& counter);
void Match(string word, const string dict[], int size, string results[], int& counter);
bool In(char c, const string nonredun, int size);
void Match(string word, const string dict[], int size, string results[], int& counter);
void Loop2(int i, int max, string permutations[], const string dict[], int size, string results[], int& counter);

//Function definition
void countLines(istream& dictfile, string dict[], int& linecount)   // A new function that helps to write into "dict", also making sure the number of lines does not exceed MAXDICTWORDS.
{
    if (linecount >= MAXDICTWORDS)
    {
        cout<<"Word count exceeds max!"<<endl;
        exit(1);
    }
    if (!dictfile) return; // Stop when dictfile is read up.
    dictfile >> dict[0];
    linecount++;
    countLines(dictfile, dict+1, linecount);
}


int readDictionary(istream &dictfile, string dict[])
{
    int linecount=0;
    countLines(dictfile, dict, linecount);// Use the countLines function to write "dict".
    return linecount-1; //linecount includes the last line that does not have a word. So the number of words should be linecount -1. That is 25143.
}

int recursivePermute(string word, const string dict[], int size, string results[])
{
    string permutations[MAXDICTWORDS]; // Initialized a string array to store the possible permutations of "word".
    string prefix; // Initialize an empty prefix string for generating permutations.
    int Permcounter=0; // A counter to count the number of permutations.
    int Rescounter=0; // A counter to count the number of results.
    
    printPermutions(prefix, word,permutations,Permcounter); // generate permutations list without redundancy, write into string "permutations".
    
    //search each permutation in the dictionary. If match, write into the results list. If not match, jump to the next permutation.
    int i=0;
    int max=Permcounter;
    Loop2(i,max, permutations, dict, size, results, Rescounter );
    /*
    for (int i=0;i<Permcounter; i++)
    {
        Match(permutations[i], dict, size, results, Rescounter);
    }
     */ //"for" loop was replaced by recursion function Loop2.
    return Rescounter;
}
void Loop2(int i, int max, string permutations[], const string dict[], int size, string results[], int& counter) //The loop that checks each permutation in the dictionary.
{
    if (i >= max)
        return;
    else
        Match(permutations[i], dict, size, results, counter);
    Loop2(i+1, max, permutations, dict, size, results,counter );
}

void recurPrint(const string results[], int size)
{
    if (size==0) return;
    cout<< results[size-1]<<endl;
    recurPrint(results, size-1);
}

void Match(string word, const string dict[], int size, string results[], int& counter) // given "word", match it to the "dict" from the last word of "dict". If there is a match, increase the counter and write into results.
{
    if (size==0)
    {
        return; // when the size of dictionary becomes 0, it means no matches. So just return.
    }
    if (word == dict[size-1]) // When there is a match
    {
        if (counter== MAXRESULTS)
            return; // if counter exceeds MAXRESULTS, return
        results[counter]= word; // write into results
        counter++;
        return;
    }
    else Match(word, dict, size-1, results, counter);
    
}

int printPermutions(string prefix, string rest, string permutations[], int& counter) // Get non-redundant permutations, given a string. Write into string permutations, and count how many non-redundant permutations total.
{
    if (rest.size()==0) // This is the base of the recursion.
    {
    permutations[counter]=prefix; //Write the premutation into the permutation array.
    counter++;
    }
    else
    {
        string nonredun; // Define a nonredundent string of characters, which will collect the nonredundant characters from "word".
        int nonredunsize=0; //Number of the characters in "nonredun".
        /*
        for (int i=0; i<rest.size();)
        {
         
            if (In(rest[i],nonredun,nonredunsize)) i++;
            else 
            {
                nonredun +=rest[i]; nonredunsize++;
         
            string prefixtemp=prefix; // create a string with the value of prefix that can be modified later in the loop. If we directly modify prefix in the loop, prefix value will be incremented every time when executing the loop.
            prefixtemp += rest[i];//Add the character to the end of prefix.
            string c; // create a string that stores the modified string "rest".
            c += rest.substr(0, i);
            c +=rest.substr(i+1, rest.size()-1-i);//
            printPermutions (prefixtemp, c, permutations, counter);//Use recursion to generate permutations with the updated values for prefix and rest.
            i++;
        }
        */ //"for" loop was replaced by recursion function "Loop"
        string::size_type i=0;
        string::size_type max=rest.size();
        Loop(i, max, prefix, rest, permutations, counter, nonredun, nonredunsize);
    }
    return counter;// finally return the total number of permutations.
}
bool In(char c, const string nonredun, int size) // A recursion function that checks whether character c already exists in string "nonredun".
{
    if (size==0) return false;
    if (c==nonredun[size-1]) return true;
    return In(c, nonredun, size-1);
}

void Loop(string::size_type i, string::size_type max, string prefix, string rest, string permutations[], int& counter, string nonredun, int& nonredunsize) // The recursion loop that helps to generate all possible permutations.
{
    if (i >= max)
        return;
    if (In(rest[i],nonredun,nonredunsize)) Loop(i+1,max,prefix,rest,permutations, counter, nonredun, nonredunsize); // if rest[i] already exists in nonredun, skip this rest [i] and jump to the rest[i+1]
    else
    {
    nonredun +=rest[i]; nonredunsize++;
    string prefixtemp=prefix; // create a string with the value of prefix that can be modified later in the loop. If we directly modify prefix in the loop, prefix value will be incremented every time when executing the loop.
    prefixtemp += rest[i];//Add the character to the end of prefix.
    string c; // create a string that stores the string "rest" with rest[i] eliminated.
    c += rest.substr(0, i);
    c +=rest.substr(i+1, rest.size()-1-i);//
    printPermutions (prefixtemp, c, permutations, counter);//Use recursion to generate permutations with the updated values for prefix and rest.
    Loop(i + 1, max, prefix, rest, permutations, counter, nonredun, nonredunsize);
    }
}
/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <csignal>
#include <unistd.h>

using namespace std;

void testone(int n)
{
    string dictionary[MAXDICTWORDS];
    string results[MAXRESULTS];
    
    switch (n)
    {
        default: {
            cout << "Bad argument" << endl;
        } break; case  1: {
            istringstream iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod");
            int numResults = readDictionary(iss, dictionary);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 9 && dictionary[0] == "act" && dictionary[1] == "art");
        } break; case  2: {
            // File is empty, Checks that file is empty and readDictionary returns 0.
            istringstream iss("");
            int numResults = readDictionary(iss, dictionary);
            assert(numResults == 0 && dictionary[0] == "");
        } break; case  3: {
            // Dictionary has been read properly
            istringstream iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod");
            int numResults = readDictionary(iss, dictionary);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 9 && dictionary[0] == "act" && dictionary[numResults-1] == "tar");
        } break; case  4: {
            // Input file is larger than the dictionary size
            istringstream iss("dog\ncat\nrat\neel\ntar\nart\nlee\nact\ngod\ntoo\nmany\nwords");
            int numResults = readDictionary(iss, dictionary);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 10 && dictionary[MAXDICTWORDS-1] == "too");
        } break; case  5: {
            // If a certain word with repeated letter is shown in results more than once - still accept.
            string dictionary[] = { "one", "oone", "ne", "e", "too" };
            int numResults = recursivePermute("oto", dictionary, 5, results);
            assert((numResults == 1 || numResults == 2) && results[0] == "too");
        } break; case  6: {
            // Doesn't check numResults nor duplicates.
            string dictionary[] = { "one", "oone", "ne", "e", "too" };
            recursivePermute("oto", dictionary, 5, results);
            assert(results[0] == "too");
        } break; case  7: {
            // If word wasn't found, numResults = 0 and results array is empty.
            string dictionary[] = { "one", "oone", "ne", "e" };
            int numResults = recursivePermute("look", dictionary, 4, results);
            assert(numResults == 0 && results[0] == "" && results[1] == "");
        } break; case  8: {
            // No fraction of a permutation is being searched in dictionary
            string dictionary[] = { "one", "oone", "non", "oon" };
            int numResults = recursivePermute("on", dictionary, 4, results);
            assert(numResults == 0 && results[0] == "" && results[1] == "");
        } break; case  9: {
            // No fraction of a permutation is being searched in dictionary
            string dictionary[] = { "one", "oone", "ne", "e", "neoo", "oneo" };
            int numResults = recursivePermute("neo", dictionary, 6, results);
            assert(numResults == 1 && results[0] == "one" && results[1] == "");
        } break; case  10: {
            // Checking that no error occurs if more than MAXRESULTS are found.
            string dictionary[] = { "true",  "treu", "teru", "teur", "ture",
                "tuer", "rtue", "rteu", "retu","reut", "ruet", "rute", "utre",
                "uter", "uetr", "uert", "urte", "uret", "etru", "etur", "ertu",
                "erut", "eurt", "eutr" };
            // All 24 permutations
            int numResults = recursivePermute("true", dictionary, 24, results);
            assert(numResults == MAXRESULTS);
        } break; case  11: {
            // Checking one word was found, no duplicates.
            string dictionary[] = { "ppp" };
            int numResults = recursivePermute("ppp", dictionary, 1, results);
            assert(numResults == 1 && results[0] == "ppp" && results[1] == "");
        } break; case  12: {
            string dictionary[] = { "run", "dodge", "break", "urn", "defeat" };
            int numResults = recursivePermute("nru", dictionary, 5, results);
            sort(dictionary, dictionary + numResults);
            assert(numResults == 2 && results[0] == "run" && results[1] == "urn");
        } break; case  13: {
            streambuf* oldCoutStreamBuf = cout.rdbuf();
            ostringstream strCout;
            cout.rdbuf(strCout.rdbuf());
            string results[] = { "cat", "act"};
            recurPrint(results, 2);
            cout.rdbuf(oldCoutStreamBuf);
            string temp = strCout.str();
            bool match1 = temp.find("act") != string::npos;
            bool match2 = temp.find("cat") != string::npos;
            assert(match1 && match2);
        } break; case  14: {
            istringstream iss ("tier\nrite\nbate\ntire\nttir");
            int numWords = readDictionary(iss, dictionary);
            sort(dictionary, dictionary + numWords);
            assert(numWords == 5 && dictionary[0] == "bate");
            int numResults = recursivePermute("tier", dictionary, numWords, results);
            assert(numResults == 3 && (results[2] == "tire" || results[2] == "tier" || results[2] == "rite"));
        } break; case  15: {
            string example[] = { "kool", "moe", "dee" };
            int numResults = recursivePermute("look", example, 3, results);
            assert(numResults == 1 && results[0] == "kool");
        } break;
    }
}

int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;
}
*/

int main()
{
    
    /*
     string word="0123";
     string permutated;
     string permutations[MAXDICTWORDS];
     int NUM=0;
     int num = printPermutions(permutated, word, permutations,NUM);
     for (int i=0;i<num;i++)
     {
     cout<< permutations[i]<<endl;
     }
     cout << num<<endl;
     */
    
    /*cout << word.size()<<endl;
     string temprest;
     temprest += word.substr(0, 2);
     temprest +=word. substr(3, word.size()-3);//Remove character from rest.
     word = temprest;
     cout << word.size()<<endl;
     cout<<word<<endl;*/
    
    string results[MAXRESULTS];
    string dict[MAXDICTWORDS];
    ifstream dictfile;         // file containing the list of words
    int nwords;                // number of words read from dictionary
    string word;
    
    dictfile.open("words.txt");
    if (!dictfile) {
        cout << "File not found!" << endl;
        return (1);
    }
    
    nwords = readDictionary(dictfile, dict);
    
    //cout<<nwords<<endl;
    //cout<<dict[25142]<<endl;
    
    cout << "Please enter a string for an anagram: ";
    cin >> word;
    int numMatches = recursivePermute(word, dict, nwords, results);
    if (!numMatches)
        cout << "No matches found" << endl;
    else
        recurPrint(results, numMatches);
    /*  for (int i=0;i<numMatches; i++)
     {
     cout<< results[i]<<endl;
     }
     */
    string exampleDict[] = {"kool", "moe", "dee"};
    int numResults = recursivePermute("look", exampleDict, 3, results);
    assert(numResults == 1 && results[0] == "kool");
    
    /*
     int counter=0;
     Match (word, dict, nwords, results, counter); */
    
}

