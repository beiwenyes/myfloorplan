#include "floorplan/FloorplanEngine.h"

#include <iostream>
#include <string>

FloorplanEngine::FloorplanEngine(Database& db)
    : db_(db)
{
}
bool FloorplanEngine::initializeCoreAreaWithMargin(Dbu margin)
{
    if(!db_.block.hasValidDieArea()){
        std::cerr << "Error: cannot initialize core area because die area is invalid.\n";
        return false;
    }
    if(margin < 0){
        std::cerr << "Error: core margin cannot be negative.\n";
        return false;
    }

    const Rect& die = db_.block.die_area;

    if(die.width() <= 2 * margin || die.height() <= 2 * margin){
        std::cerr << "Error: core margin is too large for die area.\n";
        return false;
    }

    db_.block.core_area.lx = die.lx + margin;
    db_.block.core_area.ly = die.ly + margin;
    db_.block.core_area.ux = die.ux - margin;
    db_.block.core_area.uy = die.uy - margin;

    return true;
}
bool FloorplanEngine::makeUniformRows(const std::string& site_name)
{
    Site* site = db_.findSite(site_name);

    if (site == nullptr) {
        std::cerr << "Error: cannot find site: " << site_name << "\n";
        return false;
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

bool FloorplanEngine::validateFloorplan() const
{
    if(!db_.block.hasValidDieArea()){
        std::cerr << "Error: invalid die area\n";
        return false;
    }

    if(!db_.block.hasValidCoreArea()){
        std::cerr << "Error: invalid core area\n";
        return false;
    }

    if(!db_.block.die_area.contains(db_.block.core_area)){
        std::cerr << "Error: core area is outside die area\n";
        return false;
    }

    if(db_.block.rowCount() == 0){
        std::cerr << "Error: no placement rows generated\n";
        return false;
    }

    Area instance_area = db_.block.totalInstanceArea();
    Area row_area = db_.block.rowArea();

    if(instance_area > row_area){
        std::cerr << "Error: instance area exceeds row area\n";
        std::cerr << "  instance area = " << instance_area << "\n";
        std::cerr << "  row area = " << row_area << "\n";
        return false;
    }

    if(!validateInstancesFitRows()){
        return false;
    }

    return true;
}

bool FloorplanEngine::validateInstancesFitRows() const
{
    if(db_.block.rows.empty()){
        std::cerr << "Error: cannot validate instances because no rows exist\n";
        return false;
    }

    const Row& first_row = db_.block.rows[0];
    const Dbu row_height = first_row.height();
    
    if(row_height <= 0){
        std::cerr << "Error: invalid row height\n";
        return false;
    }

    for (const Instance& instance : db_.block.instances){
        if(instance.master == nullptr){
            std::cerr << "Error: instance has no master: "
                      << instance.name << "\n";
            return false;
        }

        if(instance.height() != row_height){
            std::cerr << "Error: instance height does not match row height\n";
            std::cerr << "  instance name   = " << instance.name << "\n";
            std::cerr << "  master name     = " << instance.master->name << "\n";
            std::cerr << "  instance height = " << instance.height() << "\n";
            std::cerr << "  row height      = " << row_height << "\n";
            return false;
        }
    }

    return true;
}