#define UNIT_TEST
#include "main.c"
#include <stdlib.h>
#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "CHECK failed at line %d: %s\n", __LINE__, #condition); exit(1); } } while (0)
#include <string.h>

int main(void) {
    Subnet s;
    char ip[16];
    CHECK(parse_cidr("192.168.10.77/26", &s));
    format_ip(s.network, ip); CHECK(strcmp(ip, "192.168.10.64") == 0);
    format_ip(s.broadcast, ip); CHECK(strcmp(ip, "192.168.10.127") == 0);
    CHECK(s.usable == 62);
    CHECK(parse_cidr("10.0.0.0/31", &s) && s.usable == 2);
    CHECK(parse_cidr("10.0.0.9/32", &s) && s.usable == 1);
    CHECK(!parse_cidr("10.0.0.256/24", &s));
    CHECK(!parse_cidr("10.0.0.1/33", &s));
    CHECK(!parse_cidr("10.0.0.1/24junk", &s));
    puts("subnet tests passed");
    return 0;
}
