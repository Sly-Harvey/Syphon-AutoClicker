#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
#include <Windows.h>
#include "boost/lexical_cast.hpp"

INPUT mouseInput[2]; // do not put this is main function because your pc will crash and you will have a black screen.

bool windowShown = true;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND consoleWindow = GetConsoleWindow();
CONSOLE_SCREEN_BUFFER_INFO endConsoleCurserPos;

bool toggle = false;
std::string toggleDisplay = "False";

bool userError = false;

int timeResolution = 3;
TIMECAPS timeCap;

const int maxCps = 5000;
int cps = 0;
std::string cpsString;

const int darkRed = 4;
const int lightRed = 12;

void ChangeCurserPos(int x, int y)
{
    COORD newCurserPos = { x, y };
    SetConsoleCursorPosition(hConsole, newCurserPos);
}

void ShowConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void menu(int cps, int maxCps, std::string toggleDisplay)
{
    ShowConsoleCursor(false);
    system("cls");
#if PR_DEBUG == 1
    std::cout << " [DEBUG MODE]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " [DEBUG]Time Resolution: " << timeResolution << std::endl;
    std::cout << "" << std::endl;
#endif
    std::cout << " Clicking: " << toggleDisplay << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Current cps: " << cps << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Max cps: " << maxCps << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Mouse Button 5 to toggle clicking" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Home to change cps" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Delete to minimize/maximize" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Pause to terminate the program" << std::endl;
}

void inputHandling()
{
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
            ShowConsoleCursor(true);
            cps = 0;

            system("cls");
            std::cout << " Only whole numbers less than " << maxCps << " are allowed." << std::endl;
            std::cout << " Please try again." << std::endl;
            std::cout << "" << std::endl;
            std::cout << " Enter desired cps: ";
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

#if PR_DEBUG == 1
            ChangeCurserPos(11, 4);
#elif defined(PR_RELEASE)
            ChangeCurserPos(11, 0);
#endif
            std::cout << toggleDisplay << " " << std::endl;
            SetConsoleCursorPosition(hConsole, endConsoleCurserPos.dwCursorPosition);
        }

        if (windowShown)
        {
            if (GetAsyncKeyState(VK_HOME) & 1)
            {
                ShowConsoleCursor(true);
                SetForegroundWindow(consoleWindow);
                system("cls");
                std::cout << " Enter desired cps: ";
                std::cin >> cpsString;
                try
                {
                    cps = boost::lexical_cast<int>(cpsString);
                    //the 2 lines below are for auto minimize after changing cps

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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 8));
    }
}

int main()
{
#if PR_DEBUG == 1
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 330, SWP_SHOWWINDOW);
#elif defined(PR_RELEASE)
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 270, SWP_SHOWWINDOW);
#endif
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    SetConsoleTextAttribute(hConsole, lightRed);
    SetConsoleTitleA("Syphon AutoClicker");

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

    timeGetDevCaps(&timeCap, sizeof(timeCap));
    if (timeCap.wPeriodMin <= 1)
    {
        timeResolution = 1;
    }
    else if(timeCap.wPeriodMin > 1)
    {
        timeResolution = timeCap.wPeriodMin;
    }

    try
    {
        std::cout << " Enter desired cps: ";
        std::cin >> cpsString;
        cps = boost::lexical_cast<int>(cpsString);
        menu(cps, maxCps, toggleDisplay);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        GetConsoleScreenBufferInfo(hConsole, &endConsoleCurserPos);
    }
    catch (boost::bad_lexical_cast e)
    {
        userError = true;
    }

    std::thread inputHandling_Thread(inputHandling);

    while (true)
    {
        if (toggle)
        {
            SendInput(2, mouseInput, sizeof(INPUT));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / (cps + 1)));
    }
}