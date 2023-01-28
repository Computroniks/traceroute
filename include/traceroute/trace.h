// SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
// SPDX-License-Identifier: MIT

#include <netinet/ip.h>

#include <string>

#ifndef TRACEROUTE_TRACE_H_
#define TRACEROUTE_TRACE_H_

struct Response {
    // IP Address of last router
    std::string ip;

    // Was this response from the target?
    bool is_target;
};

class Trace {
private:
    // Target to trace to
    in_addr_t dest_;

    // Current TTL to send
    int ttl_ = 1;

    // Maximum number of hops before quitting
    int max_hops_;

    // Connection socket to use
    int sock_;

    // Configure a socket for sending and reciecing ICMP packets
    // Returns a file descriptor for the socket.
    int ConfigureSocket();

    // Calculate the internet checksum acording to RFC1071
    uint32_t CalcChecksum(const uint16_t* buf, unsigned int len);

    // Wait for response from probe. Note: This is blocking
    Response RecieveProbe();

    // Increment the TTL and update socket accordingly
    void IncrementTTL();

    // Send ICMP ECHO REQUEST packet to target
    void SendProbe();

    // Cleanup everything and exit program
    void FatalErr();

    // Convert IP address to a string
    std::string IPToString(int addr);

public:
    Trace(std::string dest, int max_hops);

    // Start the trace
    void Start();
};

#endif
