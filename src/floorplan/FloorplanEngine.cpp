#include "floorplan/FloorplanEngine.h"

#include <iostream>
#include <string>

FloorplanEngine::FloorplanEngine(Database& db)
    : db_(db)
{
}

bool FloorplanEngine::makeUniformRows(const std::string& site_name)
{
    Site* site = db_.findSite(site_name);

    if (site == nullptr) {
        std::cerr << "Error: cannot find site: " << site_name << "\n";
        return false
    }

    if (!site->isValid()){
        std::cerr << "Error: invalid site: " << site_name << "\n";
        return false;
    }

    if (!db_.block.hasValidCoreArea()){
        std::cerr << "Error: block has invalid core area\n";
        return false;
    }

    const Rect& core = db_.block.core_area;

    int row_count = core.height() / site->height;
    int site_count = core.width() / site->width;

    if(row_count <= 0){
        std::cerr << "Error: core area is too short for one row\n";
        return false;
    }

    db_.block.rows.clear();

    for (int i = 0; i < row_count; ++i){
        Row row;

        row.name = "ROW_" + std::to_string(i);
        row.site = site;
        row.origin.x = core.lx;
        row.origin.y = core.ly + i * site->height;
        //N和FS：N是正常方向，FS是N的上下翻转，是指标准单元的放置方向
        row.orient = (i % 2 == 0) ? "N" : "FS";
        row.site_count = site_count;
        row.site_spacing = site->width;
        db_.block.addRow(row);
    }
    return true;
}