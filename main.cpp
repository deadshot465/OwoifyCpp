#include <iostream>
#include <string>
#include "owoify.h"

int main(int argc, const char* argv[])
{
    std::wstring str{ Owoifier::owoify(std::string{argv[1]}, Owoifier::OwoifyLevel::Uvu) };

    return 0;
}