#pragma once
#include "Geometry.h"
#include <string>

enum class  MasterType
{
    Core,
    Block,
    Unknown
};

struct Master
{
    std::string name;
    Dbu width = 0;
    Dbu height = 0;
    MasterType type = MasterType::Unknown;

    bool isValid() const{
        return !name.empty() && width > 0;
    }

    Dbu area() const{
        return width * height;
    }

    bool isCore() const{
        return type == MasterType::Core;
    }

    bool isBlock() const{
        return type == MasterType::Block;
    }
};
