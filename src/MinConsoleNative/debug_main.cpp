﻿#ifdef _DEBUG

#include "MinConsoleNative.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    Window window;
    POINT pos = window.GetCenterPosOfWindowInDesktop();
    window.SetWindowPos(pos);



    //MinTimer timer;
    //MinInitTimer(&timer);
    //while (true)
    //{
    //    MinStartTimer(&timer);
    //    Sleep(10);
    //    double deltaTime = 0;
    //    MinStopTimer(&timer);
    //    MinTimeTimer(&timer, &deltaTime);
    //    cout << deltaTime << "\n";
    //}

    //Input::ResetMouseAxis();
    //Timer timer;
    //while (true)
    //{
    //    timer.Start();
    //    Sleep(100);
    //    timer.Stop();
    //    double deltaTime = timer.Time();
    //    Input::CheckMouseAxis();
    //    int diff = Input::GetMouseAxis(MouseAxis::MOUSE_X);
    //    cout << diff << "\n";
    //    if (Input::GetKeyDown('A'))
    //    {
    //        cout << deltaTime << "\n";
    //    }
    //}

    return 0;
}

#endif