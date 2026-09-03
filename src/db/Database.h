#pragma once

#include "Block.h"
#include "Master.h"
#include "Site.h"
#include "Tech.h"
#include <string>
#include <unordered_map>
using namespace std;
struct Database
{
    Tech tech;
    //用名字快速查找对象的表例如：
    //"NAND2_X1" → Master 对象
    //"INV_X1"   → Master 对象
    //"DFF_X1"   → Master 对象
    unordered_map<string, Site> sites;
    unordered_map<string, Master> masters;
    

    Block block;
    void addSite(const Site& site){
        sites[site.name] = site;
    }
    //把一个 Master 放进 masters 表里
    //key 是 master.name
    //value 是 master 本身
    void addMaster(const Master& master){
        masters[master.name] = master;
    }

    Site* findSite(const string& name){
        auto it = sites.find(name);
        if(it == sites.end()){
            return nullptr;
        }
        return &it->second;
    }

    Master* findMaster(const string& name){
        auto it = masters.find(name);
        if(it == masters.end()){
            return nullptr;
        }
        return &it->second;
    }

    int siteCount() const{
        return static_cast<int>(sites.size());
    }

    int masterCount() const{
        return static_cast<int>(masters.size());
    }
};