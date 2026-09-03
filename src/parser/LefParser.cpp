#include "parser/LefParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//change the micron into DBU be like
// 0.19 micron, dbu_per_micron = 1000
// result = 190 DBU
//这个namespace指这个功能只在这个cpp文件使用
namespace
{
int micronToDbu(double value, int dbu_per_micron)
{
    return static_cast<int>(value * dbu_per_micron + 0.5);
}
}

bool LefParser::read(const std::string& file_path, Database& db)
{
    std::ifstream input(file_path);

    if(!input.is_open()){
        std::cerr << "Error: cannot open LEF file: " << file_path << "\n";
        return false;
    }

    line_count_ = 0;

    int dbu_per_micron = 2000;
    //inside_site表示当前是否在site字段中
    bool inside_site = false;
    Site current_site;
    //检测是否是在macro以区分site
    bool inside_macro = false;
    Master current_master;

    std::string line;
    while (std::getline(input, line)){
        ++line_count_;
        //把一整行的字符串拆成一个个词
        //比如一行是：SIZE 0.19 BY 1.4 ;
        // 那么ss >> word;就读到SIZE，ss >> width_micron >> by_word >> height_micron;就会读到width_micron = 0.19，by_word = BY，height_micron = 1.4
        std::stringstream ss(line);
        std::string word;
        //取第一个词用来分辨读取的内容是什么，空格和tab会在这一步被跳过，直接得到字符串
        ss >> word;

        if(word.empty()){
            continue;
        }
        //add lef micron
        if(word == "DATABASE"){
            std::string unit;
            int value = 0;
            ss >> unit >> value;

            if(unit =="MICRONS" && value > 0){
                dbu_per_micron = value;
                db.tech.dbu_per_micron = value;
            }

            continue;
        }
        //add site name and make sure inside the site definite
        if(!inside_macro && word == "SITE"){
            std::string site_name;
            ss >> site_name;
            //清空之前的site存储
            current_site = Site{};
            current_site.name = site_name;
            inside_site = true;

            continue;
        }

        if(inside_site && word == "SIZE"){
            double width_micron = 0.0;
            double height_micron = 0.0;
            std::string by_world;
            ss >> width_micron >>  by_world >> height_micron;

            if(by_world == "BY"){
                current_site.width = micronToDbu(width_micron, dbu_per_micron);
                current_site.height = micronToDbu(height_micron, dbu_per_micron);
            }

            continue;
        }

        if(inside_site && word == "END"){
            std::string end_name;
            ss >> end_name;

            if(end_name == current_site.name) {
                if(current_site.isValid()){
                    db.addSite(current_site);
                } else{
                    std::cerr << "Warning: skip invalid SITE:" << current_site.name << "\n";
                }
                inside_site = false;
                current_site = Site{};
            }
            continue;
        }
        //add the master
        if(word == "MACRO"){
            std::string macro_name;
            ss >> macro_name;

            current_master = Master{};
            current_master.name = macro_name;
            inside_macro = true;

            continue;
        }

        if(inside_macro && word == "CLASS"){
            std::string class_name;
            ss >> class_name;
            if(class_name == "CORE"){
                current_master.type = MasterType::Core;
            } else if(class_name == "BLOCK"){
                current_master.type = MasterType::Block;
            } else{
                current_master.type = MasterType::Unknown;
            }
            continue;
        }

        if(inside_macro && word == "SIZE"){
            double width_micron = 0.0;
            double height_micron = 0.0;
            std::string by_word;

            ss >> width_micron >> by_word >> height_micron;

            if(by_word == "BY"){
                current_master.width = micronToDbu(width_micron, dbu_per_micron);
                current_master.height = micronToDbu(height_micron, dbu_per_micron);
            }
            continue;
        }

        if(inside_macro && word == "END"){
            std::string end_name;
            ss >> end_name;

            if(end_name == current_master.name){
                if(current_master.isValid()){
                    db.addMaster(current_master);
                } else{
                    std::cerr << "Warning: skip invalid MACRO: " << current_master.name << "\n";
                }
                inside_macro = false;
                current_master = Master{};
            }
            continue;
        }
    }
    return true;
}

int LefParser::lineCount() const
{
    return line_count_;
}