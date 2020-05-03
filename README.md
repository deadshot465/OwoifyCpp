# Owoify++
Turning your worst nightmare into a header only library.

[![Build Status](https://travis-ci.com/deadshot465/OwoifyCpp.svg?branch=master)](https://travis-ci.com/deadshot465/OwoifyCpp)

This is a C++ port of [mohan-cao's owoify-js](https://github.com/mohan-cao/owoify-js), which will help you turn any string into nonsensical babyspeak similar to LeafySweet's infamous Chrome extension. Just like owoify-js, three levels are available for owoifying your texts:

1. **owo (default):** The most vanilla one.
2. **uwu:** The moderate one.
3. **uvu:** Litewawwy unweadabwal.

Please refer to the original [owoify-js repository](https://github.com/mohan-cao/owoify-js) for more information.

## Install instructions

Since it's a header only library, just include it in your source file and you are good to go.

```c++
#include "owoify.h"
```

## Usage

```c++
#include <iostream>
#include "owoify.h"

// Owoifier accepts a std::string value, and an OwoifyLevel enum to specify the level.
// All owoifier's outputs use std::wstring instead of std::string.
// Without specifying owoify level. Default to owo.
std::wcout << Owoifier::owoify(str) << '\n';

// Specifying owoify level as uvu.
std::wcout << Owoifier::owoify(str, Owoifier::OwoifyLevel::Uvu) << '\n';

// Output (Expected):
// This is teh stwing two owo! Kinda cute isn't it?
// fwis is teh stwing two owowowo ＼(＾▽＾)／ Kinda cute isn't it?
```

## Known Issue

The PowerShell and the Command Prompt on Windows won't display the emojis in Uvu mode correctly even with `chcp 65001`. Zsh on macOS cannot display some special characters as well. However, from the debugger window, the input value is correctly owoified, so it might be a display issue instead of a code issue. If you want to use the command in your project, please do note this issue and handle it beforehand.