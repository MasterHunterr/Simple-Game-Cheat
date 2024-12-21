#include <windows.h>
#include <iostream>
#include <tchar.h>

// Program constants
#define WINDOW_NAME _T("Game Window Name")  // Name of the game window
#define POLL_RATE 1000                     // Polling rate in milliseconds
#define TARGET_ADDRESS 0x12345678          // Target memory address
#define NEW_VALUE 999                      // New value to write to the memory address

int main() {
    // Set the console window title
    SetConsoleTitle(_T("Game Cheat Tool"));

    std::cout << "START THE GAME FIRST" << std::endl;

    HWND gameWindowHandle = NULL;

    // Check if the game window is available
    while (gameWindowHandle == NULL) {
        gameWindowHandle = FindWindow(NULL, WINDOW_NAME);
        if (gameWindowHandle == NULL) {
            std::cout << "Waiting for game window..." << std::endl;
            Sleep(POLL_RATE);
        }
    }

    std::cout << "Game window found!" << std::endl;

    DWORD processId;
    GetWindowThreadProcessId(gameWindowHandle, &processId);

    if (processId != 0) {
        std::cout << "Process ID Found: " << processId << std::endl;
    } else {
        std::cerr << "Failed to find Process ID." << std::endl;
        return EXIT_FAILURE;
    }

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

    if (processHandle != NULL) {
        std::cout << "Process handle obtained successfully!" << std::endl;

        // The target address and the new value are defined as constants
        LPCVOID targetAddress = (LPCVOID)TARGET_ADDRESS;
        int value = 0;

        // Read the value from the target address
        if (ReadProcessMemory(processHandle, targetAddress, &value, sizeof(value), NULL)) {
            std::cout << "Value read from address " << targetAddress << ": " << value << std::endl;
        } else {
            std::cerr << "Failed to read memory. Error: " << GetLastError() << std::endl;
        }

        // Write a new value to the target address
        int newValue = NEW_VALUE;
        if (WriteProcessMemory(processHandle, (LPVOID)targetAddress, &newValue, sizeof(newValue), NULL)) {
            std::cout << "Value written to address " << targetAddress << ": " << newValue << std::endl;
        } else {
            std::cerr << "Failed to write memory. Error: " << GetLastError() << std::endl;
        }
    } else {
        std::cerr << "Failed to open process. Error: " << GetLastError() << std::endl;
        return EXIT_FAILURE;
    }

    // Close the handle when done
    CloseHandle(processHandle);
    std::cout << "Process handle closed." << std::endl;

    return 0;
}
