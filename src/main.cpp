#include "Options.h"
#include "db/Geometry.h"
#include <iostream>
#include "db/Site.h"
#include "db/Master.h"
#include "db/Instance.h"
#include "db/Block.h"
#include "db/Database.h"
#include "db/Row.h"
#include "floorplan/FloorplanEngine.h"
#include "parser/LefParser.h"

int main(int argc, char* argv[])
{
    Options options;
    //存储用户输入的选项
    if(!parseOptions(argc, argv, options))
    {
        printUsage(argv[0]);
        return 1;
    }
    std::cout << "===============\n";
    std::cout << "input test\n";
    std::cout << "MiniFloorplan started\n";
    std::cout << "LEF File: " << options.lef_file << "\n";
    std::cout << "DEF File: " << options.def_file << "\n";
    std::cout << "Output File: " << options.output_file << "\n";
    std::cout << "Read File Done\n";
    std::cout << "===============\n";

    //create site and master
    Site test_site;
    test_site.name = "CORE_SITE";
    test_site.width = 190;
    test_site.height = 1400;

    Master test_master;
    test_master.name = "NAND2_X1";
    test_master.width = 1400;
    test_master.height = 2800;
    test_master.type = MasterType::Core;

    //create DataBase
    Database db;
    //create lef parser
    LefParser lef_parser;
    std::cout << "Parse the LEF:\n";
    std::cout << "file = " << options.lef_file << "\n";

    if(!lef_parser.read(options.lef_file, db)){
        return 1;
    }
    //测试parser的内容
    std::cout << "line count = " << lef_parser.lineCount() << "\n";
    std::cout << "site count = " << db.siteCount() << "\n";
    std::cout << "status = success\n";
    //测试parser的site是否正常保存
    std::cout << "Parsed sites:\n";
    //dbsite -> pair,pair具有map的内容，key = pair.first value = pair.second
    for (const auto& pair : db.sites){
        const Site& site = pair.second;

        std::cout << "  name    = " << site.name << "\n";
        std::cout << "  width   = " << site.width << "\n";
        std::cout << "  height  = " << site.height << "\n";
        std::cout << "  valid   = " << site.isValid() << "\n";
    }

    if(db.sites.empty()){
        std::cerr << "Error:  no site found in LEF\n";
        return 1;
    }
    std::string site_name = db.sites.begin()->first;

    std::cout << "Use site for row generation:\n";
    std::cout << "  site name = " << site_name << "\n";
    db.block.name = "test_design";

    db.block.die_area.lx = 0;
    db.block.die_area.ly = 0;
    db.block.die_area.ux = 10000;
    db.block.die_area.uy = 8000;

    db.block.core_area.lx = 1000;
    db.block.core_area.ly = 1000;
    db.block.core_area.ux = 9000;
    db.block.core_area.uy = 7000;
    FloorplanEngine engine(db);
    if(!engine.makeUniformRows(site_name)){
        return 1;
    }

    if (db.block.rows.empty()) {
        std::cerr << "Error: no rows generated\n";
        return 1;
    }
    const Row& first_row = db.block.rows[0];
    Rect row_bbox = first_row.bbox();
    std::cout << "block row count = " << db.block.rowCount() << "\n";
    std::cout << "Test row:\n";
    std::cout << "  name       = " << first_row.name << "\n";
    std::cout << "  site       = " << first_row.site->name << "\n";
    std::cout << "  origin     = ("
            << first_row.origin.x << " "
            << first_row.origin.y << ")\n";
    std::cout << "  site count = " << first_row.site_count << "\n";
    std::cout << "  spacing = " << first_row.site_spacing << "\n";
    std::cout << "  width = " << first_row.width() << "\n";
    std::cout << "  height = " << first_row.height() << "\n";
    std::cout << "  bbox       = ("
            << row_bbox.lx << " " << row_bbox.ly << ") ("
            << row_bbox.ux << " " << row_bbox.uy << ")\n";
    std::cout << "  valid      = " << first_row.isValid() << "\n";

    return 0;
}