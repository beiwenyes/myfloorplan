#pragma once
#include "Geometry.h"
#include <string>

struct Site
{
    std::string name;
    Dbu width = 0;
    Dbu height = 0;

    bool isValid() const{
        return !name.empty() && width > 0 && height > 0;
    }
};