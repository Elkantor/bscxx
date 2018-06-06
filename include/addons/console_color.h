#pragma once

#include <iostream>
#if defined(_WIN32)
    #include <windows.h>
#else
    #include <unistd.h>
#endif


inline std::ostream& blue(std::ostream &s)
{
    #if defined(_WIN32)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
                |FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    #else 
        s << "\033[0;36m";
    #endif
    return s;
}

inline std::ostream& red(std::ostream &s)
{
    #if defined(_WIN32)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(hStdout, 
                    FOREGROUND_RED|FOREGROUND_INTENSITY);
    #else 
        s << "\033[0;31m";
    #endif
    return s;
}

inline std::ostream& green(std::ostream &s)
{
    #if defined(_WIN32)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(hStdout, 
                FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    #else 
        s << "\033[1;32m";
    #endif
    return s;
}

inline std::ostream& yellow(std::ostream &s)
{
    #if defined(_WIN32)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(hStdout, 
            FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    #else 
        s << "\033[1;33m";
    #endif
    return s;
}

inline std::ostream& white(std::ostream &s)
{
    #if defined(_WIN32)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(hStdout, 
        FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
    #else 
        s << "\033[0m";
    #endif
    return s;
}

#if defined(_WIN32)
    struct color {
        color(WORD attribute):m_color(attribute){};
        WORD m_color;
    };

    template <typename _Elem, typename _Traits>
    std::basic_ostream<_Elem, _Traits>& 
        operator<<(std::basic_ostream<_Elem, _Traits>& i, color& c)
    {
        HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(hStdout,c.m_color);
        return i;
    }
#endif

// Copyleft Vincent Godin