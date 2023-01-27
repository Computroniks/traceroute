// SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
// SPDX-License-Identifier: MIT

#include "traceroute/trace.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/icmp.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>


Trace::Trace(std::string dest, int max_hops) {
    dest_ = inet_addr(dest.c_str());
    max_hops_ = max_hops;
}

void Trace::Start() {
    sock_ = ConfigureSocket();

    while (ttl_ <= max_hops_) {
        SendProbe();
        /* Call recieve result */

        //if (/* packet is ECHO REPLY */) {
            //break;
        //}

        IncrementTTL();
    }
}

int Trace::ConfigureSocket() {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sock < 0) {
        std::cout << "Failed to create socket. ERRNO: " << errno << "\n";
        close(sock);
        FatalErr();
    }

    // Set initial TTL value
    if (setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl_, sizeof(ttl_)) < 0) {
        std::cout << "Failed to set initial TTL. ERRNO: " << errno << "\n";
        close(sock);
        FatalErr();
    }

    int one = 1;

    // Configure handling of ICMP error packets
    if (setsockopt(sock, IPPROTO_IP, IP_RECVERR, &one, sizeof(one)) < 0) {
        std::cout << "Failed to set IP_RECVERR flag. ERRNO: " << errno << "\n";
        close(sock);
        FatalErr();
    }

    // Set ICMP packet filter
    icmp_filter filter;
    filter.data = ~(
        (1 << ICMP_EXC_TTL)
        | (1 << ICMP_ECHOREPLY)
        | (1 << ICMP_DEST_UNREACH)
        | (1 << ICMP_TIME_EXCEEDED)
        );

    if (setsockopt(sock, IPPROTO_RAW, ICMP_FILTER, &filter, sizeof(filter)) < 0) {
        std::cout << "Failed to set ICMP_FILTER. ERRNO: " << errno << "\n";
        close(sock);
        FatalErr();
    }

    return sock;
}

uint32_t Trace::CalcChecksum(const uint16_t* buf, unsigned int len) {
    // Implemented using example from RFC1071 section 4.1

    uint32_t sum = 0;

    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }

    if (len > 0) {
        sum += *(unsigned char*)buf;
    }

    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}

void Trace::IncrementTTL() {
    ttl_++;
    if (setsockopt(sock_, IPPROTO_IP, IP_TTL, &ttl_, sizeof(ttl_)) < 0) {
        std::cout << "Failed to increment TTL. ERRNO: " << errno << "\n";
        FatalErr();
    }
}

void Trace::SendProbe() {
    // Configure target address
    sockaddr_in target;
    std::memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = dest_;

    // Configure ICMP packet
    char packet[sizeof(icmphdr)];
    std::memset(&packet, 0, sizeof(packet));

    icmphdr* icmp_packet = (icmphdr*)packet;
    icmp_packet->type = ICMP_ECHO;
    icmp_packet->code = 0;
    icmp_packet->checksum = 0;
    icmp_packet->un.echo.id = htons(rand());
    icmp_packet->un.echo.sequence = htons(rand());
    // Now we can calculate check sum
    icmp_packet->checksum = CalcChecksum((uint16_t*)&packet, sizeof(packet));

    int nbytes = sendto(
        sock_,
        &packet,
        sizeof(packet),
        0,
        (sockaddr*)&target,
        sizeof(target)
    );

    if (nbytes < 0) {
        std::cout << "Failed to send message to target. ERRNO: " << errno << "\n";
        FatalErr();
    }
}

void Trace::FatalErr() {
    std::cout << "Encountered unrecoverable error. Exiting. ERRNO: " << errno << "\n";
    close(sock_);
    exit(1);
}
