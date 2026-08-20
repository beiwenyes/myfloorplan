#pragma once

#include "Geometry.h"
#include "Site.h"

#include <string>

struct Row
{
    std::string name;

    Site* site = nullptr;

    Point origin;

    std::string orient = "N";

    int site_count = 0;
    Dbu site_spacing = 0;

    bool isValid() const{
        return !name.empty() &&
                site != nullptr &&
                site->isValid() &&
                site_count > 0 &&
                site_spacing > 0;
    }

    Dbu width() const{
        return site_count * site_spacing;
    }

    Dbu height() const{
        if(site == nullptr){
            return 0;
        }
        return site->height;
    }

    Rect bbox() const{
        Rect rect;
        rect.lx = origin.x;
        rect.ly = origin.y;
        rect.ux = origin.x + width();
        rect.uy = origin.y + height();
        return rect;
    }
};