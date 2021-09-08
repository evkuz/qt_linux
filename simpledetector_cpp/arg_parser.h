#include "getopt.h"
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <string>
#include <iomanip>


#define REQUIRED      true
#define NOT_REQUIRED  false

namespace Miksarus{

  struct Option {
      const char* long_name;      // long option name
      int short_name;             // char option name
      int has_arg;                // has_arg (see 'getopt.h')
      const char* description;    // description (will be shown in help)
      // External function for setting value of option
      std::function<void(char *)> set_value;
      // External function to check if value was set
      std::function<void()> check_value; 
  };


  struct ProgramOption {
      Option argument;          // definition of argument
      bool isRequired;            // is required argument
  };


  static bool verbose = false;
  Option optVerbose = {
    "verbose", 'v',
    no_argument,
    "verbose output",
    [](char* argv) { verbose = true; }, 
    []()           {}
  };

  Option optNULL = {nullptr, 0, 0, nullptr, nullptr, nullptr};

  std::string exampleValue = "default example value";
  Option optExample  = {
    "example", 'e',
    required_argument,
    "example option",
    [](char* argv) { exampleValue = std::string(argv); }, 
    []()           { if (exampleValue.length() == 0) {printf("argument for --example is required"); exit(-1);} }
  };


  void ShowUsage(const ProgramOption *options_list, const char *name) 
  {
    const int longopt_maxlen = 20; // Maximum number of characters in long option

    // Print an option
    auto printopt = [](const ProgramOption* opt) {
      std::cout << "\t"
        << "-" << (char) opt->argument.short_name << ", "
        << "--" << std::left << std::setw(longopt_maxlen)<< opt->argument.long_name
        << opt->argument.description
        << std::endl;
    };

    std::cout << "\n";
    std::cout << "Usage: " << name << " [ options ]" << std::endl << std::endl;

    // Required options first
    std::cout << "Required options: " << std::endl;
    auto req_options = options_list;
    while (req_options->argument.long_name != nullptr) {
      if (req_options->isRequired)
        printopt(req_options);
      ++req_options;
    }
    std::cout << std::endl;

    // Now, additional options
    std::cout << "Other options: " << std::endl;
    // First, help and config
    std::cout << "\t"
      << "-h, --" << std::left << std::setw(longopt_maxlen) << "help" 
      << "print this message" << std::endl;
    auto other_options = options_list;
    while (other_options->argument.long_name != nullptr) {
      if (!other_options->isRequired)
        printopt(other_options);
      ++other_options;
    }
    std::cout << std::endl;
  }

  void ParseProgramOptions(int argc, char **argv, const ProgramOption *options_list) {
    // Initialize Options
    char short_options[30];
    char *short_options_pointer = short_options;

    struct option long_options[30];
    struct option *long_options_pointer = long_options;

    const ProgramOption *options_pointer = options_list;
    while (options_pointer->argument.long_name != nullptr) {
      *short_options_pointer =
          static_cast<char>(options_pointer->argument.short_name);
      short_options_pointer++;
      if (options_pointer->argument.has_arg != no_argument) {
        *short_options_pointer = ':';
        short_options_pointer++;
      }

      long_options_pointer->name = options_pointer->argument.long_name;
      long_options_pointer->has_arg = options_pointer->argument.has_arg;
      long_options_pointer->flag = nullptr;
      long_options_pointer->val = options_pointer->argument.short_name;
      ++long_options_pointer;

      ++options_pointer;
    }

    // Add help option
    *short_options_pointer = 'h';
    short_options_pointer++;
    *long_options_pointer = {"help", no_argument, nullptr, 'h'};
    long_options_pointer++;

    // Close options lists
    *short_options_pointer = '\0';
    *long_options_pointer = {nullptr, 0, nullptr, 0};

    int c = -1;
    while (1) {
      c = getopt_long(argc, argv, short_options, long_options, 0);
      /* Detect the end of the options. */
      if (c == -1)
        break;

      options_pointer = options_list;
      while (options_pointer->argument.long_name != nullptr) {
        if (options_pointer->argument.short_name == c) {
          options_pointer->argument.set_value(optarg);
          break;        
        }
        ++options_pointer;
      }
    
      // If --help option
      if (c == 'h') {
        ShowUsage(options_list, argv[0]);
        exit(0);
      }

      // If undefined option or not defined option argument
      if (c == ':' || c == '?') {
        ShowUsage(options_list, argv[0]);
        exit(2);
      }
    }
  }

  void CheckProgramOptions(const ProgramOption *options_list)
  {
    while (options_list->argument.long_name != nullptr) {
      if(options_list->isRequired){
        if(options_list->argument.check_value != nullptr){
          options_list->argument.check_value();
        }
      }
      ++options_list;
    }
  }

};