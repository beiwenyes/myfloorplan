#pragma once

#include "db/Database.h"
#include <string>

class FloorplanEngine
{
public:
    explicit FloorplanEngine(Database& db);
    
    bool makeUniformRows(const std::string& site_name);
//引用外部的Database
private:
    Database& db_;
};