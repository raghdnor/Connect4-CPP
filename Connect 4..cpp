#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<cstdio>
using namespace std;
const int rows = 6;
const int cols = 7;
char board[rows][cols];
string player1, player2;
int p1Turns = 0;
int p2Turns = 0;
int turnCount = 0;
int current_player = 1;
bool game_end = false;
const string save_file = "savegame.txt";
const string out_file = "gamehistory.txt";
string getTimestamp() {
    time_t now = time(0);
    struct tm* tstruct = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tstruct);
    return string(buf);
}
void players(){
    turnCount = 0;
    p1Turns = 0;
    p2Turns = 0;
    game_end = false;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = ' ';
        }
    }
}
void saveCurrentSession() {
    ofstream file(save_file);
    if (file.is_open()) {
        file << player1 << endl << player2 << endl;
        file << turnCount << " " << p1Turns << " " << p2Turns << endl;
        file << current_player << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                file << board[i][j];
            }
            file << endl;
        }
        file.close();
    }
}

bool loadSession() {
    ifstream file(save_file);
    if (!file.is_open())return false;
    string line;
    getline(file, player1);
    getline(file, player2);
    file >> turnCount >> p1Turns >> p2Turns >> current_player;
    file.ignore();
    for (int i = 0; i < rows; i++) {
        getline(file, line);
        for (int j = 0; j < cols; j++) {
            board[i][j] = line[j];
        }
    }
    file.close();
    return true;

}
void logoutc(string winner) {
    ofstream file(out_file, ios::app);
    if (file.is_open()) {
        file << "Record Log: [" << getTimestamp() << "]\n";
        file << "Match: " << player1<<" vs "<<player2 << "\n";
        file << "Results: " << winner << " Won!!\n";
        file << "Stats: " << player1 <<" moves: "<<p1Turns<<" | "<<player2 << " moves: " << p2Turns << "\n";
        file << "Total turns: " << turnCount << "\n";
        file << "-------------------------------------------------------------------------------------------" << endl;
        file.close();

    }
}
void displayBoard() {
    system("cls");
    cout << "\n   ===  CONNECT 4  ===\n";
    cout << "   1   2   3   4   5   6   7\n";
    for (int i = 0; i < rows; i++) {
        cout << " |";
        for (int j = 0; j < cols; j++)
            cout << " " << board[i][j] << " |";
        cout << endl;
    }
    cout << " =============================\n";
    cout << " " << player1 << " (X) Turns: " << p1Turns << " | " << player2 << " (O) Turns: " << p2Turns << endl;
    cout << " (S) save & Exit | (R) Restart | (Q) Quit\n";
}

bool count(int col, char symbol) {
    if (col < 0 || col >= cols)return false;
    for (int i = rows - 1; i >= 0; i--) {
        if (board[i][col] == ' ') {
            board[i][col] = symbol;
            turnCount++;
            if (current_player == 1)p1Turns++;
            else p2Turns++;
            return true;
        }
    }
    return false;
}


bool checkWin(char s) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] != s) continue;
            if (j + 3 < cols && board[i][j + 1] == s && board[i][j + 2] == s && board[i][j + 3] == s)return true;
            if (i + 3 < rows && board[i+1][j] == s && board[i+2][j] == s && board[i+3][j] == s)return true;
            if (i + 3 < rows && j + 3 < cols && board[i + 1][j + 1] == s && board[i + 2][j + 2] == s && board[i + 3][j + 3] == s)return true;
            if (i + 3 < rows && j - 3 >= 0 && board[i + 1][j - 1] == s && board[i + 2][j - 2] == s && board[i + 3][j - 3] == s)return true;

        }
    }
    return false;
}


int main() {
    string timestamp = getTimestamp();
    cout << "current date and time: "<<timestamp << endl;
    ifstream check(save_file);
    if (check) {
        char choice;
        cout << "Saved game found. Resume ? (C)ontinue or (N)ew game : ";
        cin >> choice;
        check.close();
        if (choice == 'C' || choice == 'c') {
         loadSession();
        }
        else {
            remove(save_file.c_str());
            players();
            cout << "Player 1: ";
            cin >> player1;
            cout << "Player 2: ";
            cin >> player2;
        }
    }
 
          
        
        
    else {
        players();
        cout << "Player 1: ";
        cin >> player1;
        cout << "Player 2: ";
        cin >> player2;
    }

    while (true) {

        displayBoard();
        string name = (current_player == 1) ? player1 : player2;
        char symbol = (current_player == 1) ? 'X' : 'O';
        string input;
        cout << name << " (" << symbol << "), enter 1-7 or letter: ";
        cin >> input;
        if (input == "S" || input == "s") {

            saveCurrentSession();
            cout << "Progress saved.\n";
            return 0;

        }
        else if (input == "R" || input == "r") {
            remove(save_file.c_str());
            players();
            continue;

        }
        else if (input == "Q" || input == "q") {
            return 0;

        }
        if (input.length() == 1 && input[0] >= '1' && input[0] <= '7') {
            int col = input[0] - '1';
            if (count(col, symbol)) {
                if (checkWin(symbol)) {
                    displayBoard();
                    cout << "\nCongratulations " << name << " wins\n";
                    logoutc(name);
                    remove(save_file.c_str());
                    char y;
                    cout << "New game?(y/n): ";
                    cin >> y;
                    if (y == 'y' || y == 'Y') {
                        players();
                        continue;
                    }
                    else break;

                }
                else if (turnCount == rows * cols) {
                    displayBoard();
                    cout << "\n It's a Draw\n";
                    logoutc("draw");
                    remove(save_file.c_str());
                    break;
                }

                current_player = (current_player == 1) ? 2 : 1;
                saveCurrentSession();
            }
            else {
                cout << "Column is full press enter to try again ";
                cin.ignore();
                cin.get();
            }
        }
        else {
            cout << "Invalid input! use 1-7, S, R, or Q\n";
            cout << "press enter to continue  ";
            cin.ignore(1000, '\n');
            cin.get();



        }
    }

}