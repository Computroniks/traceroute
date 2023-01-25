// SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
// SPDX-License-Identifier: MIT

#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include "traceroute/Config.h"

namespace po = boost::program_options;

Config::Config(int argc, char *argv[])
{
    parseCLI(argc, argv);
}

void Config::parseCLI(int argc, char *argv[])
{

    // Options
    po::options_description opt_desc("Usage: traceroute [options] [host] ...\nOptions");
    opt_desc.add_options()("help", "Show this help message")("host", po::value<std::string>()->required(), "Host to trace route to");

    // Positional argumetns
    po::positional_options_description pos_desc;
    pos_desc.add("host", -1);

    po::variables_map vm;

    try
    {
        po::store(po::command_line_parser(argc, argv).options(opt_desc).positional(pos_desc).run(), vm);

        // Handle help before notifying to avoid error where host is
        // required but not deffined
        if (vm.count("help"))
        {
            std::cout << opt_desc;
            exit(0);
        }

        po::notify(vm);
    }
    catch (po::error &e)
    {
        std::cout << "ERROR: " << e.what() << "\n";
        std::cout << opt_desc;
        exit(1);
    }

    host = vm["host"].as<std::string>();
}