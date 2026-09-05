#pragma once

#include "db/Database.h"

#include <string>

class DefWriter
{
public:
    bool write(const std::string& file_path, const Database& db) const;
};