#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "boost/lexical_cast.hpp"

INPUT mouseInput[2]; // do not put this is main function because your pc will crash and you will have a black screen.

bool windowShown = true;
HWND consoleWindow = GetConsoleWindow();

bool toggle = false;
std::string toggleDisplay = "False";

bool userError = false;
const int timeResolution = 3;

const int maxCps = 5000;
int cps = 0;
std::string cpsString;

const int darkRed = 4;
const int lightRed = 12;

void menu(int cps, int maxCps, std::string toggleDisplay)
{
    system("cls");
    std::cout << "Clicking: " << toggleDisplay << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Current cps: " << cps << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Max cps: " << maxCps << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Press Mouse Button 5 to toggle clicking" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Press Home to change cps" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Press Delete to minimize/maximize" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Press Pause to terminate the program" << std::endl;
}

int main()
{
    SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
    SetConsoleTitleA("Syphon AutoClicker V2");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lightRed);
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 410, 300, SWP_SHOWWINDOW);

    mouseInput->type = INPUT_MOUSE;

    mouseInput[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    mouseInput[0].mi.dx = 0;
    mouseInput[0].mi.dy = 0;
    mouseInput[0].mi.mouseData = 0;
    mouseInput[0].mi.dwExtraInfo = 0;

    mouseInput[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    mouseInput[1].mi.dx = 0;
    mouseInput[1].mi.dy = 0;
    mouseInput[1].mi.mouseData = 0;
    mouseInput[1].mi.dwExtraInfo = 0;

    try
    {
        std::cout << "Enter desired cps: ";
        std::cin >> cpsString;
        cps = boost::lexical_cast<int>(cpsString);
        menu(cps, maxCps, toggleDisplay);
    }
    catch (boost::bad_lexical_cast e)
    {
        userError = true;
    }

    while (true)
    {

        if (GetAsyncKeyState(VK_PAUSE) & 1)
        {
            exit(0);
        }

        if (GetAsyncKeyState(VK_DELETE) & 1)
        {
            windowShown = !windowShown;
            if (windowShown)
            {
                ShowWindow(consoleWindow, SW_SHOWNORMAL);
            }
            if (!windowShown)
            {
                ShowWindow(consoleWindow, SW_MINIMIZE);
            }
        }

        while (userError || cps > maxCps)
        {
            userError = false;
            toggle = false;
            toggleDisplay = "False";
            cps = 0;

            system("cls");
            std::cout << "Only whole numbers less than " << maxCps << " are allowed." << std::endl;
            std::cout << "Please try again." << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Enter desired cps: ";
            std::cin >> cpsString;
            try
            {
                cps = boost::lexical_cast<int>(cpsString);
            }
            catch (boost::bad_lexical_cast e)
            {
                userError = true;
            }
            menu(cps, maxCps, toggleDisplay);
        }

        if (GetAsyncKeyState(VK_XBUTTON2) & 1)
        {
            toggle = !toggle;
            if (toggle)
            {
                timeBeginPeriod(timeResolution);
            }
            if (!toggle)
            {
                timeEndPeriod(timeResolution);
            }

            toggleDisplay = toggle ? "True" : "False";

            menu(cps, maxCps, toggleDisplay);
        }

        if (windowShown)
        {
            if (GetAsyncKeyState(VK_HOME) & 1)
            {
                SetForegroundWindow(consoleWindow);
                system("cls");
                std::cout << "Enter desired cps: ";
                std::cin >> cpsString;
                try
                {
                    cps = boost::lexical_cast<int>(cpsString);
                    //uncomment the 2 lines below for auto minimize after changing cps

                    //ShowWindow(consoleWindow, SW_MINIMIZE);
                    //windowShown = !windowShown;
                }
                catch (boost::bad_lexical_cast e)
                {
                    userError = true;
                }
                menu(cps, maxCps, toggleDisplay);
            }
        }

        if (toggle)
        {
            SendInput(2, mouseInput, sizeof(INPUT));
        }
        Sleep(1000 / (cps + 1));
    }
}