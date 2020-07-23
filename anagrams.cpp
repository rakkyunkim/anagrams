#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <cstring>

using namespace std;

const int MAXRESULTS = 20;    // Max matches that can be foundconst 
const int MAXDICTWORDS = 30000; // Max words that can be read in
//base functions
int loadWords(istream& dictfile, string dict[]);
int recBlends(string word, const string dict[], int size, string results[]);
void showResults(const string results[], int size);

//auxillary functions
void findPermutations(string prefix, string rest, const string dict[], const int& size, string results[], int& count);
void loopPermutations(int i, int max, string prefix, string rest, const string dict[], const int& size, string results[], int& count);
int checkDict(string target, const string dict[], int size, string results[], string result_spec[], int& count);
bool isDuplicated(string target, string results[], int start, int max);


int main() 
{
    string results[MAXRESULTS];
    string dict[MAXDICTWORDS];
    ifstream dictfile;         // file containing the list of words
    int nwords;                // number of words read from dictionary
    string word;

    
    dictfile.open("words.txt"); // open the file.
    if (!dictfile) {
        cout << "File not found!" << endl; // check if it opens properly.
        return (1);
    }

    nwords = loadWords(dictfile, dict);
    cout << "Please enter a string for an anagram: ";
    cin >> word;

    int numMatches = recBlends(word, dict, nwords, results);

    if (!numMatches)
        cout << "No matches found" << endl;
    else
        showResults(results, numMatches);

    return 0;

}

//Places each string in dictfile into the array dict. 
//Returns the number of words read into dict. 
//This number should not be larger than MAXDICTWORDS since that is the size of the array. 
int loadWords(istream& dictfile, string dict[]) 
{
    string word;
    //stops when reached the end of the file.
    if (dictfile.eof())
        return 0;
    //recursively get words.
    dictfile >> word;
    int i = loadWords(dictfile, dict);
    //put those words in dict[] until reached MAXDICTWORDS.
    if (i < MAXDICTWORDS) {
        dict[i] = word;
        i++;
        return i;
    }
    else
        return MAXDICTWORDS;
}
//Places all the combinations of word, which are found in dict into results.
//Returns the number of matched words found.
//This number should not be larger than MAXRESULTS since that is the size of the array.
//The size is the number of words inside the dict array.
int recBlends(string word, const string dict[], int size, string results[])
{
    int count = 0;

    findPermutations("", word, dict, size, results, count);

    return count;
}
//Displays size number of strings from results.
void showResults(const string results[], int size)
{
    if (size == 0)
        return;
    else
    {
        cout << "Matching word " << *results << endl;
        showResults(results + 1, size - 1);
    }
}

/*AUXILLARY FUNCTIONS*/
//Find permutation, count the number of annagrams found.
void findPermutations(string prefix, string rest, const string dict[], const int& size, string results[], int& count)
{   //Base case.
    if (rest.length() == 0)
    {
        //cout << prefix << endl; To see every permutations possible
        //loop through dict[] for a match. Increment the count if found.
        count += checkDict(prefix, dict, size, results, &results[count], count);
    }
    else
        loopPermutations(0, static_cast<int>(rest.length()), prefix, rest, dict, size, results, count);
}
//loop through each characters of word and find permutations.
void loopPermutations(int i, int restLength, string prefix, string rest, const string dict[], const int& size, string results[], int& count)
{   //Base case.
    //if done with permutation, return.
    if (i >= restLength)
        return;
    //for every character in rest, add the character to the end of prefix.
    //remove the first character from rest. Continue until rest is empty.
    findPermutations(prefix + rest[i], rest.substr(0, i) + rest.substr(i + 1), dict, size, results, count);
    //move one character over prefix, loop again.
    loopPermutations(i + 1, restLength, prefix, rest, dict, size, results, count);
}
//Loop through dict[] and check for a match. 
//1) If found, Return 1.
//2) if it is a duplicate or not found, Return 0.
int checkDict(string target, const string dict[], int size, string results[], string tempResult[], int& count)
{   //Base case.
    //if target not found, return 0.
    if (size == 0)
    {
        return 0;
    }
    //if annagram is found in dict[]
    if (target == *dict)
    {
        //check for a duplicate
        if (isDuplicated(target, results, 0, count))
            return 0;
        else
        {
            //store the annagram in the result array
            *tempResult = target;
            return 1 + checkDict(target, dict + 1, size - 1, results, tempResult + 1, count);
        }
    }
    else
        //recursively checks for a match until reached the end of dict[].
        return checkDict(target, dict + 1, size - 1, results, tempResult, count);
}
//loop through result[] to check for a duplicate. 
bool isDuplicated(string target, string results[], int startResults, int maxResults)
{   //Base case.
    //stops when reached the end of results[]. Return false.
    if (startResults >= maxResults)
        return false;
    //if the word is already in results[], return true.
    if (target == *results)
        return true;
    else
        //increment results and check again.
        return isDuplicated(target, results + 1, startResults + 1, maxResults);
}


