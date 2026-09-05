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
#include "writer/DefWriter.h"

void printRect(const std::string& name, const Rect& rect)
{
    std::cout << " " << name << " = ("
              << rect.lx << " " << rect.ly << ") ("
              << rect.ux << " " << rect.uy << ")\n";
}

int main(int argc, char* argv[])
{
    Options options;
    //存储用户输入的选项
    if(!parseOptions(argc, argv, options))
    {
        printUsage(argv[0]);
        return 1;
    }
    //start point
    std::cout << "MiniFloorplan Started\n";
    std::cout << "LEF file      : " << options.lef_file << "\n";
    std::cout << "DEF file      : " << options.def_file << "\n";
    std::cout << "Output file   : " << options.output_file << "\n\n";

    //create DataBase
    Database db;
    //create lef parser
    std::cout << "[1] Parse LEF\n";
    LefParser lef_parser;
    if(!lef_parser.read(options.lef_file, db)){
        return 1;
    }
    //lef report
    std::cout << "  line count      : " << lef_parser.lineCount() << "\n";
    std::cout << "  dbu per micron  : " << db.tech.dbu_per_micron << "\n";
    std::cout << "  site count      : " << db.siteCount() << "\n";
    std::cout << "  master count    : " << db.masterCount() << "\n";

    if(!db.sites.empty()){
        const Site& site = db.sites.begin()->second;
        std::cout << "  first site      : " << site.name
                  << ",width=" << site.width
                  << ",height=" << site.height << "\n";
    }
    std::cout << "\n";
    //lef report end

    //create def parser
    std::cout << "[2] Parse DEF\n";
    DefParser def_parser;
    if(!def_parser.read(options.def_file, db)){
        return 1;
    }
    //DEF report
    std::cout << "  line count      : " << def_parser.lineCount() << "\n";
    std::cout << "  dbu per micron  : " << def_parser.dbuPerMicron() << "\n";
    std::cout << "  design name     : " << db.block.name << "\n";
    printRect("die area", db.block.die_area);
    std::cout << "  die area valid  : " << db.block.hasValidDieArea() << "\n";
    std::cout << "  component count : " << db.block.instanceCount() << "\n";
    if(!db.block.instances.empty()){
        const Instance& instance = db.block.instances[0];
        std::cout << "  First DEF component: " << instance.name << "\n";
        if(instance.master != nullptr){
            std::cout << "  master name = " << instance.master->name << "\n";
            std::cout << "  width = " << instance.width() << "\n";
            std::cout << "  height = " << instance.height() << "\n";
        }
        std::cout << "\n";

    }
    std::cout << "\n";
    //def report end
    //start initialize
    std::cout << "[3] Initialize floorplan\n";
    if(db.sites.empty()){
        std::cerr << "Error:  no site found in LEF\n";
        return 1;
    }

    std::string site_name;
    if(!options.site_name.empty()){
        site_name = options.site_name;
    } else {
        site_name = db.sites.begin() -> first;
    }

    if(db.findSite(site_name) == nullptr){
        std::cerr << "Error: site not found: " << site_name << "\n";
        return 1;
    }

    FloorplanEngine engine(db);

    const Dbu core_margin = options.core_margin;
    if(!engine.initializeCoreAreaWithMargin(core_margin)){
        return 1;
    }
    if(!engine.makeUniformRows(site_name)){
        return 1;
    }
    std::cout << "  selected site : " << site_name << "\n";
    std::cout << "  core margin = " << core_margin << "\n";
    printRect("core area", db.block.core_area);
    std::cout << "  row count     : " << db.block.rowCount() << "\n";
 
    if (!db.block.rows.empty()) {
        const Row& row = db.block.rows[0];
        Rect row_bbox = row.bbox();
        std::cout << "  first row       : " << row.name << ", site_count = " << row.site_count << ", orient = " << row.orient << "\n";
        printRect("first row bbox", row_bbox);
    }
    std::cout << "\n";

    //start write the def output
    std::cout << "[4] Write DEF\n";
    DefWriter def_writer;

    if(!def_writer.write(options.output_file, db)){
        return 1;
    }
    std::cout << "  file    : " << options.output_file << "\n";
    std::cout << "  status : success\n\n";
    
    std::cout << "MiniFloorplan done\n";
    return 0;
}