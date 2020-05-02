#include <iostream>
#include "owoify.h"

int main()
{
    std::string str = "This is the string to owo! Kinda cute isn't it?";

    std::wcout << Owoifier::owoify(str) << '\n';
    std::wcout << Owoifier::owoify(str, Owoifier::OwoifyLevel::Uvu) << '\n';

    return 0;
}