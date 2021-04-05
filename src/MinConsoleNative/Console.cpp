﻿#include "Console.h"
#include "WinVersion.h"

namespace MinConsoleNative
{
    EXPORT_FUNC MinInitConsoleSession(ConsoleSession* cons)
    {
        cons->consoleInput = ::GetStdHandle(STD_INPUT_HANDLE);
        cons->consoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
        cons->consoleWindow = ::GetConsoleWindow();
        ::SetConsoleCP(65001);
        ::SetConsoleOutputCP(65001);
        return true;
    }

    EXPORT_FUNC MinEnableConsoleVT(ConsoleSession* cons)
    {
        bool supportVT = winVersionInstance.GetInstance().IsWindows10CreatorsOrLater();
        if (supportVT)
        {
            ConsoleMode cm;
            cm.outputMode._ENABLE_VIRTUAL_TERMINAL_PROCESSING = true;
            return MinSetConsoleMode(cons, cm);
        }
        return false;
    }

    EXPORT_FUNC MinGetConsolePalette(ConsoleSession* cons, DWORD index, Color24* color)
    {
        CONSOLE_SCREEN_BUFFER_INFOEX csbi;
        csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        ::GetConsoleScreenBufferInfoEx(cons->consoleOutput, &csbi);

        COLORREF colorRef = csbi.ColorTable[index];
        color->r = colorRef & 0x0000FF;
        color->g = (colorRef & 0x00FF00) >> 8;
        color->b = (colorRef & 0xFF0000) >> 16;
        return true;
    }

    EXPORT_FUNC MinSetConsolePalette(ConsoleSession* cons, DWORD index, Color24 color)
    {
        CONSOLE_SCREEN_BUFFER_INFOEX csbi;
        //get
        csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        ::GetConsoleScreenBufferInfoEx(cons->consoleOutput, &csbi);
        //set
        csbi.ColorTable[index] = color.r + (color.g << 8) + (color.b << 16);
        return ::SetConsoleScreenBufferInfoEx(cons->consoleOutput, &csbi);
    }

    EXPORT_FUNC MinGetConsoleMode(ConsoleSession* cons, ConsoleMode* consoleMode)
    {
        ConsoleInputMode cim;
        ConsoleOutputMode com;

        ConsoleInputMode* cim_ptr = &cim;
        ConsoleOutputMode* com_ptr = &com;

        DWORD inputMode = 0, outputMode = 0;
        ::GetConsoleMode(cons->consoleInput, &inputMode);
        ::GetConsoleMode(cons->consoleOutput, &outputMode);

        cim_ptr->_ENABLE_PROCESSED_INPUT =
            (inputMode & ENABLE_PROCESSED_INPUT) == ENABLE_PROCESSED_INPUT;
        cim_ptr->_ENABLE_LINE_INPUT =
            (inputMode & ENABLE_LINE_INPUT) == ENABLE_LINE_INPUT;
        cim_ptr->_ENABLE_ECHO_INPUT =
            (inputMode & ENABLE_ECHO_INPUT) == ENABLE_ECHO_INPUT;
        cim_ptr->_ENABLE_WINDOW_INPUT =
            (inputMode & ENABLE_WINDOW_INPUT) == ENABLE_WINDOW_INPUT;
        cim_ptr->_ENABLE_MOUSE_INPUT =
            (inputMode & ENABLE_MOUSE_INPUT) == ENABLE_MOUSE_INPUT;
        cim_ptr->_ENABLE_INSERT_MODE =
            (inputMode & ENABLE_INSERT_MODE) == ENABLE_INSERT_MODE;
        cim_ptr->_ENABLE_QUICK_EDIT_MODE =
            (inputMode & ENABLE_QUICK_EDIT_MODE) == ENABLE_QUICK_EDIT_MODE;
        cim_ptr->_ENABLE_EXTENDED_FLAGS =
            (inputMode & ENABLE_EXTENDED_FLAGS) == ENABLE_EXTENDED_FLAGS;
        cim_ptr->_ENABLE_AUTO_POSITION =
            (inputMode & ENABLE_AUTO_POSITION) == ENABLE_AUTO_POSITION;
        cim_ptr->_ENABLE_VIRTUAL_TERMINAL_INPUT =
            (inputMode & ENABLE_VIRTUAL_TERMINAL_INPUT) == ENABLE_VIRTUAL_TERMINAL_INPUT;

        com_ptr->_ENABLE_PROCESSED_OUTPUT =
            (outputMode & ENABLE_PROCESSED_OUTPUT) == ENABLE_PROCESSED_OUTPUT;
        com_ptr->_ENABLE_WRAP_AT_EOL_OUTPUT =
            (outputMode & ENABLE_WRAP_AT_EOL_OUTPUT) == ENABLE_WRAP_AT_EOL_OUTPUT;
        com_ptr->_ENABLE_VIRTUAL_TERMINAL_PROCESSING =
            (outputMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        com_ptr->_DISABLE_NEWLINE_AUTO_RETURN =
            (outputMode & DISABLE_NEWLINE_AUTO_RETURN) == DISABLE_NEWLINE_AUTO_RETURN;
        com_ptr->_ENABLE_LVB_GRID_WORLDWIDE =
            (outputMode & ENABLE_LVB_GRID_WORLDWIDE) == ENABLE_LVB_GRID_WORLDWIDE;

        consoleMode->inputMode = cim;
        consoleMode->outputMode = com;

        return true;
    }

    EXPORT_FUNC MinSetConsoleMode(ConsoleSession* cons, ConsoleMode consoleMode)
    {
        DWORD inputMode = 0, outputMode = 0;

        const ConsoleInputMode* cim_ptr = &consoleMode.inputMode;
        const ConsoleOutputMode* com_ptr = &consoleMode.outputMode;

        if (cim_ptr->_ENABLE_PROCESSED_INPUT)
            inputMode |= ENABLE_PROCESSED_INPUT;
        if (cim_ptr->_ENABLE_LINE_INPUT)
            inputMode |= ENABLE_LINE_INPUT;
        if (cim_ptr->_ENABLE_ECHO_INPUT)
            inputMode |= ENABLE_ECHO_INPUT;
        if (cim_ptr->_ENABLE_WINDOW_INPUT)
            inputMode |= ENABLE_WINDOW_INPUT;
        if (cim_ptr->_ENABLE_MOUSE_INPUT)
            inputMode |= ENABLE_MOUSE_INPUT;
        if (cim_ptr->_ENABLE_INSERT_MODE)
            inputMode |= ENABLE_INSERT_MODE;
        if (cim_ptr->_ENABLE_QUICK_EDIT_MODE)
            inputMode |= ENABLE_QUICK_EDIT_MODE;
        if (cim_ptr->_ENABLE_EXTENDED_FLAGS)
            inputMode |= ENABLE_EXTENDED_FLAGS;
        if (cim_ptr->_ENABLE_AUTO_POSITION)
            inputMode |= ENABLE_AUTO_POSITION;
        if (cim_ptr->_ENABLE_VIRTUAL_TERMINAL_INPUT)
            inputMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;

        if (com_ptr->_ENABLE_PROCESSED_OUTPUT)
            outputMode |= ENABLE_PROCESSED_OUTPUT;
        if (com_ptr->_ENABLE_WRAP_AT_EOL_OUTPUT)
            outputMode |= ENABLE_WRAP_AT_EOL_OUTPUT;
        if (com_ptr->_ENABLE_VIRTUAL_TERMINAL_PROCESSING)
            outputMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (com_ptr->_DISABLE_NEWLINE_AUTO_RETURN)
            outputMode |= DISABLE_NEWLINE_AUTO_RETURN;
        if (com_ptr->_ENABLE_LVB_GRID_WORLDWIDE)
            outputMode |= ENABLE_LVB_GRID_WORLDWIDE;

        bool is = ::SetConsoleMode(cons->consoleInput, inputMode);
        bool os = ::SetConsoleMode(cons->consoleOutput, outputMode);
        return is && os;
    }

    EXPORT_FUNC MinGetConsoleFont(ConsoleSession* cons, ConsoleFont* consoleFont)
    {
        //get
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        ::GetCurrentConsoleFontEx(cons->consoleOutput, false, &cfi);
        //set
        consoleFont->FontIndex = cfi.nFont;
        consoleFont->FontSize = cfi.dwFontSize;
        consoleFont->FontFamily = cfi.FontFamily;
        consoleFont->FontWeight = cfi.FontWeight;
        size_t fontNameLength = wcslen(cfi.FaceName);
        ::wcscpy_s(consoleFont->FaceName, fontNameLength + 1, cfi.FaceName);
        return true;
    }

    EXPORT_FUNC MinSetConsoleFont(ConsoleSession* cons, ConsoleFont consoleFont)
    {
        //get
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        ::GetCurrentConsoleFontEx(cons->consoleOutput, false, &cfi);
        //set
        cfi.dwFontSize = consoleFont.FontSize;
        cfi.FontWeight = consoleFont.FontWeight;
        size_t fontNameLength = wcslen(consoleFont.FaceName);
        ::wcscpy_s(cfi.FaceName, fontNameLength + 1, consoleFont.FaceName);
        return ::SetCurrentConsoleFontEx(cons->consoleOutput, false, &cfi);
    }

    EXPORT_FUNC MinGetConsoleWindowSize(ConsoleSession* cons, POINT* size)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        ::GetConsoleScreenBufferInfo(cons->consoleOutput, &csbi);
        size->x = (csbi.srWindow.Right - csbi.srWindow.Left) + 1;
        size->y = (csbi.srWindow.Bottom - csbi.srWindow.Top) + 1;
        return true;
    }

    EXPORT_FUNC MinSetConsoleWindowSize(ConsoleSession* cons, POINT size)
    {
        SMALL_RECT sm;
        sm.Left = 0;
        sm.Top = 0;
        sm.Right = size.x - 1;
        sm.Bottom = size.y - 1;
        return ::SetConsoleWindowInfo(cons->consoleOutput, true, &sm);
    }

    EXPORT_FUNC MinGetConsoleBufferSize(ConsoleSession* cons, POINT* size)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        ::GetConsoleScreenBufferInfo(cons->consoleOutput, &csbi);
        size->x = csbi.dwSize.X;
        size->y = csbi.dwSize.Y;
        return true;
    }

    EXPORT_FUNC MinSetConsoleBufferSize(ConsoleSession* cons, POINT size)
    {
        COORD coord;
        coord.X = size.x;
        coord.Y = size.y;
        return ::SetConsoleScreenBufferSize(cons->consoleOutput, coord);
    }

    EXPORT_FUNC MinGetConsoleForeColor(ConsoleSession* cons, ConsoleColor* foreColor)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        ::GetConsoleScreenBufferInfo(cons->consoleOutput, &csbi);
        *foreColor = (ConsoleColor)(csbi.wAttributes & 0x000F);
        return true;
    }

    EXPORT_FUNC MinGetConsoleBackColor(ConsoleSession* cons, ConsoleColor* backColor)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        ::GetConsoleScreenBufferInfo(cons->consoleOutput, &csbi);
        *backColor = (ConsoleColor)((csbi.wAttributes & 0x00F0) / 16);
        return true;
    }

    EXPORT_FUNC MinSetConsoleForeColor(ConsoleSession* cons, ConsoleColor foreColor)
    {
        ConsoleColor backColor = ConsoleColor::BLACK;
        MinGetConsoleBackColor(cons, &backColor);
        ushort att = (ushort)((ushort)foreColor | ((ushort)backColor << 4));
        return ::SetConsoleTextAttribute(cons->consoleOutput, att);
    }

    EXPORT_FUNC MinSetConsoleBackColor(ConsoleSession* cons, ConsoleColor backColor)
    {
        ConsoleColor foreColor = ConsoleColor::GRAY;
        MinGetConsoleForeColor(cons, &foreColor);
        ushort att = (ushort)((ushort)foreColor | ((ushort)backColor << 4));
        return ::SetConsoleTextAttribute(cons->consoleOutput, att);
    }

    EXPORT_FUNC MinGetConsoleCursorPos(ConsoleSession* cons, COORD* pos)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        ::GetConsoleScreenBufferInfo(cons->consoleOutput, &csbi);
        *pos = csbi.dwCursorPosition;
        return true;
    }

    EXPORT_FUNC MinSetConsoleCursorPos(ConsoleSession* cons, COORD pos)
    {
        return ::SetConsoleCursorPosition(cons->consoleOutput, pos);
    }

    Console::Console()
    {
        MinInitConsoleSession(&cons);
        MinEnableConsoleVT(&cons);
    }

    Console::Console(ConsoleSession cons)
    {
        this->cons = cons;
        MinEnableConsoleVT(&cons);
    }

    Console::Console(HWND consoleWindow, HANDLE consoleInput, HANDLE consoleOutput)
    {
        this->cons = ConsoleSession(consoleWindow, consoleInput, consoleOutput);
        MinEnableConsoleVT(&cons);
    }

    Color24 Console::GetConsolePalette(DWORD index)
    {
        Color24 color;
        MinGetConsolePalette(&cons, index, &color);
        return color;
    }

    bool Console::SetConsolePalette(DWORD index, const Color24& color)
    {
        return MinSetConsolePalette(&cons, index, color);
    }

    ConsoleMode Console::GetConsoleMode()
    {
        ConsoleMode cm;
        MinGetConsoleMode(&cons, &cm);
        return cm;
    }

    bool Console::SetConsoleMode(const ConsoleMode& consoleMode)
    {
        return MinSetConsoleMode(&cons, consoleMode);
    }

    ConsoleFont Console::GetConsoleFont()
    {
        ConsoleFont consoleFont;
        MinGetConsoleFont(&cons, &consoleFont);
        return consoleFont;
    }

    bool Console::SetConsoleFont(const ConsoleFont& consoleFont)
    {
        return MinSetConsoleFont(&cons, consoleFont);
    }

    POINT Console::GetConsoleWindowSize()
    {
        POINT point;
        MinGetConsoleWindowSize(&cons, &point);
        return point;
    }

    bool Console::SetConsoleWindowSize(POINT size)
    {
        return MinSetConsoleWindowSize(&cons, size);
    }

    POINT Console::GetConsoleBufferSize()
    {
        POINT size;
        MinGetConsoleBufferSize(&cons, &size);
        return size;
    }

    bool Console::SetConsoleBufferSize(POINT size)
    {
        return MinSetConsoleBufferSize(&cons, size);
    }

    ConsoleColor Console::GetConsoleForeColor()
    {
        ConsoleColor foreColor;
        MinGetConsoleForeColor(&cons, &foreColor);
        return foreColor;
    }

    ConsoleColor Console::GetConsoleBackColor()
    {
        ConsoleColor backColor;
        MinGetConsoleBackColor(&cons, &backColor);
        return backColor;
    }

    bool Console::SetConsoleForeColor(ConsoleColor foreColor)
    {
        return MinSetConsoleForeColor(&cons, foreColor);
    }

    bool Console::SetConsoleBackColor(ConsoleColor backColor)
    {
        return MinSetConsoleBackColor(&cons, backColor);
    }

    COORD Console::GetConsoleCursorPos()
    {
        COORD pos;
        MinGetConsoleCursorPos(&cons, &pos);
        return pos;
    }

    bool Console::SetConsoleCursorPos(COORD pos)
    {
        return MinSetConsoleCursorPos(&cons, pos);
    }
}