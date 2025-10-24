# ft_ping - Test Suites

This document describes the automated test suites for the ft_ping project.

## Test Files

### 1. `test_argument_parsing.sh`
**Purpose:** Validates command-line argument parsing and flag handling

**Coverage:**
- Basic arguments (no args, help, verbose)
- Individual flags (-v, -c, -s, -i, -W, --ttl, -f)
- Invalid flag values (negative, zero, out of range)
- Unknown flags detection
- Flag combinations
- Destination formats (IP, hostname, decimal, hex, octal)
- Invalid destinations
- Special cases (missing values, multiple destinations)

**Statistics:**
- Total tests: 43
- Execution time: ~30 seconds
- Success rate: 100%

**Usage:**
```bash
sudo ./test_argument_parsing.sh
```

### 2. `test_icmp_messages.sh`
**Purpose:** Verifies ICMP message handling and error reporting

**Coverage:**
- Type 0 - Echo Reply (successful responses)
- Type 3 - Destination Unreachable (codes 0-4)
- Type 11 - Time Exceeded (TTL expired)
- Type 5 - Redirect
- Type 12 - Parameter Problem
- Request Timeout (no response)
- Special cases (multiple packets, large packets, fast intervals)

**Statistics:**
- Total tests: 22
- Execution time: ~40 seconds
- Success rate: 100%

**Usage:**
```bash
sudo ./test_icmp_messages.sh
```

## Running All Tests

To run both test suites sequentially:

```bash
sudo ./test_argument_parsing.sh && sudo ./test_icmp_messages.sh
```

## Test Results

Both test suites provide:
- ✅ **PASS** indicators for successful tests
- ❌ **FAIL** indicators with detailed error messages
- 📊 Final statistics summary
- 🎯 Success percentage

## Requirements

- Root privileges (sudo) required for RAW socket operations
- Network connectivity for remote host tests
- Compiled `ft_ping` binary in the same directory

## Notes

- Tests use `localhost` and public DNS servers (8.8.8.8, 1.1.1.1)
- Some tests may timeout on slow networks
- ICMP error tests depend on network configuration
- String vacío como destino
