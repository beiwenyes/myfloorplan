#pragma once

struct Tech
{
    int dbu_per_micron = 0;
    
    bool hasValidUnits() const{
        return dbu_per_micron > 0;
    }
};