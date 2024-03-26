///File: CS112_A2_T4_2_20230591_20231235_20230359.cpp
/// Section 1/2
/// Yousef Osama Mamdouh Ahmed - 20230591      Route   -   Vignere   -   Baconian   -   XOR
/// Sara Shaban Ahmed Ali      - 20231235      Affine   -   Simple Substitution   -   Polybius Square
/// Mohamed Hisham Gaber Fahmy - 20230359      Atbash   -   Morse Code   -  Rail-Fence


#include "bits/stdc++.h"

using namespace std;

struct affine {
    void init(int choice) {
        int a, b, c;

        // Get 'a', 'b', and 'c' from the user
        cout << "Enter the value of 'a': ";
        cin >> a;

        cout << "Enter the value of 'b': ";
        cin >> b;

        // Validate 'a' to find its modular multiplicative inverse modulo 26
        int inverse_a = mod_inverse(a, 26);
        if (inverse_a == -1) {
            cout << "Error: The modular multiplicative inverse of 'a' doesn't exist modulo 26." << '\n';
            throw exception();
        }

        // Prompt the user to enter a valid value of 'c'
        bool valid_c = false;
        while (!valid_c) {
            cout << "Enter the value of c: ";
            cin >> c;

            // Check if (a * c) % 26 == 1
            if ((a * c) % 26 == 1) {
                valid_c = true;
            } else {
                cout << "Error: The value of 'c' should satisfy the condition (a * c) mod 26 == 1." << '\n';
            }
        }

        // Get message from the user
        string usermessage;
        cout << "Please enter a message: ";
        cin.ignore(); // Ignore newline character from previous input
        getline(cin, usermessage);

        if (choice == 1) {
            // Apply affine cipher
            string encrypted_message;
            for (char letter: usermessage) {
                encrypted_message += cipher(letter, a, b);
            }

            // Display encrypted message
            cout << "Encrypted message: " << encrypted_message << '\n';
        } else {
            // Decipher message
            string deciphered_message;
            for (char letter: usermessage) {
                deciphered_message += decipher(letter, b, c);
            }

            // Display deciphered message
            cout << "Deciphered message: " << deciphered_message << '\n';
        }
    }

    // Function to calculate the modular multiplicative inverse
    int mod_inverse(int a, int m) {
        a = a % m;
        for (int x = 1; x < m; x++)
            if ((a * x) % m == 1)
                return x;
        return -1; // Inverse doesn't exist
    }

    char cipher(char letter, int a, int b) {
        if (isalpha(letter)) {
            // Convert uppercase letter to lowercase
            letter = tolower(letter);

            // Convert the letter to its numeric equivalent
            int numeric_equivalent = letter - 'a';

            // Apply the transformation (ax + b) mod 26
            int new_value = (a * numeric_equivalent + b) % 26;

            // Convert the transformed numeric value back to a letter
            char new_letter = 'a' + new_value;

            return new_letter;
        }
        // Return unchanged for non-alphabetic characters
        return letter;
    }

    char decipher(char letter, int b, int c) {
        if (isalpha(letter)) {
            // Convert the letter to its numeric equivalent
            int number_equivalent = tolower(letter) - 'a';

            // Apply the transformation c(y-b) mod 26
            int old_value = c * (number_equivalent - b);
            while (old_value < 0)
                old_value += 26;
            old_value %= 26;

            // Convert the transformed numeric value back to a letter
            char deciphered_letter = 'a' + old_value;


            return deciphered_letter;
        }
        // Return unchanged for non-alphabetic characters
        return letter;
    }
};

struct route {

    void init(int choice) {
        while (true) {
            string msg;
            cout << "Enter your message: ";
            cin.ignore();
            getline(cin, msg);
            int key;
            cout << "Enter the key: ";
            if (!(cin >> key)) {
                throw exception();
            }
            if (choice == 1) {
                cout << cipher(msg, key) << "\n";
                break;
            } else {
                if (msg.size() % key) {
                    cout << "the message length should be divisible by the key\n";
                    throw exception();
                }
                cout << decipher(msg, key) << "\n";
                break;
            }
        }
    }

    string trim(string msg) {
        string result;
        for (char c: msg) {
            if (isalpha(c)) result += toupper(c);
        }
        return result;
    }

    vector<vector<int>> getRoute(int height, int width) {
        // arrays with dimensions[height][width]
        vector<vector<int>> result(height, vector<int>(width));
        vector<vector<bool>> isTaken(height, vector<bool>(width, false));
        // directions in order [down, left, up, right]
        int dx[] = {1, 0, -1, 0};
        int dy[] = {0, -1, 0, 1};
        // pointer on the current direction
        int dir = 0;
        // pointer on the current cell
        int x = -1, y = width - 1;
        // count the number of cells passed
        int cnt = 0;
        while (cnt < height * width) {
            // new cell according to the current direction
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            // if the new cell is not valid or taken, change to the next direction
            if (nx < 0 || ny < 0 || nx >= height || ny >= width || isTaken[nx][ny]) {
                dir = (dir + 1) % 4;
                continue;
            }
            // mark the new cell as taken
            isTaken[nx][ny] = true;
            // mark the order of the new cell as cnt
            result[nx][ny] = cnt++;
            // go to the new cell
            x = nx;
            y = ny;
        }
        return result;
    }

    string cipher(string msg, int key) {
        //remove spaces and symbols and change to uppercase
        msg = trim(msg);
        string result;
        // add special character in case the characters are not divisible by the key
        while (msg.size() % key) msg += 'X';
        int width = key;
        int height = msg.size() / width;
        // get order of each cell in the route
        auto route = getRoute(height, width);
        //store the position of a given order
        pair<int, int> pos[height * width];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pos[route[i][j]] = {i, j};
            }
        }
        //add the position of characters with order from 0 to height*width
        for (int i = 0; i < height * width; i++) {
            result += msg[pos[i].first * width + pos[i].second];
        }

        return result;
    }

    string decipher(string msg, int key) {
        string result;
        int width = key;
        int height = msg.size() / width;
        // get order of each cell in the route
        auto route = getRoute(height, width);
        // iterate over the cells row by row and column by column
        // and add the character in the message with the index equal to the order of this cell
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result += msg[route[i][j]];
            }
        }
        return result;
    }
};

struct atbash {
    void init(int choice) {
        if (choice == 1) {
            string plaintext;
            cout << "Enter the text to encrypt: ";
            cin.ignore();
            getline(cin, plaintext);
            string encrypted_text = cipher(plaintext);
            cout << "Encrypted text: " << encrypted_text << '\n';
        } else {
            string ciphertext;
            cout << "Enter the text to decrypt: ";
            cin.ignore();
            getline(cin, ciphertext);
            string decrypted_text = decipher(ciphertext);
            cout << "Decrypted text: " << decrypted_text << '\n';
        }
    }

    string cipher(const string &text) {
        string result;
        for (char x: text) {
            if (isalpha(x)) {
                if (isupper(x)) {
                    result += static_cast<char>(90 - (static_cast<int>(x) - 65));
                } else {
                    result += static_cast<char>(122 - (static_cast<int>(x) - 97));
                }
            } else {
                result += toupper(x);
            }
        }
        return result;
    }

    string decipher(const string &text) {
        return cipher(text);
    }
};

struct vignere {

    void init(int choice) {
        string text, key;
        cin.ignore();
        cout << "Enter the text: ";
        getline(cin, text);
        cout << "Enter the key: ";
        getline(cin, key);
        if (text.empty() || key.empty() || text.size() > 80 || key.size() > 8)
            throw exception();
        for (char c: key) {
            if (!isalpha(c)) {
                throw exception();
            }
        }
        if (choice == 1)
            cout << cipher(text, key) << '\n';
        else
            cout << decipher(text, key) << '\n';
    }

    string cipher(string text, string key) {
        string ans;
        for (int i = 0; i < text.size(); i++) {
            if (!isalpha(text[i])) {
                ans += text[i];
                continue;
            }
            text[i] = toupper(text[i]);
            ans += (text[i] + key[i % key.size()]) % 26 + 'A';
        }
        return ans;
    }

    string decipher(string text, string key) {
        string ans;
        for (int i = 0; i < text.size(); i++) {
            if (!isalpha(text[i])) {
                ans += text[i];
                continue;
            }
            ans += (text[i] - key[i % key.size()] + 26) % 26 + 'A';
        }
        return ans;
    }
};

struct baconian {
    void init(int choice) {
        string text;
        cin.ignore();
        cout << "Enter the text: ";
        getline(cin, text);
        if (choice == 1) {
            for (char c: text) {
                if (!isalpha(c))
                    throw exception();
            }
            cout << cipher(text) << '\n';
        } else
            cout << decipher(text) << '\n';
    }

    string cipher(string text) {
        string ans;
        for (int i = 0; i < text.size(); i++) {
            text[i] = toupper(text[i]);
            int order = text[i] - 'A';
            string temp;
            for (int j = 0; j < 5; j++) {
                temp += (order % 2 ? 'b' : 'a');
                order /= 2;
            }
            reverse(temp.begin(), temp.end());
            ans += temp + ' ';
        }
        return ans;
    }

    string decipher(string text) {
        string ans;
        string temp;
        for (int i = 0; i <= text.size(); i++) {
            if (i == text.size() || text[i] == ' ') {
                if (temp.size() != 5)
                    throw exception();
                int order = 0;
                for (int j = 0; j < 5; j++) {
                    if (temp[j] != 'a' && temp[j] != 'b')
                        throw exception();
                    order += (temp[j] == 'b') << (4 - j);
                }
                if (order + 'A' > 'Z')
                    throw exception();
                ans += order + 'A';
                temp = "";
            } else {
                temp += text[i];
            }
        }
        return ans;
    }
};

struct simpleSubstitution {
    vector<char> encryption_code;
    char alphabet[26];


    void init(int choice) {
        string unique_letters;
        cout << "Enter the 5 key letters: ";
        cin >> unique_letters;
        unique_letters = unique_letters.substr(0, 5); // Take only the first 5 characters

        // Populate the alphabet array
        for (char c = 'a'; c <= 'z'; ++c) {
            alphabet[c - 'a'] = c;
        }

        // Populate the encryption code array
        for (char c: unique_letters) {
            encryption_code.push_back(c);
        }

        // Add the rest of the alphabet to the encryption code, avoiding repetition of the first 5 letters
        for (char c: alphabet) {
            if (unique_letters.find(c) == string::npos) { // Check if the letter is not in the first 5 letters
                encryption_code.push_back(c);
            }
        }

        string message;
        cout << "Enter a message: ";
        cin.ignore(); // Ignore previous newline character
        getline(cin, message);
        if (choice == 1) {
            string encrypted_message = encrypt(message);
            cout << "Encrypted Message: " << encrypted_message << '\n';
        } else {
            string decrypted_message = decrypt(message);
            cout << "Decrypted Message: " << decrypted_message << '\n';
        }
    }

    // Encrypt function
    string encrypt(const string &message) {
        string encrypted_message;

        for (char c: message) {
            if (isalpha(c)) {
                c = tolower(c); // Convert to lowercase
                char encrypted_char = encryption_code[c - 'a'];
                encrypted_message += encrypted_char;
            } else {
                encrypted_message += c; // Append any non-letter character as it is
            }
        }

        return encrypted_message;
    }

// Decrypt function
    string decrypt(const string &encrypted_message) {
        string decrypted_message;

        // Decrypt each character in the encrypted message
        for (char c: encrypted_message) {
            if (isalpha(c)) {
                char decrypted_char = ' '; // Initialize decrypted character
                for (int i = 0; i < 26; ++i) {
                    if (tolower(encryption_code[i]) == tolower(c)) {
                        decrypted_char = alphabet[i]; // Found corresponding decrypted character
                        break;
                    }
                }
                // Append decrypted character preserving original case
                if (islower(c))
                    decrypted_message += decrypted_char;
                else
                    decrypted_message += toupper(decrypted_char);
            } else {
                decrypted_message += c; // Preserve non-letter characters as they are
            }
        }

        return decrypted_message;
    }
};

struct polybius {

    void init(int choice) {
        string key;
        cout << "enter key of encryption and decryption(5 numbers from 1 to 5): ";
        cin.ignore();
        getline(cin, key);
        if (key.length() != 5) {
            cout << "Key must contain exactly 5 characters." << endl;
            throw exception();
        }
        for (char ch: key) {
            if (ch < '1' || ch > '5') {
                cout << "Key must contain only digits from 1 to 5." << endl;
                throw exception();
            }
        }

        map<char, pair<char, char>> matrix = makeamatrix(key);
        if (choice == 1) {
            string message;
            cout << "Enter the message to be encrypted: ";
            getline(cin, message);

            // Convert the message to lowercase
            for (char &ch: message) {
                if (isalpha(ch)) {
                    ch = tolower(ch);
                }
            }

            string cipheredmessage = cipher(message, matrix);
            cout << "Encrypted message: " << cipheredmessage << endl;
        } else {
            string message;
            cout << "Enter the message to be decrypted: ";
            getline(cin, message);
            string decipheredmessage = decipher(message, matrix);
            cout << "Decrypted message: " << decipheredmessage << endl;
        }
    }

    const int ROWS = 5;
    const int COLS = 5;

// function to make a matrix of alphabets
    map<char, pair<char, char>> makeamatrix(const string &key) {
        map<char, pair<char, char>> matrix;
        string alphabet = "abcdefghiklmnopqrstuvwxyz"; // 'i' & 'j' are considered the same letter in polybius square

        int row = 0, col = 0;

        for (char ch: alphabet) {
            if (matrix.find(ch) == matrix.end()) {
                matrix[ch] = {key[row], key[col]};
                col++;
                if (col == COLS) {
                    col = 0;
                    row++;
                }
            }
        }

        return matrix;
    }

// Function to encrypt a message using the Polybius square and user-defined key
    string cipher(const string &message, const map<char, pair<char, char>> &matrix) {
        string cipher;

        for (char ch: message) {
            if (isalpha(ch)) {
                ch = tolower(ch); // Convert to lowercase for key lookup
                if (ch == 'j') ch = 'i'; // Replace 'j' with 'i'
                auto coords = matrix.find(ch)->second;
                cipher += coords.first;
                cipher += coords.second; // Convert to string
            } else {
                cipher += ch; // If not a letter, leave unchanged
            }
        }
        return cipher;
    }

// Function to decrypt a message using the Polybius square and user-defined key
    string decipher(const string &cipher, const map<char, pair<char, char>> &matrix) {
        string decipher;

        for (int i = 0; i < cipher.length(); i++) {
            char ch1 = cipher[i];
            char ch2 = cipher[i + 1];
            if (isdigit(ch1) && isdigit(ch2)) {
//                int row = ch1 - '1'; // Get row index
//                int col = ch2 - '1'; // Get column index
                for (auto &entry: matrix) {
                    auto coords = entry.second;
                    if (coords.first == ch1 && coords.second == ch2) {
                        decipher += entry.first;
                        break;
                    }
                }
                i++;
            } else {
                decipher += ch1;
            }
        }
        return decipher;
    }
};

struct morse {
    void init(int choice) {
        if (choice == 1) {
            string plaintext;
            cout << "Enter the text to encrypt: ";
            cin.ignore();
            getline(cin, plaintext);
            string encrypted_text = cipher(plaintext);
            cout << "Encrypted text: " << encrypted_text << endl;
        } else {
            string ciphertext;
            cout << "Enter the text to decrypt: ";
            cin.ignore();
            getline(cin, ciphertext);
            string decrypted_text = decipher(ciphertext);
            cout << "Decrypted text: " << decrypted_text << endl;
        }
    }

    string cipher(const string &text) {
        string morseCode;
        map<char, string> morseEncrypt = {
                {'A', ".-"},
                {'B', "-..."},
                {'C', "-.-."},
                {'D', "-.."},
                {'E', "."},
                {'F', "..-."},
                {'G', "--."},
                {'H', "...."},
                {'I', ".."},
                {'J', ".---"},
                {'K', "-.-"},
                {'L', ".-.."},
                {'M', "--"},
                {'N', "-."},
                {'O', "---"},
                {'P', ".--."},
                {'Q', "--.-"},
                {'R', ".-."},
                {'S', "..."},
                {'T', "-"},
                {'U', "..-"},
                {'V', "...-"},
                {'W', ".--"},
                {'X', "-..-"},
                {'Y', "-.--"},
                {'Z', "--.."},
                {'1', ".----"},
                {'2', "..---"},
                {'3', "...--"},
                {'4', "....-"},
                {'5', "....."},
                {'6', "-...."},
                {'7', "--..."},
                {'8', "---.."},
                {'9', "----."},
                {'0', "-----"},
                {' ', " "}
        };

        for (char c: text) {
            if (morseEncrypt.find(toupper(c)) != morseEncrypt.end()) {
                morseCode += morseEncrypt[toupper(c)] + " ";
            } else {
                throw exception();
            }
        }
        return morseCode;
    }

    string decipher(const string &morseCode) {
        string text;
        map<string, char> morseDecrypt = {
                {".-",    'A'},
                {"-...",  'B'},
                {"-.-.",  'C'},
                {"-..",   'D'},
                {".",     'E'},
                {"..-.",  'F'},
                {"--.",   'G'},
                {"....",  'H'},
                {"..",    'I'},
                {".---",  'J'},
                {"-.-",   'K'},
                {".-..",  'L'},
                {"--",    'M'},
                {"-.",    'N'},
                {"---",   'O'},
                {".--.",  'P'},
                {"--.-",  'Q'},
                {".-.",   'R'},
                {"...",   'S'},
                {"-",     'T'},
                {"..-",   'U'},
                {"...-",  'V'},
                {".--",   'W'},
                {"-..-",  'X'},
                {"-.--",  'Y'},
                {"--..",  'Z'},
                {".----", '1'},
                {"..---", '2'},
                {"...--", '3'},
                {"....-", '4'},
                {".....", '5'},
                {"-....", '6'},
                {"--...", '7'},
                {"---..", '8'},
                {"----.", '9'},
                {"-----", '0'},
                {" ",     ' '}
        };

        string current;
        bool state = false;
        for (char c: morseCode) {
            if (c == ' ' && !state) {
                if (morseDecrypt.find(current) != morseDecrypt.end()) {
                    text += morseDecrypt[current];
                } else {
                    throw exception();
                }
                current.clear();

                state = true;
            } else {
                current += c;
                state = false;
            }
        }
        if (morseDecrypt.find(current) != morseDecrypt.end()) {
            text += morseDecrypt[current];
        } else {
            throw exception();
        }

        return text;
    }
};

struct XOR {
    void init(int choice) {
        string text, key;
        cin.ignore();
        cout << "Enter text: ";
        getline(cin, text);
        cout << "Enter key (one char): ";
        getline(cin, key);
        if (key.size() != 1)
            throw exception();
        // cipher and decipher have the same methods
        if (choice == 1) {
            string ans = cipher(text, key);
            string hex;
            cout << "Cipher text: " << ans << '\n';
            for (char c: ans) {
                string temp;
                for (int i = 0; i < 2; i++) {
                    int rem = c % 16;
                    if (rem < 10)
                        temp += '0' + rem;
                    else
                        temp += 'a' + rem - 10;
                    c /= 16;
                }
                reverse(temp.begin(), temp.end());
                hex += temp + ' ';
            }
            cout << "Hexa: " << hex << '\n';
        } else {
            string temp, encrypted;
            for (int i = 0; i <= text.size(); i++) {
                if (i == text.size() || text[i] == ' ') {
                    if (temp.size() != 2)
                        throw exception();
                    int dec = 0;
                    for (int j = 0; j < 2; j++) {
                        int val;
                        if (temp[j] >= '0' && temp[j] <= '9')
                            val = temp[j] - '0';
                        else
                            val = temp[j] - 'a' + 10;
                        if (j == 0)
                            val *= 16;
                        dec += val;
                    }
                    encrypted += char(dec);
                    temp = "";
                } else {
                    temp += text[i];
                }
            }
            cout << "Plain Text: " << cipher(encrypted, key) << '\n';
        }
    }

    string cipher(string text, string key) {
        string ans;
        for (char c: text) {
            ans += c ^ key[0];
        }
        return ans;
    }
};

struct railfence {

    void init(int choice) {

        string text, tmp;
        int depth;

        cout << "Please enter the text: ";
        cin.ignore();
        getline(cin, tmp);
        for (char c: tmp) {
            if (isalpha(c))
                text += tolower(c);
        }

        cout << "Please enter the depth or rows. Notice: depth should be 2 or 3 only: ";
        cin >> depth;
        if (choice == 1) {
            cout << cipher(text, depth) << '\n';
        } else {
            cout << decipher(text, depth) << '\n';
        }
    }

    string cipher(string text, int depth) {
        if (depth == 2) {
            string result;
            for (int i = 0; i < text.length(); i += 2)
                result += text[i];
            for (int i = 1; i < text.length(); i += 2)
                result += text[i];
            return result;
        } else if (depth == 3) {
            string result;
            for (int i = 0; i < text.length(); i += 4)
                result += text[i];
            for (int i = 1; i < text.length(); i += 2)
                result += text[i];
            for (int i = 2; i < text.length(); i += 4)
                result += text[i];
            return result;
        } else {
            return "Number of rails not supported";
        }
    }

    string decipher(string ciphertext, int rails) {
        vector<vector<char>> fence(rails, vector<char>(ciphertext.length(), '\n'));
        int direction = -1;
        int row = 0, col = 0;

        for (char &ch: ciphertext) {
            if (row == 0 || row == rails - 1) {
                direction *= -1;
            }

            fence[row][col] = '*';
            col += 1;
            row += direction;
        }

        int index = 0;
        string plaintext;

        for (int r = 0; r < rails; ++r) {
            for (int j = 0; j < ciphertext.length(); ++j) {
                if (fence[r][j] == '*' && index < ciphertext.length()) {
                    fence[r][j] = ciphertext[index];
                    index += 1;
                }
            }
        }

        row = 0;
        col = 0;
        direction = -1;

        for (char &ch: ciphertext) {
            if (row == 0 || row == rails - 1) {
                direction *= -1;
            }

            plaintext += fence[row][col];
            col += 1;
            row += direction;
        }

        return plaintext;
    }
};

int main() {
    while (true) {
        try {
            int choice;
            cout << "What cipher do you want?\n";
            cout << "1.  Affine Cipher\n"
                    "2.  Route Cipher\n"
                    "3.  Atbash Cipher\n"
                    "4.  Vignere Cipher\n"
                    "5.  Baconian Cipher\n"
                    "6.  Simple Substitution Cipher\n"
                    "7.  Polybius Square Cipher\n"
                    "8.  Morse Code Cipher\n"
                    "9.  XOR Cipher\n"
                    "10. Rail-fence Cipher\n"
                    "11. Exit\n";
            if (!(cin >> choice) || choice < 1 || choice > 11) {
                throw exception();
            }
            if (choice == 11)
                break;
            int choice2;
            cout << "What do you want to do?\n";
            cout << "1. Cipher\n2. Decipher\n";
            if (!(cin >> choice2) || choice2 < 1 || choice2 > 2) {
                throw exception();
            }
            if (choice == 1) {
                affine cipher;
                cipher.init(choice2);
            } else if (choice == 2) {
                route cipher;
                cipher.init(choice2);
            } else if (choice == 3) {
                atbash cipher;
                cipher.init(choice2);
            } else if (choice == 4) {
                vignere cipher;
                cipher.init(choice2);
            } else if (choice == 5) {
                baconian cipher;
                cipher.init(choice2);
            } else if (choice == 7) {
                polybius cipher;
                cipher.init(choice2);
            } else if (choice == 8) {
                morse cipher;
                cipher.init(choice2);
            } else if (choice == 6) {
                simpleSubstitution cipher;
                cipher.init(choice2);
            } else if (choice == 9) {
                XOR cipher;
                cipher.init(choice2);
            } else if (choice == 10) {
                railfence cipher;
                cipher.init(choice2);
            }
            cout << '\n';
        } catch (...) {
            cin.clear();
            cout << "Invalid input.\n\n";
        }
    }
}