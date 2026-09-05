#pragma once

#include <string>

struct Options{
    std::string lef_file;
    std::string def_file;
    std::string output_file;
    int core_margin = 10000;
    std::string site_name;
};

bool parseOptions(int argc, char* argv[], Options& options);

void printUsage(const char* program_name);