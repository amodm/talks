#ifndef MEMLENS_HPP
#define MEMLENS_HPP

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

const uint8_t PERM_READ = 0x01;
const uint8_t PERM_WRIT = 0x02;
const uint8_t PERM_EXEC = 0x04;

typedef struct {
    void *start_address;
    size_t size;
    size_t resident_size;
    std::string region_type;
    std::string region_detail;
    uint8_t permissions;
} memory_region_t;

/// @brief Load the memory layout of the current process into `regions`
void load_memory_layout(std::vector<memory_region_t> &regions);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "memlens-windows.hpp"
#elif defined(__linux__)
#include "memlens-linux.hpp"
#elif defined(__APPLE__)
#include "memlens-macos.hpp"
#endif

uint32_t* prime_factors(uint32_t num);

void print_prime_factors(uint32_t num);

void show_usage(const char *error);

void test_throwing_func();

extern std::vector<memory_region_t> MEMORY_REGIONS;

/// @brief Update the memory layout of the current process.
void update_memory_layout() {
    MEMORY_REGIONS.clear();
    load_memory_layout(MEMORY_REGIONS);
}

/// @return a human readable version of `address`, e.g. stack - 10, or heap + 20
std::string named_address(const void *address) {
    size_t offset = 1<<30;
    std::string result;
    for (std::vector<memory_region_t>::const_iterator it = MEMORY_REGIONS.begin(); it != MEMORY_REGIONS.end(); ++it) {
        if (address >= it->start_address && address < (void*)((uint8_t*)it->start_address + it->size)) {
            if (it->region_type == "stack") {
                const size_t new_offset = (uint8_t*)it->start_address + it->size - (uint8_t*)address;
                if (new_offset < offset) {
                    offset = new_offset;
                    result = "[stack - " + std::to_string(offset) + "]";
                }
            } else {
                const size_t new_offset = (uint8_t*)address - (uint8_t*)it->start_address;
                if (new_offset < offset) {
                    offset = new_offset;
                    result = "[" + it->region_type + " + " + std::to_string(offset) + "]";
                }
            }
        }
    }
    std::stringstream ss;
    ss << std::hex << (void*)address << std::dec;
    if (!result.empty()) {
        ss << " @" << result;
    }
    return ss.str();
}

/// @return the address of the memory region with the given `name`
void* region_name_to_address(const std::string &name) {
    for (std::vector<memory_region_t>::const_iterator it = MEMORY_REGIONS.begin(); it != MEMORY_REGIONS.end(); ++it) {
        if (name == it->region_type) {
            return it->start_address;
        }
    }
    // treat name as an address
    return (void*)std::stoull(name, nullptr, 0);
}

/// @return human readable string representation of `size`
std::string size_str(size_t size) {
    std::string size_s;
    if (size < 1024) {
        size_s = std::to_string(size);
    } else if (size < 1024 * 1024) {
        size_s = std::to_string(size / 1024) + "K";
    } else if (size < 1024 * 1024 * 1024) {
        size_s = std::to_string(size / (1024 * 1024)) + "M";
    } else {
        size_s = std::to_string(size / (1024 * 1024 * 1024)) + "G";
    }
    return size_s;
}

/// @brief Print the memory layout of the current process.
void print_memory_layout() {
    std::cout
            << std::setfill(' ')
            << std::setw(16) << std::right << "start_addr "
            << "-"
            << std::setw(16) << std::left << " end_addr"
            << " "
            << std::setw(5) << std::right << "range"
            << " "
            << std::setw(8) << std::right << "resident"
            << " " << "perm"
            << " "
            << std::setw(8) << std::right << "type"
            << "  "
            << "detail"
            << std::endl
            << "------------------------------------------------------------"
            << std::endl;

    for (std::vector<memory_region_t>::const_iterator it = MEMORY_REGIONS.begin(); it != MEMORY_REGIONS.end(); ++it) {
        std::string size_s = size_str(it->size);
        std::string res_s = size_str(it->resident_size);

        std::cout
            << std::hex
            << std::setfill(' ')
            << std::setw(16) << std::right << std::noshowbase << reinterpret_cast<uintptr_t>(it->start_address)
            << "-"
            << std::setw(16) << std::left << std::noshowbase << reinterpret_cast<uintptr_t>((uint8_t*)it->start_address + it->size)
            << " "
            << std::dec
            << std::setw(5) << std::right << size_s
            << " "
            << std::setw(8) << std::right << res_s
            << "  "
            << (it->permissions & PERM_READ ? "r" : "-")
            << (it->permissions & PERM_WRIT ? "w" : "-")
            << (it->permissions & PERM_EXEC ? "x" : "-")
            << " "
            << std::setw(8) << std::right << it->region_type
            << "  "
            << (it->region_detail.find("memlens") != std::string::npos ? "/*/memlens" : it->region_detail)
            << std::endl;
    }
}

/// @brief Dump memory in hexdump format, starting at `address`.
/// @param address memory address to dump from
/// @param size number of bytes to dump
void dump_memory(const void *address, size_t size) {
    const uint8_t *end_address = (uint8_t*)address + size;
    const uint8_t *address_base = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(address) & ~0xf);
    const size_t offset = (uint8_t*)address - address_base;
    const size_t rows = (size + offset + 15) / 16;
    for (size_t row = 0; row < rows; row++) {
        const uint8_t *row_address = address_base + row * 16;
        // print row address
        std::cout << std::hex << std::noshowbase << reinterpret_cast<uintptr_t>(row_address) << "  ";
        // print hex bytes
        for (size_t i = 0; i < 16; i++) {
            const uint8_t *u8_addr = row_address + i;
            if (u8_addr >= address && u8_addr < end_address) {
                std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*u8_addr << " ";
            } else {
                std::cout << "   ";
            }
        }
        // print ascii
        std::cout << "  |";
        for (size_t i = 0; i < 16; i++) {
            const uint8_t *u8_addr = row_address + i;
            if (u8_addr >= address && u8_addr < end_address) {
                const uint8_t c = *u8_addr;
                std::cout << (c >= 32 && c < 127 ? (char)c : '.');
            } else {
                std::cout << " ";
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::dec << std::endl;
}

const char* BOLD_MODE = "\033[1m";
const char* NO_BOLD_MODE = "\033[22m";

const char* COLOR_GREEN = "\033[32m";
const char* COLOR_RED = "\033[31m";
const char* COLOR_CYAN = "\033[36m";
const char* colors[] = {COLOR_RED, COLOR_CYAN, COLOR_GREEN};

const char* COLOR_YELLOW = "\033[33m";
const char* COLOR_MAGENTA = "\033[35m";
const char* COLOR_BLUE = "\033[34m";
const char* ZONE_COLORS[] = {COLOR_MAGENTA, COLOR_YELLOW, COLOR_BLUE};

const char* COLOR_RESET = "\033[0m";

void print_address_of(bool cond, void *address, const char *func, const char *name, size_t depth, size_t padlen) {
    const char *func_color = colors[depth % (sizeof(colors)/sizeof(colors[0]))];
    const char *zone_color;
    const std::string named_addr = named_address(address);
    if (named_addr.find("code") != std::string::npos) {
        zone_color = COLOR_YELLOW;
    } else if (named_addr.find("stack") != std::string::npos) {
        zone_color = COLOR_BLUE;
    } else if (named_addr.find("heap") != std::string::npos) {
        zone_color = COLOR_MAGENTA;
    } else {
        zone_color = func_color;
    }
    if (cond) {
        const bool tty = is_term();
        std::cout
            << std::setfill(' ')
            << std::setw(padlen)
            << "";
        std::cout
            << (tty ? func_color : "")
            << "&[";
        std::cout
            << (func == nullptr ? "" : func)
            << (tty ? BOLD_MODE : "")
            << name
            << (tty ? NO_BOLD_MODE : "")
            << "] = "
            << (tty ? zone_color : "")
            << named_addr
            << (tty ? COLOR_RESET : "")
            << std::endl;
    }
}

#define print_address_of_func(cond, x, depth) print_address_of(cond, (void*)&x, nullptr, "@func{" #x "}", depth, 4*depth)
#define print_address_of_param(cond, x, depth) print_address_of(cond, (void*)&x, __FUNCTION__, "@param{" #x "}", depth, 4*depth+2)
#define print_address_of_localvar(cond, x, depth) print_address_of(cond, (void*)&x, __FUNCTION__, "@localvar{" #x "}", depth, 4*depth+2)

class Employee {
private:
    int id;
    int exp_yrs;
    int salary;
public:
    Employee(int id, int exp_yrs, int salary)
        :id(id), exp_yrs(exp_yrs),
            salary(salary) {}

    int salary_per_yr_of_exp() const {
        // hack to get the address of the function
        typedef int (Employee::* pfunc)() const;
        pfunc fptr = &Employee::salary_per_yr_of_exp;
        uint8_t *f_salary_per_yr_of_exp = *(uint8_t**)&fptr;
        print_address_of_func(true, *f_salary_per_yr_of_exp, 1);
        print_address_of_localvar(true, *this, 1);

        // the only logic of the function is this one-liner here
        return salary / exp_yrs;
    }

    std::string label() const {
        // hack to get the address of the function
        typedef std::string (Employee::* pfunc)() const;
        pfunc fptr = &Employee::label;
        uint8_t *f_label = *(uint8_t**)&fptr;
        print_address_of_func(true, *f_label, 1);
        print_address_of_localvar(true, *this, 1);

        // the only logic of the function is this one-liner here
        return "Employee";
    }
};

class CEO : public Employee {
public:
    CEO(int id, int exp_yrs, int salary)
        :Employee(id, exp_yrs, salary) {}

    std::string label() const {
        // hack to get the address of the function
        typedef std::string (CEO::* pfunc)() const;
        pfunc fptr = &CEO::label;
        uint8_t *f_label = *(uint8_t**)&fptr;
        print_address_of_func(true, *f_label, 1);
        print_address_of_localvar(true, *this, 1);

        // the only logic of the function is this one-liner here
        return "CEO";
    }
};

class EmployeeDyn {
private:
    int id;
    int exp_yrs;
    int salary;
public:
    EmployeeDyn(int id, int exp_yrs, int salary)
        :id(id), exp_yrs(exp_yrs),
            salary(salary) {}

    int salary_per_yr_of_exp() const {
        // hack to get the address of the function
        typedef int (EmployeeDyn::* pfunc)() const;
        pfunc fptr = &EmployeeDyn::salary_per_yr_of_exp;
        uint8_t *f_salary_per_yr_of_exp = *(uint8_t**)&fptr;
        print_address_of_func(true, *f_salary_per_yr_of_exp, 1);
        print_address_of_localvar(true, *this, 1);

        // the only logic of the function is this one-liner here
        return salary / exp_yrs;
    }

    virtual std::string label() const {
        // hack to get the address of the function
        typedef std::string (EmployeeDyn::* pfunc)() const;
        pfunc fptr = &EmployeeDyn::label;
        uint8_t *f_label = *(uint8_t**)&fptr;
        print_address_of_func(true, *f_label, 1);
        print_address_of_localvar(true, *this, 1);

        // the only logic of the function is this one-liner here
        return "EmployeeDyn";
    }
};

class CEODyn : public EmployeeDyn {
public:
    CEODyn(int id, int exp_yrs, int salary)
        :EmployeeDyn(id, exp_yrs, salary) {}

    std::string label() const {
        // hack to get the address of the function
        typedef std::string (CEODyn::* pfunc)() const;
        pfunc fptr = &CEODyn::label;
        uint8_t *f_label = *(uint8_t**)&fptr;
        print_address_of_func(true, *f_label, 1);
        print_address_of_localvar(true, *this, 1);

        // the only logic of the function is this one-liner here
        return "CEODyn";
    }
};

#endif // MEMLENS_HPP
