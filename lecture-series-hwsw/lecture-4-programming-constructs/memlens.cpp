/// @file
/// @brief Utility to demonstrate memory layouts.
///
/// This program showcases memory management in C++. To build this, run
/// the command: `make memlens`
///
/// You don't have to know everything here, just focus on the semantics, not
/// the syntax.

#include <sstream>
#include <vector>
#include "memlens.hpp"

/// The main entry point for this utility.
///
/// @param argc total number of arguments (first one is the name of the program)
/// @param argv the argument values
/// @return 0 if successful, 1 if there was an error
int main(int argc, const char *argv[]) {
    // we would normally use something like getopt to parse command line arguments
    // but in the interest of no dependencies, we'll simply parse it ourselves.
    if (argc < 2) {
        show_usage("missing command");
    }

    // parse command line arguments
    std::string command;
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        const std::string arg = std::string(argv[i]);
        if (arg == "-h" || arg == "--help") {
            show_usage(nullptr);
        } else if (arg.length() > 0) {
            if (command.length() > 0) {
                args.push_back(arg);
            } else {
                command = arg;
            }
        }
    }

    update_memory_layout();

    // execute the command
    if (command == "help") {
        show_usage(nullptr);
    } else if (command == "demo-call") {
        print_address_of_func(true, main, 0);
        print_address_of_localvar(true, argc, 0);
        print_prime_factors(12348);
    } else if (command == "layout") {
        print_memory_layout();
    } else if (command == "demo-class") {
        uint32_t before_emp = 0;
        Employee emp(1, 2, 100);
        int salary_per_yr_of_exp = 0;
        print_address_of_localvar(true, before_emp, 0);
        print_address_of_localvar(true, emp, 0);
        print_address_of_localvar(true, salary_per_yr_of_exp, 0);
        salary_per_yr_of_exp = emp.salary_per_yr_of_exp();
        std::cout << "Salary per year of experience: " << salary_per_yr_of_exp << std::endl;
    } else if (command == "demo-poly") {
        uint32_t before_emp = 0;
        Employee emp(1, 2, 100);
        int salary_per_yr_of_exp = 0;
        std::cout << "---- Employee ----" << std::endl;
        print_address_of_localvar(true, before_emp, 0);
        print_address_of_localvar(true, emp, 0);
        print_address_of_localvar(true, salary_per_yr_of_exp, 0);
        salary_per_yr_of_exp = emp.salary_per_yr_of_exp();
        std::string label = emp.label();
        std::cout << label << ": salary/yr: " << salary_per_yr_of_exp << std::endl;

        std::cout << "---- CEO ----" << std::endl;
        uint32_t before_ceo = 0;
        CEO ceo(100, 20, 200000);
        print_address_of_localvar(true, before_ceo, 0);
        print_address_of_localvar(true, ceo, 0);
        salary_per_yr_of_exp = ceo.salary_per_yr_of_exp();
        label = ceo.label();
        std::cout << label << ": salary/yr: " << salary_per_yr_of_exp << std::endl;
    } else if (command == "demo-late-poly") {
        uint32_t before_emp = 0;
        EmployeeDyn emp(1, 2, 100);
        int salary_per_yr_of_exp = 0;
        std::cout << "---- EmployeeDyn ----" << std::endl;
        print_address_of_localvar(true, before_emp, 0);
        print_address_of_localvar(true, emp, 0);
        print_address_of_localvar(true, salary_per_yr_of_exp, 0);
        salary_per_yr_of_exp = emp.salary_per_yr_of_exp();
        std::string label = emp.label();
        std::cout << label << ": salary/yr: " << salary_per_yr_of_exp << std::endl;

        std::cout << "---- CEODyn ----" << std::endl;
        uint32_t before_ceo = 0;
        CEODyn ceo(100, 20, 200000);
        EmployeeDyn *emp_ptr = &ceo;
        print_address_of_localvar(true, before_ceo, 0);
        print_address_of_localvar(true, *emp_ptr, 0);
        salary_per_yr_of_exp = emp_ptr->salary_per_yr_of_exp();
        label = emp_ptr->label();
        std::cout << label << ": salary/yr: " << salary_per_yr_of_exp << std::endl;
    } else if (command == "demo-try-catch") {
        try {
            test_throwing_func();
        } catch (const std::exception &err) {
            print_address_of_localvar(true, err, 0);
            std::cerr << "Caught exception: " << err.what() << std::endl;
            uint8_t *err_ptr = (uint8_t*)&err;
            const void *exc_ptr = *(void**)err_ptr;
            const uint32_t exc_selector = *(uint32_t*)((uint8_t*)exc_ptr + 8);
            std::cerr << "Exception pointer: " << std::hex << exc_ptr << std::endl;
            std::cerr << "Exception selector: " << exc_selector << std::endl;
        }
    } else if (command == "dump") {
        if (args.size() < 1) {
            show_usage("missing address");
        }
        const void *addr;
        if (args[0] == "prime_factors") {
            print_address_of_func(true, prime_factors, 0);
            addr = (void*)&prime_factors;
        } else if (args[0] == "print_prime_factors") {
            print_address_of_func(true, print_prime_factors, 0);
            print_address_of_func(true, prime_factors, 1);
            addr = (void*)&print_prime_factors;
        } else {
            addr = region_name_to_address(args[0]);
        }
        size_t size = 64;
        if (args.size() > 1) {
            size = std::stoul(args[1], nullptr, 0);
        }
        dump_memory(addr, size);
    } else {
        show_usage("invalid command");
    }
    return 0;
}

/// @return an array of prime factors of `num`.
uint32_t* prime_factors(uint32_t num) {
    uint32_t* factors;
    size_t factor_idx;

    factors = new uint32_t[num]; // inefficient, for demo only
    factor_idx = 0;
    for (uint32_t i = 2; i <= num; i++) {
        while (num % i == 0) {
            factors[factor_idx++] = i;
            num /= i;
        }
    }
    update_memory_layout();
    print_address_of_func(true, prime_factors, 2);
    print_address_of_param(true, num, 2);
    print_address_of_localvar(true, *factors, 2);
    print_address_of_localvar(true, factor_idx, 2);
    return factors;
}

/// @brief Print the prime factors of `num`.
void print_prime_factors(uint32_t num) {
    uint32_t *factors;
    print_address_of_func(true, print_prime_factors, 1);
    print_address_of_param(true, num, 1);
    factors = prime_factors(num);
    print_address_of_localvar(true, *factors, 1);
    std::cout << "Prime factors of " << num << ": ";
    size_t i;
    for (i = 0; factors[i] != 0; i++) {
        std::cout << factors[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl << "Memory dump of &[factors]: " << std::endl;
    dump_memory(factors, i * sizeof(uint32_t));
    delete[] factors;
}

/// @brief A function that throws an exception.
void throwing_func() {
    const std::runtime_error err = std::runtime_error("throwing_func");
    print_address_of_localvar(true, err, 2);
    throw err;
}

void test_throwing_func() {
    try {
        throwing_func();
    } catch (const std::exception &err) {
        print_address_of_localvar(true, err, 1);
        throw;
    }
}

/// @brief Show usage information about this utility and exit.
/// @param error an error message to show (if any)
void show_usage(const char *error) {
    using namespace std;

    const bool is_err = error != nullptr;
    ostream &outs = is_err ? cerr : cout;
    if (is_err) {
        outs << "error: " << error << endl;
    }
    outs << "memlens - a utility to print memory information" << endl;
    outs << "  repo: https://github.com/amodm/talks/tree/main/lecture-series-hwsw/lecture-4-programming-constructs" << endl;
    outs << endl;

    outs << "usage: memlens <command> [options]" << endl;
    outs << "where command is one of:" << endl;
    outs << "  help - show this help message" << endl;
    outs << "  prime [-p] - run prime factors function" << endl;
    outs << "  layout - show memory layout info" << endl;
    outs << "  demo-class - demo class data vs code layout" << endl;
    outs << "  demo-poly - demo polymorphism layout" << endl;
    outs << "  demo-late-poly - demo late-binding polymorphism layout" << endl;
    outs << "  demo-try-catch - demo try-catch flow" << endl;
    outs << "  dump <addr> [<size>] - dump memory starting at <addr>" << endl;
    outs << endl;

    exit (is_err ? 1 : 0);
}

/**************** Global Variables ****************/
// this is not a great way to do things, but we'll use it for
// simplicity in this example.

/// @brief the memory regions of the current process
std::vector<memory_region_t> MEMORY_REGIONS;
