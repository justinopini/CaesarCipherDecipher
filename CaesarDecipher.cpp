//Purpose: Deecipher Caesar Cipher via letter frrequency analysis
//Developed by: Justin Opini(opinijm@clarkson.edu)
//Last Modified : 11/06/2017
#include<iostream>
#include<fstream>
#include<algorithm>
#include<string>
#include<cstdlib>
//###################################################################### GLOBAL VARIABLES ########################################################################################
const int alphabetsSIZE = 26;
int sortedFrequencies[alphabetsSIZE];
struct alphabetInfo{
    char alphabet;int frequency;
    int rank;char decodedAlphabet;int shift;
};
//################################################################################################################################################################################
int rank(int);
void sortFreq(int[alphabetsSIZE]);
void write(std::string,std::string);
int main() {
    //######################################################## VARIABLES #######################################################################
    char alphabets[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char sortedAlphabets[] = {'j', 'z', 'q', 'k', 'x', 'v', 'g', 'w', 'b', 'p', 'y', 'f', 'c', 'u', 'm', 'd', 'l', 'h', 'o', 'n', 'i', 'r', 's', 'a', 't', 'e'};
    int alphabetfrequency[alphabetsSIZE] = {0};
    int shiftValues[alphabetsSIZE] = {0};
    int modeShift = 0;
    int buffer = 0;
    const int ASCIIconvert = 97;
    char bufferAlphabet;
    std::string toDecode = "";
    std::string temp;
    std::string decoded;
    std::string original;
    std::string fileName;
    long messageLength = 0;
    alphabetInfo current;
    std::ifstream cipher;
    //############################################################################################################################################
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^CAESAR CIPHER DECIPHER^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<< std::endl;
    std::cout<< " Please enter the file name to be decrypted" << std::endl;
    std::cin >> fileName;
    cipher.open( fileName.c_str());
    if ( cipher.fail()) {
        std::cout<< "WARNING!! make sure that the filename is correct in the correct case!! and in the same path as the programme" <<std::endl;
        sleep(2);
        main();
    }
    while (!cipher.eof()) {
        getline(cipher,temp);
        toDecode += temp;
    }
    original = toDecode;
    messageLength = toDecode.length();
    //Read in file
    for (int a = 0; a < messageLength; ++a)
    {
        toDecode[a] = tolower(toDecode[a]);
        if(ispunct(toDecode[a])) {
            toDecode[a] = ' ';
        }
    }
    //Frequecy of each letter
    for (int b = 0; b < messageLength; b++) {
        buffer = toDecode[b] - 97;
        if (buffer >= 0) {
            alphabetfrequency[buffer]++;
        }
    }
    //sort frequencies globally
    sortFreq(alphabetfrequency);
    //calculates all letters shift values
    for(int c = 0; c < alphabetsSIZE; c++){
        current.alphabet = alphabets[c];
        current.frequency = alphabetfrequency[c];
        current.rank = rank(current.frequency);
        current.decodedAlphabet = sortedAlphabets[current.rank];
        current.shift = current.decodedAlphabet - current.alphabet;
        shiftValues[c] = current.shift;
        }
    //Finds mode shift value
    std::sort(shiftValues,shiftValues+alphabetsSIZE);
    for(int i = 0,highestMode = 0,modeCount = 0,currentmode = shiftValues[0],highestmodecount = 0;i < alphabetsSIZE;i++)
    {
        if (shiftValues[i] == currentmode) {
            modeCount++;
        } else {
            if (modeCount > highestmodecount) {
                highestMode = currentmode;
                highestmodecount = modeCount;
                modeCount = 0;
            }
        }
        currentmode = shiftValues[i];
        modeShift = highestMode;
    }
    //decodes message with mode shift value
    decoded = toDecode;
    for (int y = 0; y < messageLength; y++){
            if (isalpha(decoded[y])) {
                buffer = decoded[y] - ASCIIconvert;
                buffer = (buffer + modeShift) % alphabetsSIZE;
                buffer = buffer + ASCIIconvert;
                bufferAlphabet = buffer;
                decoded[y] = bufferAlphabet;
                if(decoded[y] < ASCIIconvert){
                    buffer = ASCIIconvert - decoded[y];
                    buffer = (ASCIIconvert+alphabetsSIZE) - buffer;
                    bufferAlphabet = buffer;
                    decoded[y] = bufferAlphabet;
                }
                //restors capitalization
                if (isupper(original[y])) {
                    decoded[y] = toupper(decoded[y]);
                }

            } else{}
        //restores punctuation
        if (ispunct(original[y])) {
                    decoded[y] = original[y];
                }
     }
    std::cout << decoded << std::endl;
    write(fileName,decoded);
    return 0;
}
//sorts frequencies globally
void sortFreq(int frequencyarray[alphabetsSIZE]) {
    for (int x = 0; x < alphabetsSIZE; x++) {
        sortedFrequencies[x] = frequencyarray[x];
    }
    std::sort(sortedFrequencies,sortedFrequencies+alphabetsSIZE);
}
//returns the rank of the frequency in ascending order
int rank(int toInput) {
    static int found = 0;
    found = std::distance(sortedFrequencies,std::find(sortedFrequencies,sortedFrequencies+alphabetsSIZE,toInput));
    return found;
}
//Outputs the decoded message to a new file
void write(std::string fileName, std::string message){
    std::ofstream decipher;
    static int loc = fileName.find('.');
    fileName = fileName.substr(0,loc);
    fileName += "_decoded.txt";
    decipher.open(fileName.c_str());
    decipher << message;
    std::cout << "\n\nYour message has been 100% decoded to the file: " << fileName << std::endl;
}
