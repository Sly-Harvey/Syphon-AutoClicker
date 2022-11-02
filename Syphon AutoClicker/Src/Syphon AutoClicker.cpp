#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
#include <vector>
#include <Windows.h>
#include "boost/lexical_cast.hpp"

INPUT mouseInput[2]; // do not put this is main function because your pc will crash and you will have a black screen.
INPUT multiMouseInput[2]; // do not put this is main function because your pc will crash and you will have a black screen.

bool windowShown = true;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND consoleWindow = GetConsoleWindow();
CONSOLE_SCREEN_BUFFER_INFO endConsoleCurserPos;

bool multiTargetMode = false;
bool multiToggle = false;

bool toggle = false;
std::string toggleDisplay = "False";

std::vector<POINT> cursorPositions;
int multiClicksPerSecond = 0;
int maxMultiClicksPerSecond = 1000;
bool rememberMultiSettings = false;
std::string multiClicksPerSecondString;

bool userError = false;
bool multiUserError = false;

int timeResolution = 3;
TIMECAPS timeCap;

int maxCps = 5000;
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

void menu(int& cps, int& maxCps, std::string& toggleDisplay)
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
    std::cout << " Press Insert to go to multi target mode" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Home to change cps" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Delete to minimize/maximize" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Pause to terminate the program" << std::endl;

    GetConsoleScreenBufferInfo(hConsole, &endConsoleCurserPos);
}

void mutiTargetmenu(std::string& toggleDisplay)
{
    ShowConsoleCursor(false);
    system("cls");
#if PR_DEBUG == 1
    std::cout << " [DEBUG MODE]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " [DEBUG]Time Resolution: " << timeResolution << std::endl;
    std::cout << "" << std::endl;
#endif
    std::cout << " [MULTI TARGET MODE]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Clicking: " << toggleDisplay << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Positions: " << cursorPositions.size() << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Clicks per second: " << multiClicksPerSecond << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Max Clicks per second: " << maxMultiClicksPerSecond << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press F8 to toggle clicking" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press F7 to delete all positions" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press PageUp to add a location" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press PageDown to delete a location" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Insert to go to single target mode" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Home to change cps" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Delete to minimize/maximize" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Press Pause to terminate the program" << std::endl;

    GetConsoleScreenBufferInfo(hConsole, &endConsoleCurserPos);
}

void inputHandling()
{
    while (true)
    {

        if (GetAsyncKeyState(VK_PAUSE) & 1)
        {
            cursorPositions.clear();
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

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            userError = false;
            toggle = false;
            toggleDisplay = "False";
            ShowConsoleCursor(true);
            SetForegroundWindow(consoleWindow);

            if (multiTargetMode == false && rememberMultiSettings == false)
            {
                multiToggle = false;
                multiTargetMode = true;
                rememberMultiSettings = true;
                SetConsoleTextAttribute(hConsole, darkRed);

#if PR_DEBUG == 1
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 520, SWP_NOMOVE);
#elif defined(PR_RELEASE)
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 460, SWP_NOMOVE);
#endif
                system("cls");
                std::cout << " Enter desired clicks per second: ";
                std::cin >> multiClicksPerSecondString;
                try
                {
                    multiClicksPerSecond = boost::lexical_cast<int>(multiClicksPerSecondString);
                }
                catch (boost::bad_lexical_cast e)
                {
                    multiUserError = true;
                }

                mutiTargetmenu(toggleDisplay);
            }
            else if (multiTargetMode == false && rememberMultiSettings == true)
            {
                multiToggle = false;
                multiTargetMode = true;
                SetConsoleTextAttribute(hConsole, darkRed);

#if PR_DEBUG == 1
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 520, SWP_NOMOVE);
#elif defined(PR_RELEASE)
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 460, SWP_NOMOVE);
#endif

                mutiTargetmenu(toggleDisplay);
            }
            else if (multiTargetMode == true)
            {
                userError = false;
                toggle = false;
                toggleDisplay = "False";
                multiToggle = false;
                multiTargetMode = false;
                //ZeroMemory(mouseInput, sizeof(INPUT));
                SetConsoleTextAttribute(hConsole, lightRed);
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 300, SWP_NOMOVE);
                menu(cps, maxCps, toggleDisplay);
            }
        }

        if (multiTargetMode == true)
        {
            if (GetAsyncKeyState(VK_F8) & 1)
            {
                multiToggle = !multiToggle;
                if (multiToggle)
                {
                    timeBeginPeriod(timeResolution);
                }
                else if (!multiToggle)
                {
                    timeEndPeriod(timeResolution);
                }

                toggleDisplay = multiToggle ? "True" : "False";

#if PR_DEBUG == 1
                ChangeCurserPos(11, 6);
#elif defined(PR_RELEASE)
                ChangeCurserPos(11, 2);
#endif
                std::cout << toggleDisplay << " " << std::endl;
                SetConsoleCursorPosition(hConsole, endConsoleCurserPos.dwCursorPosition);
            }

            if (GetAsyncKeyState(VK_F7) & 1)
            {
                cursorPositions.clear();

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif

                std::cout << cursorPositions.size() << "     " << std::endl;
                SetConsoleCursorPosition(hConsole, endConsoleCurserPos.dwCursorPosition);
            }

            if (GetAsyncKeyState(VK_PRIOR) & 1)
            {
                POINT cursorPos;
                GetCursorPos(&cursorPos);
                cursorPositions.push_back(cursorPos);

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif
                std::cout << cursorPositions.size() << "     " << std::endl;
                SetConsoleCursorPosition(hConsole, endConsoleCurserPos.dwCursorPosition);
            }
            else if (GetAsyncKeyState(VK_NEXT) & 1)
            {
                if (!cursorPositions.empty())
                {
                    cursorPositions.pop_back();
                }

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif
                std::cout << cursorPositions.size() << " " << std::endl;
                SetConsoleCursorPosition(hConsole, endConsoleCurserPos.dwCursorPosition);
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
            std::cout << " Only whole numbers fewer than " << maxCps << " are allowed." << std::endl;
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
            if (multiTargetMode == true)
                mutiTargetmenu(toggleDisplay);
            else
                menu(cps, maxCps, toggleDisplay);
        }

        while (multiUserError || multiClicksPerSecond > maxMultiClicksPerSecond)
        {
            multiUserError = false;
            userError = false;
            multiToggle = false;
            multiClicksPerSecond = 0;
            toggleDisplay = "False";
            ShowConsoleCursor(true);

            system("cls");
            std::cout << " Only whole numbers are allowed" << std::endl;
            std::cout << "";
            std::cout << " Max multi clicks per second is " << maxMultiClicksPerSecond << std::endl;
            std::cout << "";
            std::cout << " Please try again." << std::endl;
            std::cout << "" << std::endl;
            std::cout << " Enter desired clicks per second: ";
            std::cin >> multiClicksPerSecondString;
            try
            {
                multiClicksPerSecond = boost::lexical_cast<int>(multiClicksPerSecondString);
            }
            catch (boost::bad_lexical_cast e)
            {
                multiUserError = true;
            }

            if (multiTargetMode == true)
                mutiTargetmenu(toggleDisplay);
            else
                menu(cps, maxCps, toggleDisplay);
        }

        if (multiTargetMode == false)
        {
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
        }
        if (windowShown)
        {
            if (multiTargetMode == false)
            {
                if (GetAsyncKeyState(VK_HOME) & 1)
                {
                    multiUserError = false;
                    userError = false;
                    toggle = false;
                    multiToggle = false;
                    toggleDisplay = "False";

                    std::cout << std::flush;
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
                    if (multiTargetMode == true)
                        mutiTargetmenu(toggleDisplay);
                    else
                        menu(cps, maxCps, toggleDisplay);
                }
            }
            else if (multiTargetMode == true)
            {
                if (GetAsyncKeyState(VK_HOME) & 1)
                {
                    multiUserError = false;
                    userError = false;
                    toggle = false;
                    multiToggle = false;
                    toggleDisplay = "False";

                    std::cout << std::flush;
                    ShowConsoleCursor(true);
                    SetForegroundWindow(consoleWindow);
                    system("cls");
                    std::cout << " Enter desired clicks per second: ";
                    std::cin >> multiClicksPerSecondString;
                    try
                    {
                        multiClicksPerSecond = boost::lexical_cast<int>(multiClicksPerSecondString);
                    }
                    catch (boost::bad_lexical_cast e)
                    {
                        multiUserError = true;
                    }

                    std::cout.flush();

                    mutiTargetmenu(toggleDisplay);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 8));
    }
}

int main()
{
#if PR_DEBUG == 1
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 360, SWP_SHOWWINDOW);
#elif defined(PR_RELEASE)
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 300, SWP_SHOWWINDOW);
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
        if (multiTargetMode == true)
            mutiTargetmenu(toggleDisplay);
        else
            menu(cps, maxCps, toggleDisplay);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(905 / cps));
        }
        else if (multiToggle)
        {
            for(POINT i : cursorPositions)
            {
                SetCursorPos(i.x, i.y);
                
                multiMouseInput[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                multiMouseInput[0].mi.dx = i.x;
                multiMouseInput[0].mi.dy = i.y;
                multiMouseInput[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                multiMouseInput[1].mi.dx = i.x;
                multiMouseInput[1].mi.dy = i.y;
                
                SendInput(2, multiMouseInput, sizeof(INPUT));
                std::this_thread::sleep_for(std::chrono::milliseconds(905/multiClicksPerSecond));
            }
        }
    }
}