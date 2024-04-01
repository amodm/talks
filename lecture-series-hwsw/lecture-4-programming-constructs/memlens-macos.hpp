/// @file
/// @brief macOS specific memory related functions.
/// @details This file contains macOS specific memory related functions. Going through
/// this file is not necessary for understanding the lecture. This file is used only
/// to make sure that the code compiles and runs on macOS. The real meat is in the
/// `memory.cpp` file.

#ifndef MEMLENS_MACOS_HPP
#define MEMLENS_MACOS_HPP

#ifdef __APPLE__

#include <iomanip>
#include <libproc.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/vm_statistics.h>
#include <mach-o/getsect.h>
#include <mach-o/loader.h>
#include <malloc/malloc.h>
#include <unistd.h>
#include "memlens.hpp"

#define is_term() isatty(fileno(stdout))

void load_memory_layout(std::vector<memory_region_t> &regions) {
    mach_port_t task = mach_task_self();
    mach_vm_address_t address = 0;
    mach_vm_size_t size = 0;
    const int page_size = getpagesize();

    uint8_t *data_addr = 0;
    uint8_t *const_addr = 0;
    uint8_t *linksh_addr = 0;

    while (1) {
        mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
        vm_region_basic_info_data_64_t info;
        mach_msg_type_number_t ext_count = VM_REGION_EXTENDED_INFO_COUNT;
        vm_region_extended_info_data_t ext_info;
        mach_port_t object_name;

        kern_return_t result = mach_vm_region(task, &address, &size, VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info, &count, &object_name);
        if (result != KERN_SUCCESS) {
            break;
        }
        vm_prot_t prot = info.protection;
        int user_tag = 0;
        result = mach_vm_region(task, &address, &size, VM_REGION_EXTENDED_INFO, (vm_region_info_t)&ext_info, &ext_count, &object_name);
        if (result == KERN_SUCCESS) {
            user_tag = ext_info.user_tag;
        }

        memory_region_t region;
        region.start_address = (void*)address;
        region.size = size;
        region.region_type = "-";
        const bool writable = prot & VM_PROT_WRITE;
        switch (user_tag) {
            case VM_MEMORY_MALLOC:
                if (prot == VM_PROT_NONE) {
                    region.region_type = "guard";
                    region.region_detail = "malloc guard";
                } else {
                    region.region_type = "meta";
                    region.region_detail = "malloc metadata";
                }
                break;
            case VM_MEMORY_STACK:
                region.region_type = prot == VM_PROT_NONE ? "guard" : "stack";
                region.region_detail = prot == VM_PROT_NONE ? "stack guard" : "stack";
                break;
            case VM_MEMORY_MALLOC_SMALL:
                region.region_type = "heap";
                region.region_detail = "malloc small";
                break;
            case VM_MEMORY_MALLOC_LARGE:
            case VM_MEMORY_MALLOC_LARGE_REUSED:
            case VM_MEMORY_MALLOC_LARGE_REUSABLE:
                region.region_type = "heap";
                region.region_detail = "malloc large";
                break;
            case VM_MEMORY_REALLOC:
                region.region_type = "heap";
                region.region_detail = "malloc realloc";
                break;
            case VM_MEMORY_MALLOC_HUGE:
                region.region_type = "heap";
                region.region_detail = "malloc huge";
                break;
            case VM_MEMORY_SBRK:
                region.region_type = "heap";
                region.region_detail = "malloc sbrk";
                break;
            case VM_MEMORY_MALLOC_TINY:
                region.region_type = "heap";
                region.region_detail = "malloc tiny";
                break;
            case VM_MEMORY_MALLOC_MEDIUM:
                region.region_type = "heap";
                region.region_detail = "malloc medium";
                break;
            case VM_MEMORY_MALLOC_NANO:
                region.region_type = "heap";
                region.region_detail = "malloc nano";
                break;
            case VM_MEMORY_OS_ALLOC_ONCE:
                region.region_type = "kernel";
                region.region_detail = "kernel alloc once";
                break;
            case VM_MEMORY_DYLD:
                region.region_type = "dyld";
                region.region_detail = "dyld";
                break;
            case VM_MEMORY_DYLD_MALLOC:
                region.region_type = "dyld";
                region.region_detail = "dyld malloc";
                break;
            default: {
                char filename[1024];
                memset(filename, 0, sizeof(filename));
                const int num_written = proc_regionfilename(getpid(), address, filename, sizeof(filename));
                if (num_written > 0) {
                    region.region_detail = filename;
                    uint32_t first4bytes = (size >= 4 && prot & VM_PROT_READ && user_tag == 0) ? *(uint32_t*)address : 0;
                    if (first4bytes == MH_MAGIC_64) {
                        unsigned long seg_size = 0;
                        data_addr = (uint8_t*)getsegmentdata((mach_header_64*)address, SEG_DATA, &seg_size);
                        const_addr = (uint8_t*)getsegmentdata((mach_header_64*)address, "__DATA_CONST", &seg_size);
                        linksh_addr = (uint8_t*)getsegmentdata((mach_header_64*)address, "__LINKEDIT", &seg_size);
                        region.region_type = "code";
                    } else {
                        if ((uint8_t*)address == data_addr) {
                            region.region_type = "data";
                        } else if ((uint8_t*)address == const_addr) {
                            region.region_type = "const";
                        } else if ((uint8_t*)address == linksh_addr) {
                            region.region_type = "linkd";
                        } else {
                            region.region_type = "-";
                        }
                    }
                } else {
                    region.region_type = "-";
                    region.region_detail = "";
                    if (info.reserved) {
                        if (region.region_detail.size() > 0) region.region_detail.append(", ");
                        region.region_detail.append("reserved");
                    }
                    if (info.shared) {
                        if (region.region_detail.size() > 0) region.region_detail.append(", ");
                        region.region_detail.append("shared");
                    }
                }
            }
        }

        region.permissions = 0;
        if (prot & VM_PROT_READ)    region.permissions |= PERM_READ;
        if (prot & VM_PROT_WRITE)   region.permissions |= PERM_WRIT;
        if (prot & VM_PROT_EXECUTE) region.permissions |= PERM_EXEC;

        region.resident_size = ext_info.pages_resident * page_size;
        regions.push_back(region);

        address += size;
    }

    // invoke vmmap if the MACOS_VMMAP environment variable is set
    // as a way to get more details.
    const char *vmmap_env = getenv("MACOS_VMMAP");
    if (vmmap_env && strcmp(vmmap_env, "1") == 0) {
        system((std::string("vmmap -w ") + std::to_string(getpid())).c_str());
    }
}

#endif // __APPLE__
#endif // MEMLENS_MACOS_HPP