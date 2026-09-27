#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t address, network, broadcast, first, last;
    unsigned prefix;
    uint64_t usable;
} Subnet;

static int component(const char **p, unsigned max, unsigned *out) {
    unsigned value = 0, digits = 0;
    while (**p >= '0' && **p <= '9') {
        unsigned d = (unsigned)(**p - '0');
        if (value > (max - d) / 10) return 0;
        value = value * 10 + d;
        ++*p; ++digits;
    }
    if (!digits || value > max) return 0;
    *out = value;
    return 1;
}

static int parse_cidr(const char *text, Subnet *s) {
    const char *p = text;
    unsigned octet, prefix;
    uint32_t address = 0;
    for (int i = 0; i < 4; ++i) {
        if (!component(&p, 255, &octet)) return 0;
        address = (address << 8) | octet;
        if (i < 3) { if (*p++ != '.') return 0; }
    }
    if (*p++ != '/' || !component(&p, 32, &prefix) || *p != '\0') return 0;
    uint32_t mask = prefix ? UINT32_MAX << (32 - prefix) : 0;
    s->address = address;
    s->prefix = prefix;
    s->network = address & mask;
    s->broadcast = s->network | ~mask;
    s->first = prefix >= 31 ? s->network : s->network + 1;
    s->last = prefix >= 31 ? s->broadcast : s->broadcast - 1;
    s->usable = prefix == 32 ? 1 : prefix == 31 ? 2 : (UINT64_C(1) << (32 - prefix)) - 2;
    return 1;
}

static void format_ip(uint32_t ip, char out[16]) {
    snprintf(out, 16, "%u.%u.%u.%u", (unsigned)(ip >> 24), (unsigned)((ip >> 16) & 255),
             (unsigned)((ip >> 8) & 255), (unsigned)(ip & 255));
}

#ifndef UNIT_TEST
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: subnet CIDR [CIDR ...]\n");
        return 2;
    }
    for (int i = 1; i < argc; ++i) {
        Subnet s;
        if (!parse_cidr(argv[i], &s)) {
            fprintf(stderr, "Invalid IPv4 CIDR: %s\n", argv[i]);
            return 2;
        }
    }
    puts("input,network,broadcast,first_usable,last_usable,usable_hosts");
    for (int i = 1; i < argc; ++i) {
        Subnet s;
        char network[16], broadcast[16], first[16], last[16];
        parse_cidr(argv[i], &s);
        format_ip(s.network, network); format_ip(s.broadcast, broadcast);
        format_ip(s.first, first); format_ip(s.last, last);
        printf("%s,%s,%s,%s,%s,%llu\n", argv[i], network, broadcast, first, last,
               (unsigned long long)s.usable);
    }
    return 0;
}
#endif
