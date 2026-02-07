# Polverine - Arduino IDE Board Package

Custom Arduino board package for the **Polverine** environmental sensing board by [BlackIoT sagl](https://blackiot.swiss).

## Board Overview

The Polverine is a mikroBUS-compatible (25.4 x 28.6 mm) environmental monitoring board featuring:

- **MCU:** ESP32-S3-MINI-1 — Dual-core Xtensa LX7 @ 240 MHz, 8 MB Flash, 0.5 MB PSRAM
- **Connectivity:** Wi-Fi 802.11 b/g/n, Bluetooth 5 LE, USB-C
- **Sensors:**
  - **BMV080** — Bosch PM sensor (PM1, PM2.5, PM10) via SPI
  - **BME690** — Bosch environmental sensor (temperature, humidity, pressure, VOC) via I2C
- **Interfaces:** SPI, I2C, UART, JTAG, GPIO/PWM, analog, Qwiic connector
- **User I/O:** RGB LED (Red=GPIO47, Green=GPIO48, Blue=GPIO38), user button (GPIO0), reset button

## Prerequisites

- [Arduino IDE 2.x](https://www.arduino.cc/en/software) installed
- USB-C cable

## Installation

### Step 1 — Install the ESP32 Core

The Polverine board package depends on the official ESP32 Arduino core by Espressif.

1. Open Arduino IDE
2. Go to **Tools → Board → Boards Manager**
3. Search for **esp32**
4. Install **esp32 by Espressif Systems** (version 3.x or later, IDF v5.5 based)
5. Wait for the installation to complete (this may take several minutes)

### Step 2 — Add the BlackIoT Board Manager URL

1. Go to **File → Preferences** (or **Arduino IDE → Settings** on macOS)
2. In the **Additional Boards Manager URLs** field, add:

   ```
   https://raw.githubusercontent.com/BlackIoT/Firmware_Polverine_Arduino/main/package_blackiot_index.json
   ```

   > If you already have other URLs there, separate them with a comma.

3. Click **OK**

### Step 3 — Install the BlackIoT Board Package

1. Go to **Tools → Board → Boards Manager**
2. Search for **BlackIoT**
3. Install **BlackIoT Boards** (latest version)
4. Wait for the installation to complete

### Step 4 — Select the Polverine Board

1. Go to **Tools → Board → BlackIoT → Polverine**
2. Configure board settings under **Tools** menu as needed:

   | Setting | Recommended Default |
   |---|---|
   | CPU Frequency | 240 MHz (WiFi) |
   | Flash Mode | QIO 80 MHz |
   | Flash Size | 8 MB |
   | Partition Scheme | 8M with spiffs (3MB APP/1.5MB SPIFFS) |
   | Upload Speed | 921600 |
   | Upload Mode | UART0 / Hardware CDC |
   | USB Mode | Hardware CDC and JTAG |
   | USB CDC On Boot | Disabled |
   | JTAG Adapter | Disabled |
   | Core Debug Level | None |

### Step 5 — Connect and Select the Port

1. Connect the Polverine board to your computer via USB-C
2. Go to **Tools → Port** and select the corresponding serial port
   - **Linux:** typically `/dev/ttyACM0` or `/dev/ttyUSB0`
   - **Windows:** `COM3` or similar (check Device Manager)
   - **macOS:** `/dev/cu.usbmodem*`

> **Linux users:** If the port is not accessible, add your user to the `dialout` group:
> ```bash
> sudo usermod -aG dialout $USER
> ```
> Then log out and log back in.

## Testing the Installation

### Blink Example

This board package includes a **Blink_Polverine** example that cycles through the RGB LED.

1. Go to **File → Examples → Blink_Polverine → Blink_Polverine**
2. Click **Upload** (or press `Ctrl+U`)
3. The RGB LED should cycle: Red → Green → Blue, each for 1 second

### Minimal Test Sketch

```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Polverine is alive!");

  // Initialize RGB LED pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
}

void loop() {
  // Red
  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);

  // Green
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);

  // Blue
  digitalWrite(BLUE_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(BLUE_LED_PIN, LOW);
}
```

## Pin Reference

### MikroBUS Connector

| Function | GPIO |
|---|---|
| TX | 17 |
| RX | 18 |
| SDA | 8 |
| SCL | 9 |
| SS | 34 |
| MOSI | 35 |
| MISO | 37 |
| SCK | 36 |
| PWR | 2 |
| INT | 7 |
| AN | 1 |

### On-Board Sensors

| Sensor | Interface | Pins |
|---|---|---|
| BMV080 (PM) | SPI | CS=10, MOSI=11, MISO=13, SCK=12, INT=5 |
| BME690 (Env) | I2C | SDA=14, SCL=21 |

### RGB LED

| Color | GPIO |
|---|---|
| Red | 47 |
| Green | 48 |
| Blue | 38 |

### JTAG (J3 Header)

| Signal | GPIO |
|---|---|
| TMS | 42 |
| TDI | 41 |
| TDO | 40 |
| TCK | 39 |

## Bosch Sensor SDKs

To use the BMV080 and BME690 sensors, you need to download the official SDKs from Bosch Sensortec (license acceptance required):

- **BMV080 SDK:** [Bosch Sensortec BMV080](https://www.bosch-sensortec.com/products/environmental-sensors/particulate-matter-sensor/bmv080/)
- **BME690 SDK:** [Bosch Sensortec BME690](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme690/)

For complete demo projects including MQTT, BLE, and Home Assistant integration, see the main repository:
[BlackIoT/Polverine](https://github.com/BlackIoT/Polverine)

## Troubleshooting

### Board not detected (no serial port)

- Try a different USB-C cable (make sure it supports data, not charge-only)
- On Linux, check permissions: `ls -l /dev/ttyACM0` and ensure your user is in the `dialout` group
- On Windows, check Device Manager for the COM port

### Upload fails

- Make sure the correct port and board are selected
- Try lowering the upload speed to 115200 under **Tools → Upload Speed**
- Enter bootloader mode manually:
  1. Press and hold **SW1/BOOT** (GPIO0)
  2. Press and release **SW2/EN** (reset)
  3. Release **SW1/BOOT**
  4. Retry the upload

### Board resets periodically

Enter bootloader mode using the sequence above and re-flash your firmware.

### Erase flash (factory reset)

Using `esptool.py` or `arduino-cli`:

```bash
esptool.py --chip esp32s3 --port /dev/ttyACM0 --baud 921600 erase_flash
```

## License

See [LICENSE](https://github.com/BlackIoT/Firmware_Polverine_Arduino/blob/main/LICENSE) for details.

## Resources

- [BlackIoT Website](https://blackiot.swiss)
- [Polverine Product Page](https://www.blackiot.swiss/polverine)
- [Polverine FAQ](https://www.blackiot.swiss/polverine_faq)
- [Main Polverine Repository (PlatformIO demos)](https://github.com/BlackIoT/Polverine)
- [Arduino Board Package Repository](https://github.com/BlackIoT/Firmware_Polverine_Arduino)
