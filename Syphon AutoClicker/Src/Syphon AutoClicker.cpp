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
HANDLE hConsoleOutput, hConsoleInput;
HWND consoleWindow = GetConsoleWindow();
CONSOLE_SCREEN_BUFFER_INFO endConsoleCurserPos;

bool multiTargetMode = false;
bool multiToggle = false;

bool toggle = false;
std::string toggleDisplay = "False";

POINT cursorPos;
std::vector<POINT> cursorPositions;

int multiClicksPerSecond = 0;
const int maxMultiClicksPerSecond = 1000;
std::string multiClicksPerSecondString;

bool rememberMultiSettings = false;

bool userError = false;
bool multiUserError = false;

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
    SetConsoleCursorPosition(hConsoleOutput, newCurserPos);
}

void setConsoleBufferSize(int width, int height)
{
    SMALL_RECT r;
    COORD c;

    r.Left = 0;
    r.Top = 0;
    r.Right = width;
    r.Bottom = height;

    c.X = width;
    c.Y = height;

    SetConsoleScreenBufferSize(hConsoleOutput, c);
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &r);
}

void ShowConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

void menu()
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
    //std::cout << " Max cps: " << maxCps << std::endl;
    //std::cout << "" << std::endl;
    std::cout << " Mouse Button 5 to toggle clicking" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Insert to go to multi target mode" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Home to change cps" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Delete to minimize/maximize" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Pause to terminate the program" << std::endl;

    GetConsoleScreenBufferInfo(hConsoleOutput, &endConsoleCurserPos);
}

void mutiTargetmenu()
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
    //std::cout << " Max Clicks per second: " << maxMultiClicksPerSecond << std::endl;
    //std::cout << "" << std::endl;
    std::cout << " F8 to toggle clicking" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Alt+R to delete all positions" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Alt+Z to add a position" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Alt+X to delete last position" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Ctrl+Alt to add/remove 5 positions" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Insert to go to single target mode" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Home to change cps" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Delete to minimize/maximize" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " Pause to terminate the program" << std::endl;

    GetConsoleScreenBufferInfo(hConsoleOutput, &endConsoleCurserPos);
}

void inputHandling()
{
    while (true)
    {

        

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            userError = false;
            toggle = false;
            toggleDisplay = "False";

            if (multiTargetMode == false && rememberMultiSettings == false)
            {
                multiToggle = false;
                multiTargetMode = true;
                rememberMultiSettings = true;
                SetConsoleTextAttribute(hConsoleOutput, darkRed);
                if(windowShown)
                    SetForegroundWindow(consoleWindow);

#if PR_DEBUG == 1
                //untested
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 315, 485, SWP_NOMOVE);
                setConsoleBufferSize(37, 30);
#elif defined(PR_RELEASE)
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 315, 455, SWP_NOMOVE);
                setConsoleBufferSize(37, 26);
#endif
                system("cls");
                FlushConsoleInputBuffer(hConsoleInput);

                std::cout << " Enter desired cps: " << std::flush;
                std::cin >> multiClicksPerSecondString;
                try
                {
                    multiClicksPerSecond = boost::lexical_cast<int>(multiClicksPerSecondString);
                }
                catch (boost::bad_lexical_cast e)
                {
                    multiUserError = true;
                }

                mutiTargetmenu();
            }
            else if (multiTargetMode == false && rememberMultiSettings == true)
            {
                multiToggle = false;
                multiTargetMode = true;
                SetConsoleTextAttribute(hConsoleOutput, darkRed);
                if (windowShown)
                    SetForegroundWindow(consoleWindow);

#if PR_DEBUG == 1
                //untested
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 315, 485, SWP_NOMOVE);
                setConsoleBufferSize(37, 30);
#elif defined(PR_RELEASE)
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 315, 455, SWP_NOMOVE);
                setConsoleBufferSize(37, 26);
#endif

                mutiTargetmenu();
            }
            else if (multiTargetMode == true)
            {
                userError = false;
                toggle = false;
                toggleDisplay = "False";
                multiToggle = false;
                multiTargetMode = false;
                SetConsoleTextAttribute(hConsoleOutput, lightRed);
                if (windowShown)
                    SetForegroundWindow(consoleWindow);
#if PR_DEBUG == 1
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 305, 325, SWP_NOMOVE);
                setConsoleBufferSize(36, 18);
#elif defined(PR_RELEASE)
                SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 305, 265, SWP_NOMOVE);
                setConsoleBufferSize(36, 14);
#endif
                menu();
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
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
            }

            if (GetAsyncKeyState(VK_MENU) & 0x8000 && GetAsyncKeyState(0x52) & 1)
            {
                cursorPositions.clear();

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif

                std::cout << cursorPositions.size() << "     " << std::endl;
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
            }

            if (GetAsyncKeyState(VK_MENU) & 0x8000 && (GetAsyncKeyState(VK_CONTROL) & 0x8000) == 0 && GetAsyncKeyState(0x5A) & 1)
            {
                GetCursorPos(&cursorPos);
                cursorPositions.push_back(cursorPos);

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif
                std::cout << cursorPositions.size() << "     " << std::endl;
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
            }
            else if (GetAsyncKeyState(VK_MENU) & 0x8000 && (GetAsyncKeyState(VK_CONTROL) & 0x8000) == 0 && GetAsyncKeyState(0x58) & 1)
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
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
            }
            else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000 && GetAsyncKeyState(0x5A) & 1)
            {
                GetCursorPos(&cursorPos);
                for (int i = 0; i < 5; i++)
                {
                    cursorPositions.push_back(cursorPos);
                }

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif
                std::cout << cursorPositions.size() << "     " << std::endl;
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
            }
            else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000 && GetAsyncKeyState(0x58) & 1)
            {
                if (cursorPositions.size() >= 5)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        cursorPositions.pop_back();
                    }
                }
                else
                {
                    cursorPositions.clear();
                }

#if PR_DEBUG == 1
                ChangeCurserPos(12, 4);
#elif defined(PR_RELEASE)
                ChangeCurserPos(12, 4);
#endif
                std::cout << cursorPositions.size() << " " << std::endl;
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
            }
        }
        
        while (userError || cps > maxCps)
        {
            userError = false;
            toggle = false;
            toggleDisplay = "False";
            cps = 0;
            ShowConsoleCursor(true);
            system("cls");
            FlushConsoleInputBuffer(hConsoleInput);

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
                mutiTargetmenu();
            else
                menu();
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
            FlushConsoleInputBuffer(hConsoleInput);

            std::cout << " Only whole numbers are allowed." << std::endl;
            std::cout << " Max multi clicks per second is " << maxMultiClicksPerSecond << '.' << std::endl;
            std::cout << " Please try again." << std::endl;
            std::cout << "" << std::endl;
            std::cout << " Enter desired clicks per second: " << std::flush;
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
                mutiTargetmenu();
            else
                menu();
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
                SetConsoleCursorPosition(hConsoleOutput, endConsoleCurserPos.dwCursorPosition);
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
                    ShowConsoleCursor(true);
                    SetForegroundWindow(consoleWindow);
                    system("cls");
                    FlushConsoleInputBuffer(hConsoleInput);

                    std::cout << " Enter desired cps: " << std::flush;
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
                        mutiTargetmenu();
                    else
                        menu();
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
                    FlushConsoleInputBuffer(hConsoleInput);

                    std::cout << " Enter desired cps: " << std::flush;
                    std::cin >> multiClicksPerSecondString;
                    try
                    {
                        multiClicksPerSecond = boost::lexical_cast<int>(multiClicksPerSecondString);
                    }
                    catch (boost::bad_lexical_cast e)
                    {
                        multiUserError = true;
                    }

                    mutiTargetmenu();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 8));
    }
}

int main()
{
    hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
#if PR_DEBUG == 1
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 305, 325, SWP_SHOWWINDOW);
    setConsoleBufferSize(36, 18);
#elif defined(PR_RELEASE)
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 305, 265, SWP_SHOWWINDOW); // original SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 310, 300, SWP_SHOWWINDOW);
    setConsoleBufferSize(36, 14); // original setConsoleBufferSize(36, 16);
#endif

    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    SetConsoleTextAttribute(hConsoleOutput, lightRed);
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

    std::cout << " Enter desired cps: " << std::flush;
    std::cin >> cpsString;
    try
    {
        cps = boost::lexical_cast<int>(cpsString);
        if (multiTargetMode == true)
            mutiTargetmenu();
        else
            menu();
    }
    catch (boost::bad_lexical_cast e)
    {
        userError = true;
    }

    std::thread inputHandling_Thread(inputHandling);

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

#if PR_DEBUG == 1
                if (multiTargetMode)
                {
                    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 520, SWP_NOMOVE);
                }
                else if (!multiTargetMode)
                {
                    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 360, SWP_NOMOVE);
                    setConsoleBufferSize(42, 20);
                    menu();
                }
#elif defined(PR_RELEASE)
                if (multiTargetMode)
                {
                    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 460, SWP_NOMOVE);
                }
                else if (!multiTargetMode)
                {
                    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 370, 300, SWP_NOMOVE);
                    setConsoleBufferSize(42, 16);
                    menu();
                }
#endif
            }
            if (!windowShown)
            {
                ShowWindow(consoleWindow, SW_MINIMIZE);
            }
        }

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
        else if (!toggle && !multiToggle)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}