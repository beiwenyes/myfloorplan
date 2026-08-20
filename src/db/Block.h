#pragma once

#include "Geometry.h"
#include "Instance.h"
#include "Row.h"

#include <string>
#include <vector>

using namespace std;

struct Block
{
    string name;

    Rect die_area;
    Rect core_area;
    //使用动态数组保存多个instance
    vector<Instance> instances;
    vector<Row> rows;
    bool hasValidDieArea() const{
        return die_area.isValid();
    }

    bool hasValidCoreArea() const{
        return core_area.isValid();
    }
    //添加instance
    //Instance&表示直接使用传进来的instance
    void  addInstance(const Instance& instance){
        instances.push_back(instance);
    }

    int instanceCount() const{
        //类型转换为int
        return static_cast<int>(instances.size());
    }

    Dbu totalInstanceArea() const{
        Dbu total_area = 0;
        for(const Instance& instance : instances){
            total_area += instance.area();
        }
        return total_area;
    }

    void addRow(const Row& row){
        rows.push_back(row);
    }

    int rowCount() const{
        return static_cast<int>(rows.size());
    }
};