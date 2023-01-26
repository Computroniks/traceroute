// SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
// SPDX-License-Identifier: MIT

#include <iostream>
#include <string>

#include "traceroute/config.h"

int main(int argc, char* argv[]) {
    Config config(argc, argv);
    std::cout << config.host << "\n";
    return 0;
}
