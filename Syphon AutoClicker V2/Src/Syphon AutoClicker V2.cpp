#include <iostream>
#include <Windows.h>
#include <string>
#include "boost/lexical_cast.hpp"

int main()
{
    //console width: 55, height: 18

    bool windowShown = true;
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, 55, 18, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ShowWindow(consoleWindow, SW_NORMAL);

    bool toggle = false;
    bool userError = false;
    const int timeResolution = 3;

    const int maxCps = 5000;
    int cps = 0;
    std::string cpsString;

    const int darkRed = 4;
    const int lightRed = 12;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, lightRed);

    try
    {
        std::cout << "Enter desired cps: ";
        std::cin >> cpsString;
        cps = boost::lexical_cast<int>(cpsString);
        system("cls");
        std::cout << "Current cps is: " << cps << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Max cps is: " << maxCps << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Press Mouse button2 to toggle clicking" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Press Home to change cps" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Press Delete to minimize/maximize" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Press Pause to terminate the program" << std::endl;
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
            system("cls");
            std::cout << "Current cps is: " << cps << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Max cps is: " << maxCps << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Press Mouse button2 to toggle clicking" << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Press Home to change cps" << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Press Delete to minimize/maximize" << std::endl;
            std::cout << "" << std::endl;
            std::cout << "Press Pause to terminate the program" << std::endl;
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
                system("cls");
                std::cout << "Current cps is: " << cps << std::endl;
                std::cout << "" << std::endl;
                std::cout << "Max cps is: " << maxCps << std::endl;
                std::cout << "" << std::endl;
                std::cout << "Press Mouse button2 to toggle clicking" << std::endl;
                std::cout << "" << std::endl;
                std::cout << "Press Home to change cps" << std::endl;
                std::cout << "" << std::endl;
                std::cout << "Press Delete to minimize/maximize" << std::endl;
                std::cout << "" << std::endl;
                std::cout << "Press Pause to terminate the program" << std::endl;
            }
        }

        if (toggle)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
        Sleep(906 / cps);
    }
}