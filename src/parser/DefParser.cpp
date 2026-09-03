#include "parser/DefParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool DefParser::read(const std::string& file_path, Database& db)
{
    std::ifstream input(file_path);

    if(!input.is_open()){
        std::cerr << "Error: cannot open DEF file: " << file_path << "\n";
        return false; 
    }

    line_count_  = 0;
    dbu_per_micron_ = 0;

    std::string line;
    while(std::getline(input, line)){
        ++line_count_;

        std::stringstream ss(line);

        std::string word;
        ss >> word;
        if(word.empty()){
            continue;
        }

        if(word == "DESIGN"){
            std::string design_name;
            ss >> design_name;
            db.block.name = design_name;
            continue;
        }

        if(word == "UNITS"){
            std::string distance_word;
            std::string microns_word;
            int value = 0;
            ss >> distance_word >> microns_word >> value;
            if(distance_word == "DISTANCE" && microns_word == "MICRONS" && value > 0){
                dbu_per_micron_ = value;
                //确认lef和def的micron值一样
                if(db.tech.hasValidUnits() && db.tech.dbu_per_micron != dbu_per_micron_){
                    std::cerr << "Warning: LEF and DEF DBU units are different.\n";
                    std::cerr << "  LEF dbu per micron = " << db.tech.dbu_per_micron << "\n";
                    std::cerr << "  DEF dbu per micron = " << dbu_per_micron_ << "\n";
                }
            }
            continue;
        }

        if(word == "DIEAREA"){
            std::string left_paren_1;
            std::string right_paren_1;
            std::string left_paren_2;
            std::string right_paren_2;
            int lx = 0;
            int ly = 0;
            int ux = 0;
            int uy = 0;

            ss >> left_paren_1 >> lx >> ly >> right_paren_1
               >> left_paren_2 >> ux >> uy >> right_paren_2;
            
            if(left_paren_1 == "(" && right_paren_1 == ")" && left_paren_2 == "(" && right_paren_2 == ")" ){
                db.block.die_area.lx = lx;
                db.block.die_area.ly = ly;
                db.block.die_area.ux = ux;
                db.block.die_area.uy = uy;
            } else{
                std::cerr << "Warning: fail to parse DIEAREA at line" << line_count_ << "\n";
            }
            continue;
        }
    }

    return true;
}

int DefParser::lineCount() const
{
    return line_count_;
}

int DefParser::dbuPerMicron() const
{
    return dbu_per_micron_;
}