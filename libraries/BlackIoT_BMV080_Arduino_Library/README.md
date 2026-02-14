# Air Quality PM1/PM2.5/PM10 Sensor - BMV080 (Qwiic)


The BMV080 is an ultra-small, fan-less air quality sensor for sensing PM2.5 particulate matter! The sensing element measures merely 4.2mm x 3.5mm x 3.1mm (W x L x H), which is more than 450 times smaller than any comparable device on the market. The innovative design is based on ultra-compact lasers with integrated photodiodes. The sensor applies sophisticated algorithms to measure PM1 and PM2.5 concentrations directly in free space, without requiring an intrusive fan.

> [!NOTE]
> This library uses an pre-compiled archive library provided by Bosch, which is linked to this Arduino library and build time. 
> To use this library, the Bosch BMV080 SDK must be downloaded from Bosch, and some files copied over into this library once it's installed. Details of this are outlined below [here](#install-the-bosch-sdk).
>
> If the SDK files are not copied into this library, this library will fail to build.
>
> Bosch BMV080 SDK v11.2.0
> Arduino IDE 2.3.6

## Install the Bosch SDK

To use this library the Bosch SDK for the BMV080 must be installed and files copied into the correct locations within this library.

The following outlines how to install the library.

## Download the Bosch BMV080 SDK

The SDK is avilable [on this page](https://www.bosch-sensortec.com/products/environmental-sensors/particulate-matter-sensor/bmv080/#documents). Select the 'Download the SDK for BMV080` link/button.

Once downloaded, unzip the file.

## Copy Files

Files from the SDK must be copied into the arduino folder. This, is installed in the following locations:

### Library Install Directory

| OS | Directory|
|---|---|
|Linux| $HOME/.arduino15/packages/BlackIoT/hardware/esp32/X.Y.Z/libraries/BlackIoT_BMV080_Arduino_Library|
|Windows | $HOME\AppData\Local\Arduino15\packages\BlackIoT\hardware\esp32\X.Y.Z\libraries\BlackIoT_BMV080_Arduino_Library|
|macOS | $HOME/Library/Arduino15/packages/BlackIoT/hardware/esp32/X.Y.Z/libraries/BlackIoT_BMV080_Arduino_Library|

### Files to copy and locations to copy to

From the Bosch SDK, the following files are copied into the specified library locations.

|Bosch SDK File | SparkFun BMV080 Arduino Library Directory|
|--|--|
|api/inc/bmv080.h| src/bmv080.h|
|api/inc/bmv080_defs.h| src/bmv080_defs.h|
|api/api/lib/xtensa_esp32s3/xtensa_esp32s3_elf_gcc/release/lib_postProcessor.a | src/esp32s3/lib_postProcessor.a|
|api/api/lib/xtensa_esp32s3/xtensa_esp32s3_elf_gcc/release/lib_bmv080.a | src/esp32s3/lib_bmv080.a|

Once the files are copied into place, this library is available for use.

## License Information

This product is ***open source***!

This product is licensed using the [MIT Open Source License](https://opensource.org/license/mit).
