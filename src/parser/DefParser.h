#pragma once

#include "db/Database.h"

#include <string>

class DefParser
{
public:
    bool read(const std::string& file_path, Database& db);
    int lineCount() const;
    int dbuPerMicron() const;

private:
    int line_count_ = 0;
    int dbu_per_micron_ = 0;
};