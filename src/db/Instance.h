#pragma once

#include "Geometry.h"
#include "Master.h"

#include <string>
//instance是否被摆放
//Unplaced = 还没有坐标
//Placed   = 已经有坐标，但是以后算法还可以移动它
//Fixed    = 已经固定，算法不应该移动它
enum class PlacementStatus
{
    Unplaced,
    Placed,
    Fixed
};

struct Instance
{
    std::string name;
    //声明一个指针指向master，之后只需要将使用的master的地址给到这个指针就可以使用了
    Master* master = nullptr;
    //声明起点
    Point origin;
    std::string orient = "N";

    PlacementStatus status = PlacementStatus::Unplaced;

    bool isValid() const{
        return !name.empty() && master != nullptr && master->isValid();
    }

    Dbu width() const{
        if (master == nullptr){
            return 0;
        }
        return master->width;
    }

    Dbu height() const{
        if (master == nullptr){
            return 0;
        }
        return master->height;
    }

    Dbu area() const{
        return width() * height();
    }
    //bounding box
    Rect bbox() const{
        Rect rect;
        rect.lx = origin.x;
        rect.ly = origin.y;
        rect.ux = origin.x + width();
        rect.uy = origin.y + height();
        return rect;
    }

    bool isPlaced() const{
        return status == PlacementStatus::Placed ||
                status == PlacementStatus::Fixed;
    }
};