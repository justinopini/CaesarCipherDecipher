/**
 * @author Justin Opini(opinijm@clarkson.edu)
 * Last Modified : 12/07/2019
 * Deciphers Caesar input via letter frequency analysis
 */

#include<algorithm>
#include<iostream>
#include<fstream>
#include <functional>
#include <set>
#include <vector>

static const int ALPHABET_SIZE = 26;
static const int ASCII_ALPHA_LOWER_BOUND = 97;
static const int ASCII_ALPHA_UPPER_BOUND = ASCII_ALPHA_LOWER_BOUND + ALPHABET_SIZE;
static const char SORTED_ALPHABETS_BY_NORMAL_FREQ[] = {'j', 'z', 'q', 'k', 'x', 'v', 'g', 'w', 'b', 'p', 'y', 'f', 'c', 'u', 'm', 'd', 'l', 'h', 'o', 'n', 'i', 'r', 's', 'a', 't', 'e'};

struct letter_freq{
    char letter;
    int freq;
};
typedef std::function<bool(letter_freq, letter_freq)> comparator;

static std::string prettify(std::string str);
static std::set<letter_freq, comparator>get_char_frequencies(std::string str);
static int get_mode_shift(std::string str);
static char decode_char(char c, int shift);
static void write(const std::string &, const std::string &);

int main() {
    //Get the file name and set up the stream.
    std::string file_name;
    std::ifstream input;
    while (!input.is_open() || input.fail()) {
        std::cout<< " Please enter a valid file name to be decrypted: ";
        std::cin >> file_name;
        input.open( file_name.c_str());
    }

    //Read the file.
    std::string file_content((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
    input.close();

    int mode_shift = get_mode_shift(file_content);

    //Decodes message with mode shift value.
    char decoded_letter;
    for (char &c : file_content){
        if (isalpha(c)) {
            decoded_letter = decode_char(static_cast<char>(tolower(c)), mode_shift);

            //Restores capitalization.
            if (isupper(c)) {
                c = static_cast<char>(toupper(decoded_letter));
            } else{
                c = decoded_letter;
            }

        }
    }

    write(file_name, file_content);
    return 0;
}

/**
 *
 * @param str
 * @return The most frequent shift based from the normal distribution in English.
 */
static int get_mode_shift(std::string str){
    str = prettify(str);
    std::set<letter_freq, comparator> sorted_letter_frequencies = get_char_frequencies(str);
    std::vector<int > shift_values;

    // Calculate each individual shift
    int freq_rank = 0;
    int shift;
    for (letter_freq e : sorted_letter_frequencies) {
        shift = SORTED_ALPHABETS_BY_NORMAL_FREQ[freq_rank] - e.letter;
        shift_values.push_back(shift);
        freq_rank += 1;
    }


    //Find mode shift
    int highest_count, current_count = 0;
    int mode_shift, current_shift = 0;
    std::sort(shift_values.begin(), shift_values.end());
    for(int s : shift_values)
    {
        if(s == current_shift){
            ++current_count;
        } else{
            if (current_count > highest_count){
                mode_shift = s;
                highest_count = current_count;
            }
            current_count = 0;
            current_shift = s;
        }

    }

    return mode_shift;
}

/**
 * Removes punctuations and makes all letters lower case.
 * @param str
 * @return 'pretiffied' str
 */
static std::string prettify(std::string str){
    for (char &c : str) {
        c = static_cast<char>(tolower(c));
        if(ispunct(c)) {
            c = ' ';
        }
    }

    return str;
}

/**
 * Generates letter frequencies of each letter.
 * @param str
 * @return a set of pairs that can be iterated through letter frequency order
 */
static std::set<letter_freq, comparator> get_char_frequencies(std::string str) {
    int letter_frequencies[ALPHABET_SIZE] = {0};
    int int_char;
    for (char c: str) {
        int_char = c - ASCII_ALPHA_LOWER_BOUND;
        if (int_char >= 0)
            letter_frequencies[int_char]++;
    }
    std::vector<letter_freq> letter_frequencies_pairs;
    char c = 'a';
    for (int f: letter_frequencies){
        letter_frequencies_pairs.push_back(letter_freq{c, f});
        ++c;
    }

    // Sorts by second element of pair (frequency).
    static const comparator comp =
            [](letter_freq pair1 ,letter_freq pair2){
                return pair1.freq < pair2.freq;
            };


    // Set that is iterable through letter frequency.
    std::set<letter_freq, comparator> sorted_letter_frequencies(
            letter_frequencies_pairs.begin(), letter_frequencies_pairs.end(), comp);
    return sorted_letter_frequencies;
}

/**
 *
 * @param c
 * @param shift
 * @return decoded c
 */
static char decode_char(char c, int shift){
    int int_letter = c - ASCII_ALPHA_LOWER_BOUND;
    int_letter = (int_letter + shift) % ALPHABET_SIZE;
    int_letter += ASCII_ALPHA_LOWER_BOUND;

    // In case shift is negative.
    if(int_letter < ASCII_ALPHA_LOWER_BOUND){
        int overflow = ASCII_ALPHA_LOWER_BOUND - int_letter;
        int_letter = ASCII_ALPHA_UPPER_BOUND - overflow;
    }
    return static_cast<char>(int_letter);

}

/**
 * Outputs the decoded message to a new file
 * @param file_name
 * @param decrypted_message
 */
static void write(const std::string &file_name, const std::string &decrypted_message){
    std::ofstream writer;
    unsigned long file_extension_location = file_name.find('.');
    std::string output_file_name = file_name.substr(0, file_extension_location) + "_decoded.txt";

    writer.open(output_file_name.c_str());
    writer << decrypted_message;
    writer.close();
    std::cout << "\n\nYour message has been decoded to the file: " << output_file_name << std::endl;
}
