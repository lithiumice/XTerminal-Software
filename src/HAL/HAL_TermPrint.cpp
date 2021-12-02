//
// Created by lithiumice on 2021/11/29.
//
#include "HAL.h"

namespace HAL{
    std::string globalTermText;
    uint8_t term_text_update_flag=0;
}

void HAL::TerminalPrint(char* text)
{
//    va_list arg_ptr;
//    va_start(arg_ptr, count);
//    printf(text,...);

    printf("%s",text);
    globalTermText = globalTermText + text;
    term_text_update_flag=1;
}

// #define TerminalPrintf(...) printf
void HAL::TerminalPrintln(char* text)
{
    TerminalPrint((char*)"\r\n");
    TerminalPrint(text);
}
void HAL::TerminalPrintln(String text)
{
    TerminalPrintln((char*)text.c_str());
}