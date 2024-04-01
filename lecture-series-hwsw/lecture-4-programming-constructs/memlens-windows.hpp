#ifndef MEMLENS_WINDOWS_HPP
#define MEMLENS_WINDOWS_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <io.h>
#include <iostream>
#include <sstream>

#include <windows.h>
#include <psapi.h>
#include "memlens.hpp"

#include <stdio.h>

//#define is_term() _isatty(_fileno(stdout))
#define is_term() false

typedef struct {
    MODULEINFO info;
    std::string filename;
} MODFILEINFO;

void load_memory_layout(std::vector<memory_region_t> &regions) {
    HANDLE process = GetCurrentProcess();
    HMODULE modules[1024];
    DWORD cbNeeded;
    std::vector<MODFILEINFO> moduleInfos;
    if (EnumProcessModules(process, modules, sizeof(modules), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            MODULEINFO info;
            if (GetModuleInformation(process, modules[i], &info, sizeof(info))) {
                TCHAR moduleName[MAX_PATH];
                if (GetModuleBaseName(process, modules[i], moduleName, sizeof(moduleName) / sizeof(TCHAR))) {
                    std::stringstream name_ss;
                    name_ss << moduleName;
                    MODFILEINFO mfi;
                    mfi.filename = name_ss.str();
                    mfi.info = info;
                    moduleInfos.push_back(mfi);
                }
            }
        }
    }

    MEMORY_BASIC_INFORMATION mbi;
    uint8_t *malloc_ptr = new uint8_t[1024*4];
    char* addr = 0;
    while (VirtualQueryEx(process, addr, &mbi, sizeof(mbi))) {
        memory_region_t region;
        region.start_address = mbi.BaseAddress;
        region.size = mbi.RegionSize;

        PVOID mbi_ptr = (PVOID)&mbi;
        if (mbi_ptr >= mbi.BaseAddress && (size_t)((uint8_t*)mbi_ptr - (uint8_t*)mbi.BaseAddress) < mbi.RegionSize) {
            region.region_type = "stack";
            region.region_detail = "-";
        } else if (malloc_ptr >= mbi.BaseAddress && (size_t)((uint8_t*)malloc_ptr - (uint8_t*)mbi.BaseAddress) < mbi.RegionSize) {
            region.region_type = "heap";
            region.region_detail = "-";
        } else {
            region.region_type = "";
            region.region_detail = "";
        }

        region.permissions = 0;
        if (mbi.Protect & PAGE_READONLY) {
            region.permissions |= PERM_READ;
        }
        if (mbi.Protect & PAGE_READWRITE) {
            region.permissions |= PERM_READ | PERM_WRIT;
        }
        if (mbi.Protect & PAGE_EXECUTE) {
            region.permissions |= PERM_EXEC;
        }
        if (mbi.Protect & PAGE_EXECUTE_READ) {
            region.permissions |= PERM_EXEC | PERM_READ;
        }
        if (mbi.Protect & PAGE_EXECUTE_READWRITE) {
            region.permissions |= PERM_EXEC | PERM_READ | PERM_WRIT;
        }
        if (mbi.Protect & PAGE_NOACCESS) {
            region.permissions = 0;
        }

        addr += mbi.RegionSize;
        region.resident_size = 0;

        if (mbi.State == MEM_COMMIT) {
            region.resident_size = region.size;
        } else if (mbi.State == MEM_RESERVE) {
            region.region_type = "reserved";
        } else {
            continue;
        }

        if (mbi.Type == MEM_IMAGE) {
            for (std::vector<MODFILEINFO>::const_iterator it = moduleInfos.begin(); it != moduleInfos.end(); ++it) {
                if (mbi.BaseAddress >= it->info.lpBaseOfDll && (void*)((uint8_t*)mbi.BaseAddress + mbi.RegionSize) < (void*)((uint8_t*)it->info.lpBaseOfDll + it->info.SizeOfImage)) {
                    region.region_detail = it->filename;
                    bool isMemlens = region.region_detail.find("memlens") != std::string::npos;
                    if (isMemlens) {
                        if (region.permissions & PERM_EXEC) {
                            region.region_type = "code";
                        } else if (region.permissions & PERM_WRIT) {
                            region.region_type = "data";
                        } else if (region.permissions & PERM_READ) {
                            region.region_type = mbi.BaseAddress == it->info.lpBaseOfDll ? "loader" : "const";
                        } else {
                            region.region_type = "-";
                        }
                    } else {
                        region.region_type = "dll";
                    }
                }
            }
        }

        regions.push_back(region);
    }

    delete[] malloc_ptr;
    CloseHandle(process);
}

#endif // is windows
#endif // MEMLENS_WINDOWS_HPP