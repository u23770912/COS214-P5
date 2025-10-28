// TerminalUI.h - Simple colored terminal output
#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include <iostream>
#include <string>
#include <vector>

// ANSI Color Codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define GRAY    "\033[90m"

class TerminalUI {
public:
    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    static void printHeader(const std::string& title) {
        std::cout << BOLD << CYAN << "\n+============================================================+\n";
        std::cout << "|  " << title;
        // Pad to 58 chars
        for (int i = title.length(); i < 58; i++) std::cout << " ";
        std::cout << "|\n";
        std::cout << "+============================================================+" << RESET << "\n\n";
    }

    static void printSection(const std::string& title) {
        std::cout << BOLD << YELLOW << "+- " << title << " " << RESET << std::endl;
    }

    static void printSuccess(const std::string& msg) {
        std::cout << GREEN << "[OK] " << msg << RESET << std::endl;
    }

    static void printError(const std::string& msg) {
        std::cout << RED << "[X] " << msg << RESET << std::endl;
    }

    static void printWarning(const std::string& msg) {
        std::cout << YELLOW << "! " << msg << RESET << std::endl;
    }

    static void printInfo(const std::string& msg) {
        std::cout << BLUE << "i " << msg << RESET << std::endl;
    }

    static void printBusy(const std::string& name) {
        std::cout << RED << "  [BUSY] " << RESET << name << GRAY << " (working...)" << RESET << std::endl;
    }

    static void printAvailable(const std::string& name) {
        std::cout << GREEN << "  [FREE] " << RESET << name << std::endl;
    }

    static void printStaffStatus(const std::string& name, bool busy) {
        if (busy) {
            printBusy(name);
        } else {
            printAvailable(name);
        }
    }

    static void printCommandExecution(const std::string& command, const std::string& handler) {
        std::cout << MAGENTA << "> " << RESET << "Executing '" << BOLD << command << RESET 
                  << "' via " << CYAN << handler << RESET << std::endl;
    }

    static void printPrompt(const std::string& msg) {
        std::cout << BOLD << YELLOW << "\n> " << msg << RESET;
    }

    static void printDivider() {
        std::cout << GRAY << "────────────────────────────────────────────────────────────" << RESET << std::endl;
    }
};

#endif // TERMINAL_UI_H