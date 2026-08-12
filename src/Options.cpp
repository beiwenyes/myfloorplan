#include "Options.h"
#include <iostream>

void printUsage(const char* program_name)
{
    std::cerr << "Usage: " << program_name
              << "--lef <file> --def <file> --output <file>\n";
}

bool parseOptions(int argc, char* argv[], Options& options)
{
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if(arg == "--lef" && i+1 < argc){
            options.lef_file = argv[++i];
        } else if (arg == "--def" && i+1 < argc){
            options.def_file = argv[++i];
        } else if (arg == "--output" && i+1 < argc){
            options.output_file = argv[++i];
        } else{
            std::cerr << "Unknow or incoomplete argument" << arg << "\n";
            return false;
        }
    }
    if(options.lef_file.empty()){
        std::cerr << "Missing required option: --lef\n";
        return false;
    }
    if(options.def_file.empty()){
        std::cerr << "Missing required option: --def\n";
        return false;
    }
    if(options.output_file.empty()){
        std::cerr << "Missing required option: --output\n";
        return false;
    }

    return true;
}