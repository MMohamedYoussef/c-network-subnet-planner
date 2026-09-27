# IPv4 subnet planner (C11)

Turn CIDR addresses into network ranges for a firewall rule, lab, or address plan. It accepts one or more IPv4 CIDRs and writes CSV to standard output. `fixtures/cidrs.txt` contains synthetic examples; `examples/report.csv` is the checked-in result for those three arguments in order.

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic main.c -o subnet
./subnet 192.168.10.77/26 10.0.0.0/31 203.0.113.9/32
cc -std=c11 -O2 -Wall -Wextra -pedantic -Wno-unused-function tests.c -o tests
./tests
```

The CSV columns are input, network, broadcast, first usable, last usable, and usable host count. A `/31` treats both addresses as usable point-to-point endpoints; `/32` represents one host. A `/0` has 4,294,967,294 usable addresses under the conventional network/broadcast rule. Input must be dotted decimal IPv4 with four octets and prefix 0–32; malformed input exits nonzero. The program validates all arguments before printing rows.

Sample row: `192.168.10.77/26,192.168.10.64,192.168.10.127,192.168.10.65,192.168.10.126,62`.
