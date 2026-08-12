#pragma once

using Dbu = int;

struct Point
{
    Dbu x = 0;
    Dbu y = 0;
};

struct Rect
{
    Dbu lx = 0;
    Dbu ly = 0;
    Dbu ux = 0;
    Dbu uy = 0;

    Dbu width() const{
        return ux - lx;
    }

    Dbu height() const{
        return uy - ly;
    }

    Dbu area() const{
        return width() * height();
    }

    bool isValid() const{
        return ux > lx && uy > ly;
    }

    bool constains(const Point& p) const{
        return p.x >= lx && p.x <= ux && 
                p.y >= ly && p.y <= uy;
    }
    
};
