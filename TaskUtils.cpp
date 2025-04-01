#include "TaskUtils.h"
#include <cmath>
#include <iostream>

namespace win {
#include <windows.h>
}
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

int TaskUtils::xDir = 0;
bool TaskUtils::oneDown = false;
bool TaskUtils::goDown = false;
bool TaskUtils::rotate = false;

int TaskUtils::RandomInt(int range) {
    int num;
    if (win::BCryptGenRandom(NULL, (win::PUCHAR)&num, sizeof(num), BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) {
        return RandomInt(range);
    }
    int temp = num % range;
    if (temp < 0)
        return RandomInt(range);
    return temp;
}

char TaskUtils::KeyPressed() {
    for (size_t i = 0; i < 256; i++) {
        if (win::GetAsyncKeyState(i) & 0x8000) {
            return static_cast<char>(i);
        }
    }
    return '!';
}

void TaskUtils::Direction()
{
    switch (KeyPressed())
    {
    case 'W': Reset(); rotate = true; break;
    case 'S': Reset(); oneDown = true; break;
    case 'A': Reset(); xDir = -1;  break;
    case 'D': Reset(); xDir = 1;  break;
    case ' ': Reset(); goDown = true; break;
    default: break;
    }
}

void TaskUtils::Reset()
{
    xDir = 0;
    oneDown = false;
    goDown = false;
    rotate = false;
}