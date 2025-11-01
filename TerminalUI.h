// TerminalUI.h - Enhanced colored terminal output with progress bars and tables
#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>

// ANSI Color Codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"

class TerminalUI {
public:
    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            std::cout << "\033[2J\033[1;1H";
        #endif
    }

    static void setCursorPosition(int row, int col) {
        std::cout << "\033[" << row << ";" << col << "H";
    }

    static void hideCursor() {
        std::cout << "\033[?25l";
    }

    static void showCursor() {
        std::cout << "\033[?25h";
    }

    static void printHeader(const std::string& title, const std::string& subtitle = "") {
        std::cout << BOLD << CYAN << "\n============================================================\n";
        std::cout << "  " << title << "\n";
        
        if (!subtitle.empty()) {
            std::cout << "  " << subtitle << "\n";
        }
        
        std::cout << "============================================================" << RESET << "\n\n";
    }

    static void printSection(const std::string& title) {
        std::cout << BOLD << YELLOW << ">> " << title << RESET << std::endl;
    }

    static void printSubsection(const std::string& title) {
        std::cout << BOLD << BLUE << "  - " << title << RESET << std::endl;
    }

    static void printSuccess(const std::string& msg) {
        std::cout << GREEN << "[OK] " << msg << RESET << std::endl;
    }

    static void printError(const std::string& msg) {
        std::cout << RED << "[ERROR] " << msg << RESET << std::endl;
    }

    static void printWarning(const std::string& msg) {
        std::cout << YELLOW << "[WARN] " << msg << RESET << std::endl;
    }

    static void printInfo(const std::string& msg) {
        std::cout << BLUE << "[INFO] " << msg << RESET << std::endl;
    }

    static void printDebug(const std::string& msg) {
        std::cout << GRAY << "[DEBUG] " << msg << RESET << std::endl;
    }

    static void printBusy(const std::string& name) {
        std::cout << RED << "  [BUSY] " << RESET << name << GRAY << " (working...)" << RESET << std::endl;
    }

    static void printAvailable(const std::string& name) {
        std::cout << GREEN << "  [FREE] " << RESET << name << std::endl;
    }

    static void printStaffAssignment(const std::string& plantId,
                                     const std::string& plantState,
                                     const std::string& task) {
        std::cout << GRAY << "    -> " << RESET;
        if (!plantId.empty()) {
            std::cout << CYAN << plantId << RESET;
        } else {
            std::cout << CYAN << "Unknown" << RESET;
        }

        if (!plantState.empty()) {
            std::cout << " [" << BOLD << plantState << RESET << "]";
        }

        if (!task.empty()) {
            std::cout << " | task: " << BOLD << MAGENTA << task << RESET;
        }

        std::cout << std::endl;
    }

    static void printCommandExecution(const std::string& command, const std::string& handler) {
        std::cout << MAGENTA << "> " << RESET << "Executing '" << BOLD << command << RESET 
                  << "' via " << CYAN << handler << RESET << std::endl;
    }

    static void printPrompt(const std::string& msg) {
        std::cout << BOLD << YELLOW << "\n> " << msg << RESET;
    }

    static void printDivider(char style = '-', int length = 60) {
        std::cout << GRAY << std::string(length, style) << RESET << std::endl;
    }

    static void printDoubleDivider() {
        std::cout << GRAY << "============================================================" << RESET << std::endl;
    }

    // Progress bar functionality
    static void printProgressBar(int current, int total, int width = 50, const std::string& label = "") {
        float percentage = static_cast<float>(current) / total;
        int filled = static_cast<int>(width * percentage);
        
        std::cout << "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) {
                std::cout << BG_GREEN << " " << RESET;
            } else {
                std::cout << GRAY << "." << RESET;
            }
        }
        std::cout << "] " << std::setw(3) << static_cast<int>(percentage * 100) << "%";
        
        if (!label.empty()) {
            std::cout << " " << label;
        }
        std::cout << std::endl;
    }

    // Animated loading spinner
    static void showSpinner(int durationMs = 1000, const std::string& message = "Processing") {
        const char spinner[] = {'|', '/', '-', '\\'};
        auto start = std::chrono::steady_clock::now();
        int i = 0;
        
        hideCursor();
        std::cout << YELLOW << message << " " << RESET;
        
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - start).count() < durationMs) {
            std::cout << "\b" << spinner[i] << std::flush;
            i = (i + 1) % 4;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "\b" << GREEN << "DONE" << RESET << std::endl;
        showCursor();
    }

    // Table printing utility
    static void printTable(const std::vector<std::vector<std::string>>& rows, 
                          const std::vector<std::string>& headers = {}) {
        if (rows.empty()) return;
        
        // Calculate column widths
        std::vector<size_t> colWidths(rows[0].size(), 0);
        for (const auto& row : rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                colWidths[i] = std::max(colWidths[i], row[i].length());
            }
        }
        
        // Print headers if provided
        if (!headers.empty()) {
            std::cout << BOLD << CYAN;
            for (size_t i = 0; i < headers.size(); ++i) {
                std::cout << std::setw(colWidths[i] + 2) << std::left << headers[i];
            }
            std::cout << RESET << std::endl;
            printDivider('-');
        }
        
        // Print rows
        for (const auto& row : rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << std::setw(colWidths[i] + 2) << std::left << row[i];
            }
            std::cout << std::endl;
        }
    }

    // Plant state visualization
    static void printPlantState(const std::string& plantId, const std::string& species, 
                               const std::string& state, int progress, bool healthy = true) {
        std::string color = healthy ? GREEN : RED;
        std::string stateSymbol = healthy ? "[HEALTHY]" : "[WITHERING]";
        
        std::cout << color << stateSymbol << " " << RESET 
                  << BOLD << plantId << RESET << " - " << species
                  << " [" << color << state << RESET << "] "
                  << "(" << progress << "%)" << std::endl;
    }

    // Status badge
    static void printBadge(const std::string& text, const std::string& color = BLUE) {
        std::cout << color << BOLD << "[" << text << "]" << RESET;
    }

    // Clear current line
    static void clearLine() {
        std::cout << "\033[2K\r";
    }

    // Print with timestamp
    static void printWithTimestamp(const std::string& msg, const std::string& color = RESET) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::cout << GRAY << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] " 
                  << RESET << color << msg << RESET << std::endl;
    }
};

#endif // TERMINAL_UI_H