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
#include "parser/DefParser.h"
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
    if(!lef_parser.read(options.lef_file, db)){
        return 1;
    }
    //create def parser
    DefParser def_parser;
    if(!def_parser.read(options.def_file, db)){
        return 1;
    }
    //测试parser的内容
    std::cout << "Parse the LEF:\n";
    std::cout << "file = " << options.lef_file << "\n";
    std::cout << "line count = " << lef_parser.lineCount() << "\n";
    std::cout << "site count = " << db.siteCount() << "\n";
    std::cout << "master count = " << db.masterCount() << "\n";
    std::cout << "LEF dbu per micron = " << db.tech.dbu_per_micron << "\n";
    std::cout << "status = success\n";
    //test the master
    Master* dff_master = db.findMaster("DFFPOSX1");
    if(dff_master != nullptr){
        std::cout << "Found DFFPOSX1 master:\n";
        std::cout << "  name = " <<  dff_master->name << "\n";
        std::cout << "  width = " << dff_master->width<< "\n";
        std::cout << "  height = " << dff_master->height << "\n";
        std::cout << "  area = " << dff_master->area() << "\n";
        std::cout << "  valid = " <<  dff_master->isValid() << "\n";
        std::cout << "  isCore = " << dff_master->isCore() << "\n";
    } else {
        std::cout << "DFFPOSX1 master not found\n";
    }
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
    std::cout << "====================\n";
    std::cout << "Parse the DEF:\n";
    std::cout << "file = " << options.def_file << "\n";
    std::cout << "line count = " << def_parser.lineCount() << "\n";
    std::cout << "dbu per micron = " << def_parser.dbuPerMicron() << "\n";
    std::cout << "design name = " << db.block.name << "\n";
    std::cout << "die area = (" << db.block.die_area.lx << " " << db.block.die_area.ly << ") (" 
              << db.block.die_area.ux << " " << db.block.die_area.uy << ")\n";
    std::cout << "die area valid = " << db.block.hasValidDieArea() << "\n";
    std::cout << "status = success\n";
    std::cout << "====================\n";
    if(db.sites.empty()){
        std::cerr << "Error:  no site found in LEF\n";
        return 1;
    }
    std::string site_name = db.sites.begin()->first;

    std::cout << "Use site for row generation:\n";
    std::cout << "  site name = " << site_name << "\n";
    const int core_margin = 10000;

    db.block.core_area.lx = db.block.die_area.lx + core_margin;
    db.block.core_area.ly = db.block.die_area.ly + core_margin;
    db.block.core_area.ux = db.block.die_area.ux - core_margin;
    db.block.core_area.uy = db.block.die_area.uy - core_margin;

    std::cout << "Temporary core area:\n";
    std::cout << "  core area = ("
            << db.block.core_area.lx << " "
            << db.block.core_area.ly << ") ("
            << db.block.core_area.ux << " "
            << db.block.core_area.uy << ")\n";
    std::cout << "  valid = " << db.block.hasValidCoreArea() << "\n";
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