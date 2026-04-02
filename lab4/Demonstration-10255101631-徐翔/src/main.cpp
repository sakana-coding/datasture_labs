#include "Myqueue.h"

#include <cctype>
#include <iostream>
#include <string>

namespace {

bool firstNonSpaceChar(const std::string& text, char& value) {
    for (char ch : text) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            value = ch;
            return true;
        }
    }
    return false;
}

void printProgramDescription() {
    std::cout << "This program allows the user to enter one command\n"
              << "(but only one) on each input line.\n"
              << "For example, if the command S is entered, then\n"
              << "the program will serve the front of the queue.\n\n";
}

void printHelpScreen() {
    std::cout << "The valid commands are:\n"
              << "A - Append the next input character to the extended queue\n"
              << "S - Serve the front of the extended queue\n"
              << "R - Retrieve and print the front entry.\n"
              << "# - The current size of the extended queue\n"
              << "C - Clear the extended queue (same as delete)\n"
              << "P - Print the extended queue\n"
              << "H - This help screen\n"
              << "Q - Quit\n";
}

void printInstructions() {
    printProgramDescription();
    printHelpScreen();
}

void printValidCommandReminder() {
    std::cout << "Please enter a valid command:\n"
              << "[a] Append [s] Serve  [r] Retrieve\n"
              << "[#] Size [c] Clear [p] Print [h] Help page\n"
              << "[q]uit.\n";
}

bool readSingleCharacter(std::istream& input, char& value) {
    std::string line;
    while (true) {
        if (!std::getline(input, line)) {
            return false;
        }

        if (firstNonSpaceChar(line, value)) {
            return true;
        }

        std::cout << "please input a char\n";
    }
}

bool readCommand(std::istream& input, char& command) {
    std::string line;
    if (!std::getline(input, line)) {
        return false;
    }

    char first = '\0';
    if (!firstNonSpaceChar(line, first)) {
        command = '\0';
        return true;
    }

    command = static_cast<char>(std::toupper(static_cast<unsigned char>(first)));
    return true;
}

} // namespace

int main() {
    MyQueue queue;
    printInstructions();

    while (true) {
        std::cout << "Select command and press <Enter>:";

        char command = '\0';
        if (!readCommand(std::cin, command)) {
            break;
        }

        switch (command) {
            case 'A': {
                std::cout << "please input a char\n";
                char value = '\0';
                if (!readSingleCharacter(std::cin, value)) {
                    return 0;
                }
                queue.append(value);
                break;
            }
            case 'S': {
                if (!queue.serve()) {
                    std::cout << "Empty Queue\n";
                }
                break;
            }
            case 'R': {
                char value = '\0';
                if (queue.retrieve(value)) {
                    std::cout << "\nThe first entry is: " << value << '\n';
                } else {
                    std::cout << "Empty Queue\n";
                }
                break;
            }
            case '#': {
                std::cout << "The size of Queue is:" << queue.size() << '\n';
                break;
            }
            case 'C': {
                queue.clear();
                std::cout << "The empty queue with length 0\n";
                break;
            }
            case 'P': {
                if (queue.empty()) {
                    std::cout << "Empty Queue\n";
                } else {
                    std::cout << "The Queue is " << queue.toString() << '\n';
                }
                break;
            }
            case 'H': {
                printInstructions();
                break;
            }
            case 'Q': {
                std::cout << "Queue demonstration finished.\n";
                return 0;
            }
            default: {
                printValidCommandReminder();
                break;
            }
        }
    }

    return 0;
}
