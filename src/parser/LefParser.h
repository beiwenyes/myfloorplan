#pragma once
#include "db/Database.h"
#include <string>

class LefParser
{
public:
    bool read(const std::string& file_path, Database& db);

    int lineCount() const;

private:
    int line_count_ = 0;
};