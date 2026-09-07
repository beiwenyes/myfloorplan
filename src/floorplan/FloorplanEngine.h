#pragma once

#include "db/Database.h"
#include <string>

class FloorplanEngine
{
public:
    explicit FloorplanEngine(Database& db);
    
    bool initializeCoreAreaWithMargin(Dbu margin);
    
    bool makeUniformRows(const std::string& site_name);

    bool validateFloorplan() const;

    bool validateInstancesFitRows() const;
//引用外部的Database
private:
    Database& db_;
};