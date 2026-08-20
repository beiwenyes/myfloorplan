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

int main(int argc, char* argv[])
{
    Options options;
    //存储用户输入的选项
    if(!parseOptions(argc, argv, options))
    {
        printUsage(argv[0]);
        return 1;
    }

    std::cout << "MiniFloorplan started\n";
    std::cout << "LEF File: " << options.lef_file << "\n";
    std::cout << "DEF File: " << options.def_file << "\n";
    std::cout << "Output File: " << options.output_file << "\n";
    std::cout << "Read File Done\n";
    std::cout << "===============\n";

    //测试site master
    Site test_site;
    test_site.name = "CORE_SITE";
    test_site.width = 190;
    test_site.height = 1400;

    Master test_master;
    test_master.name = "NAND2_X1";
    test_master.width = 1400;
    test_master.height = 2800;
    test_master.type = MasterType::Core;

    Database db;

    db.addSite(test_site);
    db.addMaster(test_master);

    Master* found_master = db.findMaster("NAND2_X1");
    if(found_master == nullptr){
        std::cout << "Error: cannot find the master in database";
        return 1;
    }
    Instance db_inst;
    db_inst.name = "U1";
    db_inst.master = found_master;
    db_inst.origin.x = 1500;
    db_inst.origin.y = 3000;
    db_inst.orient = "N";
    db_inst.status = PlacementStatus::Placed;

    Block db_block;
    db_block.name = "test_design";
    db_block.die_area.lx = 0;
    db_block.die_area.ly = 0;
    db_block.die_area.ux = 10000;
    db_block.die_area.uy = 8000;

    db_block.core_area.lx = 1000;
    db_block.core_area.ly = 1000;
    db_block.core_area.ux = 9000;
    db_block.core_area.uy = 7000;

    db_block.addInstance(db_inst);
    db.block = db_block;

    Site* found_site = db.findSite("CORE_SITE");
    if(found_site == nullptr){
        std::cout << "Error: cannot find site CORE_SITE in database\n";
        return 1;
    }
    
    FloorplanEngine engine(db);
    if(!engin.makeUniformRows("CORE_SITE")) {
        return 1;
    }

    std::cout << "Test database-owned instance:\n";
    std::cout << "  site count       = " << db.siteCount() << "\n";
    std::cout << "  master count     = " << db.masterCount() << "\n";
    std::cout << "  block name       = " << db.block.name << "\n";
    std::cout << "  block inst count = " << db.block.instanceCount() << "\n";

    std::cout << "========================\n";
    std::cout << "block row count = " << db.block.rowCount() << "\n";
    if(!db.block.rows.empty()){
        const Row& first_row = db.block.rows[0];
        Rect row_bbox = first_row.bbox();

        std::cout << "Test row:\n";
        std::cout << "name = " << first_row.name << "\n";
        std::cout << "site = " << first_row.site->name << "\n";
        std::cout << "origin = (" << first_row.origin.x << " " << first_row.origin.y << ")\n";
        std::cout << "site count = " << first_row.site_count << "\n";
        std::cout << "spacing = " << first_row.site_spacing << "\n";
        std::cout << "width = " << first_row.width() << "\n";
        std::cout << "height = " << first_row.height() << "\n";
        std::cout << "bbox = (" << row_bbox.lx << " " << row_bbox.ly << ") (" << row_bbox.ux << " " << row_bbox.uy << ")\n";
        std::cout << "valid = " << first_row.isValid() << "\n";
    }
    if (found_site != nullptr) {
        std::cout << "  found site       = " << found_site->name << "\n";
    }

    if (found_master != nullptr) {
        std::cout << "  found master     = " << found_master->name << "\n";
    }
    if (db.block.instances.empty()){
        std::cerr << "Error : block has no instances\n";
        return 1;
    }
    const Instance& first_inst = db.block.instances[0];

    std::cout << "  instance name    = " << first_inst.name << "\n";
    std::cout << "  instance master  = " << first_inst.master->name << "\n";
    std::cout << "  instance width   = " << first_inst.width() << "\n";
    std::cout << "  instance height  = " << first_inst.height() << "\n";
    std::cout << "  instance area    = " << first_inst.area() << "\n";

    return 0;
}