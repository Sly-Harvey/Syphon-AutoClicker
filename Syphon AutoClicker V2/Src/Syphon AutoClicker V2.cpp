#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <Windows.h>
#include "boost/lexical_cast.hpp"

INPUT mouseInput[2]; // do not put this is main function because your pc will crash and you will have a black screen.

std::future<void> inputsFuture;

#if PR_DEBUG == 1
std::thread::id mainID;
std::thread::id inputHandling_ID;
#endif

bool windowShown = true;
HWND consoleWindow = GetConsoleWindow();

bool toggle = false;
std::string toggleDisplay = "False";

bool userError = false;

int timeResolution = 3;
TIMECAPS timeCap;

const int maxCps = 5000;
int cps = 0;
std::string cpsString;

static std::mutex dataLock;

const int darkRed = 4;
const int lightRed = 12;

void menu(int cps, int maxCps, std::string toggleDisplay)
{
    system("cls");
#if PR_DEBUG == 1
    std::cout << " [DEBUG MODE]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << " [DEBUG]Time Resolution: " << timeResolution << std::endl;
    std::cout << "" << std::endl;
    std::cout << " [DEBUG]Main Thread id: " << mainID << std::endl;
    std::cout << "" << std::endl;
    std::cout << " [DEBUG]InputHandling Thread id: " << inputHandling_ID << std::endl;
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
            std::lock_guard<std::mutex> lock(dataLock);
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
            std::lock_guard<std::mutex> lock(dataLock);
            userError = false;
            toggle = false;
            toggleDisplay = "False";
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
            std::lock_guard<std::mutex> lock(dataLock);
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
                std::lock_guard<std::mutex> lock(dataLock);
                SetForegroundWindow(consoleWindow);
                system("cls");
                std::cout << " Enter desired cps: ";
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
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 8));
    }
}

int main()
{
#if PR_DEBUG == 1
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 380, 400, SWP_SHOWWINDOW);
#elif defined(PR_RELEASE)
    SetWindowPos(consoleWindow, HWND_TOPMOST, 700, 400, 380, 280, SWP_SHOWWINDOW);
#endif
    SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
    SetConsoleTitleA("Syphon AutoClicker");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lightRed);

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

    //inputsFuture = std::async(std::launch::async, inputHandling);
    std::thread inputHandling_Thread(inputHandling);
#if PR_DEBUG == 1
    mainID = std::this_thread::get_id();
    inputHandling_ID = inputHandling_Thread.get_id();
#endif

    try
    {
        std::cout << " Enter desired cps: ";
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
        if (toggle)
        {
            SendInput(2, mouseInput, sizeof(INPUT));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / (cps + 1)));
    }
}