#include <iostream>
#include <Windows.h>
#include <string>
#include "boost/lexical_cast.hpp"

INPUT mouseInput[2]; // do NOT put this is main function because your pc will crash and you will have a black screen.

void menu(int cps, int maxCps, std::string toggleDisplay)
{
    system("cls");
    std::cout << "Clicking: " << toggleDisplay << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Current cps is: " << cps << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Max cps is: " << maxCps << std::endl;
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

    bool windowShown = true;
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, 47, 17, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ShowWindow(consoleWindow, SW_NORMAL);

    bool toggle = false;
    std::string toggleDisplay = "False";

    bool userError = false;
    const int timeResolution = 3;

    const int maxCps = 5000;
    int cps = 0;
    std::string cpsString;

    const int darkRed = 4;
    const int lightRed = 12;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, lightRed);

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
                ShowWindow(consoleWindow, SW_NORMAL);
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
            std::cout << "Error: Only whole numbers less than " << maxCps << " are allowed." << std::endl;
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