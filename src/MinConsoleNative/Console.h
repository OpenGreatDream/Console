﻿#pragma once

#include "MinDefines.h"
#include "Singleton.h"
#include <utility>
#include <string>

#define CONSOLE_INPUT_STRING  L"CONIN$"
#define CONSOLE_OUTPUT_STRING L"CONOUT$"
#define CONSOLE_GENERIC       L"CON"

namespace MinConsoleNative
{
    EXPORT_ENUM_CLASS ConsoleType
    {
        Unknown = 0,
        WindowsLegacyConsole = 1,
        WindowsConsole = 2,
        WindowsTerminal = 4,
    };

    EXPORT_ENUM_CLASS ConsoleFile
    {
        Read = 1,
        Write = 2,
    };

    EXPORT_ENUM_CLASS CharWidth
    {
        Unknown = 0,
        Half = 1,
        Full = 2,
    };

    EXPORT_ENUM_CLASS ConsoleColor
    {
        BLACK = 0,
        DARKBLUE = 1,
        DARKGREEN = 2,
        DARKCYAN = 3,
        DARKRED = 4,
        DARKMAGENTA = 5,
        DARKYELLOW = 6,
        GRAY = 7,
        DARKGRAY = 8,
        BLUE = 9,
        GREEN = 10,
        CYAN = 11,
        RED = 12,
        MAGENTA = 13,
        YELLOW = 14,
        WHITE = 15,
    };

    EXPORT_ENUM_CLASS MouseWheelDirection
    {
        None = 0,
        Up = 1,     //The mouse wheel turns away from the player
        Down = 2,   //The mouse wheel turns towards the player
    };

    inline ushort ConsoleColorToUshort(ConsoleColor foreColor, ConsoleColor backColor)
    {
        return (ushort)((ushort)foreColor | ((ushort)backColor << 4));
    }

    inline std::pair<ConsoleColor, ConsoleColor> UshortToConsoleColor(ushort u)
    {
        std::pair<ConsoleColor, ConsoleColor> result;
        result.first = (ConsoleColor)(u & 0x000F);
        result.second = (ConsoleColor)((u & 0x00F0) / 16);
        return result;
    }

    struct ConsoleColorPair
    {
    public:
        ConsoleColor foreColor;
        ConsoleColor backColor;

        ConsoleColorPair()
        {
            this->foreColor = ConsoleColor::GRAY;
            this->backColor = ConsoleColor::BLACK;
        }

        ConsoleColorPair(ConsoleColor foreColor)
        {
            this->foreColor = foreColor;
            this->backColor = ConsoleColor::BLACK;
        }

        ConsoleColorPair(ConsoleColor foreColor, ConsoleColor backColor)
        {
            this->foreColor = foreColor;
            this->backColor = backColor;
        }

        ConsoleColorPair(ushort u)
        {
            std::pair<ConsoleColor, ConsoleColor> cp = UshortToConsoleColor(u);
            this->foreColor = cp.first;
            this->backColor = cp.second;
        }

        ushort ToUshort()
        {
            return ConsoleColorToUshort(foreColor, backColor);
        }
    };

    EXPORT_STRUCT Color24
    {
    public:
        EXPORT_STRUCT_MEMBER byte r;
        EXPORT_STRUCT_MEMBER byte g;
        EXPORT_STRUCT_MEMBER byte b;

        Color24()
        {
            this->r = 0;
            this->g = 0;
            this->b = 0;
        }

        Color24(byte r, byte g, byte b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        bool operator ==(const Color24& other)
        {
            if (r == other.r && g == other.g && b == other.b)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator !=(const Color24& other)
        {
            if (r == other.r && g == other.g && b == other.b)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        ConsoleColor ToConsoleColor();
    };

    EXPORT_STRUCT ConsoleInputMode
    {
    public:
        EXPORT_STRUCT_MEMBER bool _ENABLE_PROCESSED_INPUT;               //default is TRUE(recommend to FALSE to avoid pressing CTRL+C to close the console program)
        EXPORT_STRUCT_MEMBER bool _ENABLE_LINE_INPUT;                    //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_ECHO_INPUT;                    //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_WINDOW_INPUT;                  //default is FALSE(recommend to TRUE)
        EXPORT_STRUCT_MEMBER bool _ENABLE_MOUSE_INPUT;                   //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_INSERT_MODE;                   //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_QUICK_EDIT_MODE;               //default is TRUE(recommend to FALSE)
        EXPORT_STRUCT_MEMBER bool _ENABLE_EXTENDED_FLAGS;                //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_AUTO_POSITION;                 //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_VIRTUAL_TERMINAL_INPUT;        //default is FALSE

        ConsoleInputMode()
        {
            this->_ENABLE_PROCESSED_INPUT = true;
            this->_ENABLE_LINE_INPUT = true;
            this->_ENABLE_ECHO_INPUT = true;
            this->_ENABLE_WINDOW_INPUT = false;
            this->_ENABLE_MOUSE_INPUT = true;
            this->_ENABLE_INSERT_MODE = true;
            this->_ENABLE_QUICK_EDIT_MODE = true;
            this->_ENABLE_EXTENDED_FLAGS = true;
            this->_ENABLE_AUTO_POSITION = true;
            this->_ENABLE_VIRTUAL_TERMINAL_INPUT = false;
        }

        static ConsoleInputMode Standard()
        {
            ConsoleInputMode cim;
            cim._ENABLE_PROCESSED_INPUT = false;
            cim._ENABLE_WINDOW_INPUT = true;
            cim._ENABLE_QUICK_EDIT_MODE = false;
            return cim;
        }
    };

    EXPORT_STRUCT ConsoleOutputMode
    {
    public:
        EXPORT_STRUCT_MEMBER bool _ENABLE_PROCESSED_OUTPUT;              //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_WRAP_AT_EOL_OUTPUT;            //default is TRUE
        EXPORT_STRUCT_MEMBER bool _ENABLE_VIRTUAL_TERMINAL_PROCESSING;   //default is FALSE(recommend to TRUE, If want to use the VT100 sequence)
        EXPORT_STRUCT_MEMBER bool _DISABLE_NEWLINE_AUTO_RETURN;          //default is FALSE
        EXPORT_STRUCT_MEMBER bool _ENABLE_LVB_GRID_WORLDWIDE;            //default is FALSE

        ConsoleOutputMode()
        {
            this->_ENABLE_PROCESSED_OUTPUT = true;
            this->_ENABLE_WRAP_AT_EOL_OUTPUT = true;
            this->_ENABLE_VIRTUAL_TERMINAL_PROCESSING = false;
            this->_DISABLE_NEWLINE_AUTO_RETURN = false;
            this->_ENABLE_LVB_GRID_WORLDWIDE = false;
        }

        static ConsoleOutputMode Standard()
        {
            ConsoleOutputMode com;
            com._ENABLE_VIRTUAL_TERMINAL_PROCESSING = true;
            return com;
        }
    };

    EXPORT_STRUCT ConsoleMode
    {
    public:
        EXPORT_STRUCT_MEMBER ConsoleInputMode inputMode;
        EXPORT_STRUCT_MEMBER ConsoleOutputMode outputMode;

        ConsoleMode()
        {
            this->inputMode = ConsoleInputMode();
            this->outputMode = ConsoleOutputMode();
        }

        ConsoleMode(ConsoleInputMode inputMode, ConsoleOutputMode outputMode)
        {
            this->inputMode = inputMode;
            this->outputMode = outputMode;
        }

        static ConsoleMode Standard()
        {
            auto inputMode = ConsoleInputMode::Standard();
            auto outputMode = ConsoleOutputMode::Standard();
            ConsoleMode consoleMode(inputMode, outputMode);
            return consoleMode;
        }
    };

    EXPORT_STRUCT ConsoleSession
    {
    public:
        EXPORT_STRUCT_MEMBER HWND consoleWindow;
        EXPORT_STRUCT_MEMBER HANDLE consoleInput;
        EXPORT_STRUCT_MEMBER HANDLE consoleOutput;

    public:
        ConsoleSession()
        {
            this->consoleWindow = nullptr;
            this->consoleInput = nullptr;
            this->consoleOutput = nullptr;
        }

        ConsoleSession(HWND consoleWindow, HANDLE consoleInput, HANDLE consoleOutput)
        {
            this->consoleWindow = consoleWindow;
            this->consoleInput = consoleInput;
            this->consoleOutput = consoleOutput;
        }
    };

    EXPORT_STRUCT ConsoleFont
    {
    public:
        EXPORT_STRUCT_MEMBER DWORD FontIndex;
        EXPORT_STRUCT_MEMBER COORD FontSize;
        EXPORT_STRUCT_MEMBER uint FontFamily;
        EXPORT_STRUCT_MEMBER uint FontWeight; //Over 400 is bold
        EXPORT_STRUCT_MEMBER wchar FaceName[LF_FACESIZE];

        ConsoleFont()
        {
            this->FontIndex = 0;
            this->FontSize = { 0, 0 };
            this->FontFamily = 0;
            this->FontWeight = 0; //Over 400 is bold
            SetFontName(L"");
        }

        bool GetFontBold()
        {
            return FontWeight > FW_NORMAL;
        }

        int GetFontWidth()
        {
            return FontSize.X;
        }

        int GetFontHeight()
        {
            return FontSize.Y;
        }

        std::wstring GetFontName()
        {
            return std::wstring(FaceName);
        }

        void SetFontBold(bool bold)
        {
            if (bold)
            {
                FontWeight = FW_BOLD;
            }
            else
            {
                FontWeight = FW_NORMAL;
            }
        }

        void SetFontWidth(int width)
        {
            FontSize.X = width;
        }

        void SetFontHeight(int height)
        {
            FontSize.Y = height;
        }

        void SetFontName(const std::wstring& faceName)
        {
            size_t fontNameLength = faceName.size();
            ::wcscpy_s(this->FaceName, fontNameLength + 1, faceName.c_str());
        }
    };

    EXPORT_STRUCT ConsoleMouseInputRecord
    {
    public:
        EXPORT_STRUCT_MEMBER bool moved;                         //Whether the mouse is moving
        EXPORT_STRUCT_MEMBER bool doubleClick;                   //Double click
        EXPORT_STRUCT_MEMBER COORD position;                     //The unit is cell
        EXPORT_STRUCT_MEMBER MouseWheelDirection mouseWheelDir;  //Mouse wheel direction

        ConsoleMouseInputRecord()
        {
            this->moved = false;
            this->doubleClick = false;
            this->position = { 0 };
            this->mouseWheelDir = MouseWheelDirection::None;
        }
    };

    EXPORT_STRUCT ConsoleKeyboardInputRecord
    {
    public:
        EXPORT_STRUCT_MEMBER wchar KeyChar;
        EXPORT_STRUCT_MEMBER ushort VirualKey;
        EXPORT_STRUCT_MEMBER bool _RIGHT_ALT_PRESSED;        // the right alt key is pressed.
        EXPORT_STRUCT_MEMBER bool _LEFT_ALT_PRESSED;         // the left alt key is pressed.
        EXPORT_STRUCT_MEMBER bool _RIGHT_CTRL_PRESSED;       // the right ctrl key is pressed.
        EXPORT_STRUCT_MEMBER bool _LEFT_CTRL_PRESSED;        // the left ctrl key is pressed.
        EXPORT_STRUCT_MEMBER bool _SHIFT_PRESSED;            // the shift key is pressed.
        EXPORT_STRUCT_MEMBER bool _NUMLOCK_ON;               // the numlock light is on.
        EXPORT_STRUCT_MEMBER bool _SCROLLLOCK_ON;            // the scrolllock light is on.
        EXPORT_STRUCT_MEMBER bool _CAPSLOCK_ON;              // the capslock light is on.
        EXPORT_STRUCT_MEMBER bool _ENHANCED_KEY;             // the key is enhanced.

        ConsoleKeyboardInputRecord()
        {
            this->KeyChar = 0;
            this->VirualKey = 0;
            this->_RIGHT_ALT_PRESSED = false;
            this->_LEFT_ALT_PRESSED = false;
            this->_RIGHT_CTRL_PRESSED = false;
            this->_LEFT_CTRL_PRESSED = false;
            this->_SHIFT_PRESSED = false;
            this->_NUMLOCK_ON = false;
            this->_SCROLLLOCK_ON = false;
            this->_CAPSLOCK_ON = false;
            this->_ENHANCED_KEY = false;
        }
    };

    EXPORT_DELEGATE typedef void (*OnReadConsoleMouseInputRecord)(ConsoleMouseInputRecord mouseInput);

    EXPORT_DELEGATE typedef void (*OnReadConsoleKeyboardInputRecord)(ConsoleKeyboardInputRecord keyboardInput);

    EXPORT_DELEGATE typedef void (*OnConsoleOutputBufferChanged)(COORD newSize);

    extern const int MAX_INPUT_CHAR_COUNT;

    //Initialize the console inside, don't worry.
    EXPORT_FUNC MinInitConsoleSession(_OUT_ ConsoleSession* cons);

    //return true means successful enable VT!
    EXPORT_FUNC MinEnableConsoleVT(HANDLE consoleInput, HANDLE consoleOutput);

    EXPORT_FUNC MinGetConsolePalette(HANDLE consoleOutput, DWORD index, _OUT_ Color24* color);

    //Notice:After calling this API, the screen buffer will change!
    EXPORT_FUNC MinSetConsolePalette(HANDLE consoleOutput, DWORD index, Color24 color);

    EXPORT_FUNC_EX(ConsoleInputMode) MinGetConsoleInputMode(HANDLE consoleInput);

    EXPORT_FUNC_EX(bool) MinSetConsoleInputMode(HANDLE consoleInput, ConsoleInputMode mode);

    EXPORT_FUNC_EX(ConsoleOutputMode) MinGetConsoleOutputMode(HANDLE consoleOutput);

    EXPORT_FUNC_EX(bool) MinSetConsoleOutputMode(HANDLE consoleOutput, ConsoleOutputMode mode);

    EXPORT_FUNC MinGetConsoleMode(HANDLE consoleInput, HANDLE consoleOutput, _OUT_ ConsoleMode* consoleMode);

    EXPORT_FUNC MinSetConsoleMode(HANDLE consoleInput, HANDLE consoleOutput, ConsoleMode consoleMode);

    EXPORT_FUNC MinGetConsoleFont(HANDLE consoleOutput, _OUT_ ConsoleFont* consoleFont);

    EXPORT_FUNC MinSetConsoleFont(HANDLE consoleOutput, ConsoleFont consoleFont);

    EXPORT_FUNC MinGetConsoleWindowSize(HANDLE consoleOutput, _OUT_ POINT* size);

    EXPORT_FUNC MinSetConsoleWindowSize(HANDLE consoleOutput, POINT size);

    EXPORT_FUNC MinGetConsoleBufferSize(HANDLE consoleOutput, _OUT_ POINT* size);

    EXPORT_FUNC MinSetConsoleBufferSize(HANDLE consoleOutput, POINT size);

    EXPORT_FUNC MinGetConsoleForeColor(HANDLE consoleOutput, _OUT_ ConsoleColor* foreColor);

    EXPORT_FUNC MinGetConsoleBackColor(HANDLE consoleOutput, _OUT_ ConsoleColor* backColor);

    EXPORT_FUNC MinSetConsoleForeColor(HANDLE consoleOutput, ConsoleColor foreColor);

    EXPORT_FUNC MinSetConsoleBackColor(HANDLE consoleOutput, ConsoleColor backColor);

    EXPORT_FUNC MinGetConsoleCursorPos(HANDLE consoleOutput, _OUT_ COORD* pos);

    EXPORT_FUNC MinSetConsoleCursorPos(HANDLE consoleOutput, COORD pos);

    //size of buffer should be : charCount * sizeof(wchar)
    EXPORT_FUNC MinReadConsole(HANDLE consoleInput, wchar* buffer, int charCount);

    //See:https://docs.microsoft.com/en-us/windows/console/readconsoleinput
    //Call this function in update.
    //IMPORTANT:Please turn on EnableWindowInput and turn off EnableQuickEditMode
    EXPORT_FUNC MinReadConsoleInput(HANDLE consoleInput, OnReadConsoleMouseInputRecord callback1, OnReadConsoleKeyboardInputRecord callback2, OnConsoleOutputBufferChanged callback3);

    //See:https://docs.microsoft.com/en-us/windows/console/readconsoleinput
    //Call this function in update.
    //IMPORTANT:Please turn on EnableWindowInput and turn off EnableQuickEditMode
    EXPORT_FUNC_EX(bool) MinReadConsoleOneInput(HANDLE consoleInput, OnReadConsoleMouseInputRecord callback1, OnReadConsoleKeyboardInputRecord callback2, OnConsoleOutputBufferChanged callback3);

    EXPORT_FUNC MinWriteConsole(HANDLE consoleOutput, const wchar* buffer);

    //You can use something like this:arr[i].Attributes |= COMMON_LVB_UNDERSCORE
    EXPORT_FUNC MinWriteConsoleOutput(HANDLE consoleOutput, const CHAR_INFO* charInfos, short x, short y, short width, short height);

    EXPORT_FUNC MinWriteConsoleOutputAttribute(HANDLE consoleOutput, const ushort* att, int attCount, COORD pos);

    //This function is useful for mixed renderer.
    EXPORT_FUNC MinWriteConsoleOutputCharacter(HANDLE consoleOutput, const wchar* str, int charCount, COORD pos);

    //The created HANDLE can be recycled using the MinCloseConsoleScreenBuffer function
    EXPORT_FUNC MinCreateConsoleScreenBuffer(HANDLE* consoleOutput);

    EXPORT_FUNC MinSetConsoleActiveScreenBuffer(HANDLE consoleOutput);

    EXPORT_FUNC MinCloseConsoleScreenBuffer(HANDLE consoleOutput);

    //SEE:https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew#consoles
    EXPORT_FUNC MinCreateFile(ConsoleFile fileMode, _OUT_ HANDLE* handle);

    //IMPORTANT!!!  File I/O function does not support Unicode.
    EXPORT_FUNC MinWriteFile(HANDLE handle, const char* str);

    //IMPORTANT!!!  File I/O function does not support Unicode.
    EXPORT_FUNC MinReadFile(HANDLE handle, char* buffer, DWORD bufferLen);

    EXPORT_FUNC MinGetCharWidth(HWND consoleWindow, HANDLE consoleOutput, wchar c, _OUT_ CharWidth* cw);

    EXPORT_FUNC MinGetStringWidth(HWND consoleWindow, HANDLE consoleOutput, const wchar* str, int* width);

    //sizeof(title) should be MAX_PATH
    EXPORT_FUNC MinGetTitle(wchar* titleBuffer, int sizeOfBuffer);

    EXPORT_FUNC MinSetTitle(const wchar* titleBuffer);

    EXPORT_FUNC MinGetConsoleCursorVisible(HANDLE consoleOutput, _OUT_ bool* visible);

    EXPORT_FUNC MinSetConsoleCursorVisible(HANDLE consoleOutput, bool visible);

    //See:https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
    EXPORT_FUNC MinClear(HANDLE consoleOutput);

    //nullptr, true to disable ctrl + c signal
    EXPORT_FUNC MinSetConsoleCtrlHandler(PHANDLER_ROUTINE handler, bool add);

    //Get type of Console Emulator on Windows.
    EXPORT_FUNC MinGetConsoleType(_OUT_ ConsoleType* type);

    class Console
    {
    public:
        static bool forceVT;
        static Singleton<Console> Global;

    public:
        static ConsoleSession InitConsoleSession();

        static HANDLE CreateConsoleScreenBuffer();

        static bool SetConsoleActiveScreenBuffer(HANDLE consoleOutput);

        static bool CloseConsoleScreenBuffer(HANDLE consoleOutput);

        static HANDLE CreateFileW(ConsoleFile filemode);

        static std::wstring GetTitle();

        static bool SetTitle(const std::wstring& title);

        static ConsoleType GetConsoleType();

        //Console class only functions:

        inline static void SetConsoleHistory(uint historyBufferSize, bool noDuplicate)
        {
            CONSOLE_HISTORY_INFO chi;
            chi.cbSize = sizeof(CONSOLE_HISTORY_INFO);
            chi.HistoryBufferSize = historyBufferSize;
            chi.NumberOfHistoryBuffers = 1; //default to 1
            chi.dwFlags = noDuplicate;
            SetConsoleHistoryInfo(&chi);
        }

        //This function will reinitialize Global
        inline static void RestartConsole()
        {
            ::FreeConsole();
            ::AllocConsole();

            ConsoleSession cons;
            MinInitConsoleSession(&cons);

            Console::Global.GetInstance() = Console(cons);
        }

    public:
        ConsoleSession cons;
        bool supportVT;

        Console();

        Console(ConsoleSession cons);

        Console(HWND consoleWindow, HANDLE consoleInput, HANDLE consoleOutput);

        //Basic Console functions:

        Color24 GetConsolePalette(DWORD index);

        //Notice:After calling this API, the screen buffer will change!
        bool SetConsolePalette(DWORD index, const Color24& color);

        ConsoleInputMode GetConsoleInputMode();

        bool SetConsoleInputMode(ConsoleInputMode mode);

        ConsoleOutputMode GetConsoleOutputMode();

        bool SetConsoleOutputMode(ConsoleOutputMode mode);

        ConsoleMode GetConsoleMode();

        bool SetConsoleMode(const ConsoleMode& consoleMode);

        ConsoleFont GetConsoleFont();

        bool SetConsoleFont(const ConsoleFont& consoleFont);

        POINT GetConsoleWindowSize();

        bool SetConsoleWindowSize(POINT size);

        POINT GetConsoleBufferSize();

        bool SetConsoleBufferSize(POINT size);

        ConsoleColor GetConsoleForeColor();

        ConsoleColor GetConsoleBackColor();

        bool SetConsoleForeColor(ConsoleColor foreColor);

        bool SetConsoleBackColor(ConsoleColor backColor);

        COORD GetConsoleCursorPos();

        bool SetConsoleCursorPos(COORD pos);

        bool GetConsoleCursorVisible();

        bool SetConsoleCursorVisible(bool visible);

        //Console standard IO functions:

        std::wstring ReadConsoleW();

        //See:https://docs.microsoft.com/en-us/windows/console/readconsoleinput
        //Call this function in update.
        //IMPORTANT:Please turn on EnableWindowInput and turn off EnableQuickEditMode
        bool ReadConsoleInputW(OnReadConsoleMouseInputRecord callback1, OnReadConsoleKeyboardInputRecord callback2, OnConsoleOutputBufferChanged callback3);

        //See:https://docs.microsoft.com/en-us/windows/console/readconsoleinput
        //Call this function in update.
        //IMPORTANT:Please turn on EnableWindowInput and turn off EnableQuickEditMode
        bool ReadConsoleOneInput(OnReadConsoleMouseInputRecord callback1, OnReadConsoleKeyboardInputRecord callback2, OnConsoleOutputBufferChanged callback3);

        bool WriteConsoleW(const std::wstring& msg);

        //You can use something like this:arr[i].Attributes |= COMMON_LVB_UNDERSCORE
        bool WriteConsoleOutputW(const CHAR_INFO* charInfos, short x, short y, short width, short height);

        bool WriteConsoleOutputAttribute(const ushort* att, int attCount, COORD pos);

        bool WriteConsoleOutputCharacterW(const std::wstring& str, COORD pos);

        //Console file IO functions:

        bool WriteFile(std::string str);

        std::string ReadFile();

        //Expand Console API:

        //The return value does not contain \r\n
        int Read();

        //The return value does not contain \r\n
        std::wstring ReadLine();

        bool Write(const std::wstring& msg);

        bool Write(const std::wstring& msg, ConsoleColor foreColor);

        bool Write(const std::wstring& msg, ConsoleColor foreColor, ConsoleColor backColor);

        bool WriteLine();

        bool WriteLine(const std::wstring& msg);

        bool WriteLine(const std::wstring& msg, ConsoleColor foreColor);

        bool WriteLine(const std::wstring& msg, ConsoleColor foreColor, ConsoleColor backColor);

        //See:https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
        bool Clear();

        CharWidth GetWcharWidth(wchar c);

        int GetWstringWidth(const std::wstring& str);

        bool GetTreatControlCAsInput();

        void SetTreatControlCAsInput(bool yes);

        bool SetConsoleCtrlHandler(PHANDLER_ROUTINE handler);

        bool EnableConsoleVT();

        //ANSI Escape Sequence (Virtual Terminal Sequences)

        bool Write(const std::wstring& msg, Color24 foreColor);

        bool Write(const std::wstring& msg, Color24 foreColor, Color24 backColor);

        bool Write(const std::wstring& msg, Color24 foreColor, Color24 backColor, bool under_score);

        bool WriteLine(const std::wstring& msg, Color24 foreColor);

        bool WriteLine(const std::wstring& msg, Color24 foreColor, Color24 backColor);

        bool WriteLine(const std::wstring& msg, Color24 foreColor, Color24 backColor, bool under_score);
    };
}