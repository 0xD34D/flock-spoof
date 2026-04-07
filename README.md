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
- ESP32 board (C3, S3, or compatible variants like C3-Zero, S3-Zero)
- PlatformIO (VS Code extension or CLI)
- ESP-IDF v5.x (automatically managed by PlatformIO)

### Build Steps

```bash
pio run
pio run -t upload
pio device monitor
```

For specific boards, use the environment:
- ESP32-C3: `pio run -e esp32-c3-devkitm-1`
- ESP32-S3: `pio run -e esp32-s3-devkitc-1`
- ESP32-C3-Zero: `pio run -e esp32-c3-zero`
- ESP32-S3-Zero: `pio run -e esp32-s3-zero`

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
