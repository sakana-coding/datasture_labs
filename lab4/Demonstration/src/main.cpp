#include "Myqueue.h"

#include <cctype>
#include <iostream>
#include <string>

namespace {

std::string trim(const std::string& text) {
    std::size_t left = 0;
    while (left < text.size() && std::isspace(static_cast<unsigned char>(text[left]))) {
        ++left;
    }

    std::size_t right = text.size();
    while (right > left && std::isspace(static_cast<unsigned char>(text[right - 1]))) {
        --right;
    }

    return text.substr(left, right - left);
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

bool readSingleCharacter(std::istream& input, char& value) {
    std::string line;
    while (true) {
        if (!std::getline(input, line)) {
            return false;
        }

        std::string cleaned = trim(line);
        if (cleaned.size() == 1) {
            value = cleaned[0];
            return true;
        }

        std::cout << "Please input exactly one character.\n";
        std::cout << "please input a char\n";
    }
}

bool readSingleCommand(std::istream& input, char& command) {
    std::string line;
    if (!std::getline(input, line)) {
        return false;
    }

    std::string cleaned = trim(line);
    if (cleaned.size() != 1) {
        return false;
    }

    command = static_cast<char>(std::toupper(static_cast<unsigned char>(cleaned[0])));
    return true;
}

} // namespace

int main() {
    MyQueue queue;
    printInstructions();

    while (true) {
        std::cout << "Select command and press <Enter>:";

        char command = '\0';
        if (!readSingleCommand(std::cin, command)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << '\n';
            printInstructions();
            continue;
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
                    std::cout << "Queue is empty.\n";
                }
                break;
            }
            case 'R': {
                char value = '\0';
                if (queue.retrieve(value)) {
                    std::cout << "\nThe first entry is: " << value << '\n';
                } else {
                    std::cout << "Queue is empty.\n";
                }
                break;
            }
            case '#': {
                std::cout << "The size of Queue is:" << queue.size() << '\n';
                break;
            }
            case 'C': {
                queue.clear();
                break;
            }
            case 'P': {
                if (queue.empty()) {
                    std::cout << "Queue is empty.\n";
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
                return 0;
            }
            default: {
                std::cout << '\n';
                printInstructions();
                break;
            }
        }
    }

    return 0;
}
