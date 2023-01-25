// SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
// SPDX-License-Identifier: MIT

#include <string>

#ifndef TRACEROUTE_CONFIG_H_
#define TRACEROUTE_CONFIG_H_

class Config
{
private:
    void parseCLI(int argc, char *argv[]);

public:
    std::string host;
    Config(int argc, char *argv[]);
};

#endif