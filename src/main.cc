// SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
// SPDX-License-Identifier: MIT

#include <iostream>
#include <string>

#include "traceroute/config.h"
#include "traceroute/trace.h"

int main(int argc, char* argv[]) {
    Config config(argc, argv);
    Trace route(config.dest, 60);
    route.Start();
    return 0;
}
