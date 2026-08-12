#include "Options.h"
#include "db/Geometry.h"
#include <iostream>
#include "db/Site.h"
#include "db/Master.h"


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

    //测试site master
    Site test_site;
    test_site.name = "CORE_SITE";
    test_site.width = 190;
    test_site.height = 1400;

    std::cout << "Test site:\n";
    std::cout << " name = " << test_site.name << "\n";
    std::cout << " width = " << test_site.width << "\n";
    std::cout << " height = " << test_site.height << "\n";
    std::cout << " valid = " << test_site.isValid() << "\n";

    Master test_master;
    test_master.name = "NAND2_X1";
    test_master.width = 1400;
    test_master.height = 2800;
    test_master.type = MasterType::Core;

    std::cout << "Test master:\n";
    std::cout << "name = " << test_master.name << "\n";
    std::cout << "width = " << test_master.width << "\n";
    std::cout << "height = " << test_master.height << "\n";
    std::cout << "area = " << test_master.area() << "\n";
    std::cout << "valid = " << test_master.isValid() << "\n";
    std::cout << "is Core = " << test_master.isCore() << "\n";



    return 0;
}