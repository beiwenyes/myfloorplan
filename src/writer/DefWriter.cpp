#include "writer/DefWriter.h"

#include <fstream>
#include <iostream>

bool DefWriter::write(const std::string& file_path, const Database& db) const
{
    std::ofstream output(file_path);

    if(!output.is_open()){
        std::cerr << "Error: cannot open output DEF file: " << file_path << "\n";
        return false;
    }

    output << "VERSION 5.8 ;\n";
    output << "DIVIDERCHAR \"/\" ;\n";
    output << "BUSBITCHARS \"[]\" ;\n";

    output << "DESIGN " << db.block.name << " ;\n";

    output << "UNITS DISTANCE MICRONS " << db.tech.dbu_per_micron << " ;\n";

    output << "DIEAREA ( "
           << db.block.die_area.lx << " "
           << db.block.die_area.ly << " ) ( "
           << db.block.die_area.ux << " "
           << db.block.die_area.uy << " ) ;\n";
    
    output << "\n";

    for (const Row& row : db.block.rows){
        output << "ROW " << row.name << " "
                << row.site->name << " "
                << row.origin.x << " "
                << row.origin.y << " "
                << row.orient << " "
                << "DO " << row.site_count
                << " BY 1 "
                << "STEP " << row.site_spacing
                << " 0 ;\n";
    }

    output << "\n";
    
    output << "COMPONENTS " << db.block.instanceCount() << " ;\n";
    
    for(const Instance& instance : db.block.instances){
        output << "- " << instance.name << " "
                << instance.master->name
                << " + UNPLACED ;\n";
    }

    output << "END COMPONENTS\n";
    
    output << "\n";
    output << "END DESIGN\n";

    return true;
}