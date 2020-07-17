#include <iostream>
#include <string>
#include "owoify.h"

int main()
{
    std::wstring str{ Owoifier::owoify(std::string{"This is the string to owo! Kinda cute isn't it?"}, Owoifier::OwoifyLevel::Uvu) };

    return 0;
}