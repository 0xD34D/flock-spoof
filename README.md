# flock-spoof

An ESP32 project for research and testing purposes that emulates Flock Safety and other surveillance devices via WiFi and BLE spoofing.

## What This Is

This project demonstrates how to spoof Flock Safety and other surveillance device characteristics on an ESP32 by emulating their WiFi MAC addresses, SSIDs, and BLE advertisements. Useful for security research and testing.

### Features

- **WiFi MAC Spoofing**: Broadcast with known Flock Safety device MAC prefixes (20+ variants)
- **WiFi SSID Mimicry**: Advertise SSID patterns including "Flock", "FS Ext Battery", "Penguin", and "Pigvision"
- **BLE Advertisement Spoofing**: Emulate surveillance device BLE advertisements
- **BLE UUID Patterns**: Advertise RAVEN surveillance device UUID patterns
- **Comprehensive Test Suite**: Test all spoofing combinations with configurable intervals

## Building

You'll need:
- ESP32-C3 or compatible ESP32 board
- ESP-IDF v5.x or later
- Basic familiarity with ESP-IDF tooling

### Build Steps

```bash
idf.py build
idf.py flash
idf.py monitor
```

## Notes

- Network routers may detect or flag unusual MAC/SSID combinations
- WiFi network managers may display unexpected device entries
- Regulatory compliance is your responsibility—verify legal status in your jurisdiction before use

## Legal Disclaimer

This code is for **educational and research purposes only**. Unauthorized spoofing of network devices or interference with communications networks may violate local regulations. Users are responsible for ensuring their use complies with applicable laws and regulations in their jurisdiction.

Based on research by [the EFF](https://www.eff.org/deeplinks/2024/04/defeat-flock) and the
[flock-you](https://github.com/colonelpanichacks/flock-you) project.

## License

See [LICENSE.md](LICENSE.md) for details.
