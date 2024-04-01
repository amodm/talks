#ifndef MEMLENS_LINUX_HPP
#define MEMLENS_LINUX_HPP

#ifdef __linux__

#include <fstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include "memlens.hpp"

#define is_term() isatty(fileno(stdout))

void load_memory_layout(std::vector<memory_region_t> &regions) {
    std::ifstream maps("/proc/self/maps");
    if (!maps.is_open()) {
        std::cerr << "failed to open /proc/self/maps" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(maps, line)) {
        std::istringstream iss(line);
        memory_region_t region;
        iss >> std::hex >> region.start_address;
        iss.ignore(1); // skip '-'
        void *end_address;
        iss >> std::hex >> end_address;
        region.size = (uint8_t*)end_address - (uint8_t*)region.start_address;
        // iss.ignore(1); // skip ' '
        std::string perms;
        iss >> perms;
        region.permissions = 0;
        if (perms.find('r') != std::string::npos) {
            region.permissions |= PERM_READ;
        }
        if (perms.find('w') != std::string::npos) {
            region.permissions |= PERM_WRIT;
        }
        if (perms.find('x') != std::string::npos) {
            region.permissions |= PERM_EXEC;
        }
        region.resident_size = 0; // TODO: implement via /proc/self/smaps
        // iss.ignore(1); // skip ' '
        uint64_t file_offset;
        iss >> std::hex >> file_offset;
        std::string ignore;
        iss >> ignore; // skip device major minor
        iss >> ignore; // skip inode
        std::string detail;
        iss >> detail;
        if (detail.length() > 0 && detail[0] == '/') {
            region.region_detail = detail;
            bool this_file = detail.find("/memlens") != std::string::npos;
            if (file_offset == 0) {
                if (this_file) {
                    region.region_type = "loader";
                } else {
                    region.region_type = "-";
                    region.region_detail += " (loader)";
                }
            } else if (region.permissions & PERM_EXEC) {
                if (this_file) {
                    region.region_type = "code";
                } else {
                    region.region_type = "-";
                    region.region_detail += " (code)";
                }
            } else if (region.permissions & PERM_WRIT) {
                if (this_file) {
                    region.region_type = "data";
                } else {
                    region.region_type = "-";
                    region.region_detail += " (data)";
                }
            } else if (region.permissions & PERM_READ) {
                if (this_file) {
                    region.region_type = "const";
                } else {
                    region.region_type = "-";
                    region.region_detail += " (const)";
                }
            } else {
                region.region_type = "-";
            }
        } else if (detail.length() > 0 && detail[0] == '[') {
            region.region_type = detail.substr(1, detail.length() - 2);
            region.region_detail = "-";
        } else {
            region.region_type = "-";
            region.region_detail = detail;
        }
        regions.push_back(region);
    }
}

#endif // is linux
#endif // MEMLENS_LINUX_HPP