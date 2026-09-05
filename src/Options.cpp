#include "Options.h"
#include <iostream>
#include <stdexcept>

void printUsage(const char* program_name)
{
    //[] means it's optional
    std::cerr << "Usage: " << program_name
              << "--lef <file> --def <file> --output <file>"
              << " [--core-margin <dbu>]"
              << " [--site <site_name>]\n";
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
        } else if (arg == "--core-margin" && i+1 < argc){
            try{
                //change the string to int. if input is not number will be catch
                options.core_margin = std::stoi(argv[++i]);
            } catch (const std::exception&){
                std::cerr << "Invalid value for --core-margin\n";
                return false;
            }
        } else if (arg == "--site" && i + 1 < argc){
            options.site_name = argv[++i];
        } else {
            std::cerr << "Unknown or incomplete argument: " << arg << "\n";
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