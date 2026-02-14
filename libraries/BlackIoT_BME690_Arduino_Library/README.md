# Digital low power temperature, humidity, pressure and gas sensor with AI - BME690


The BME690 is the gas sensor with Artificial Intelligence (AI) and integrated high-linearity and high-accuracy
pressure, humidity and temperature sensors

> [!NOTE]
> This library uses an pre-compiled archive library provided by Bosch, which is linked to this Arduino library and build time. 
> To use this library, the Bosch BSEC SDK must be downloaded from Bosch, and some files copied over into this library once it's installed. Details of this are outlined below [here](#install-the-bosch-sdk).
>
> If the SDK files are not copied into this library, this library will fail to build.
>
> Bosch bsec_v3-2-0-0
> Arduino IDE 2.3.6

## Install the Bosch BSEC SDK

To use this library the Bosch BSEC SDK for the BME690 must be installed and files copied into the correct locations within this library.

The following outlines how to install the library.

## Download the Bosch BSEC SDK

The BSEC SDK is avilable [on this page](https://www.bosch-sensortec.com/zh/software-tools/software/bme688-and-bme690-software/). Select the 'Download BSEC` link/button.

Once downloaded, unzip the file.

## Copy Files

Files from the BSEC SDK must be copied into the arduino folder. This, is installed in the following locations:

### Library Install Directory

| OS | Directory|
|---|---|
|Linux| $HOME/.arduino15/packages/BlackIoT/hardware/esp32/X.Y.Z/libraries/BlackIoT_BME690_Arduino_Library|
|Windows | $HOME\AppData\Local\Arduino15\packages\BlackIoT\hardware\esp32\X.Y.Z\libraries\BlackIoT_BME690_Arduino_Library|
|macOS | $HOME/Library/Arduino15/packages/BlackIoT/hardware/esp32/X.Y.Z/libraries/BlackIoT_BME690_Arduino_Library|

### Files to copy and locations to copy to

From the Bosch BSEC SDK, the following files are copied into the specified library locations.

|Bosch BSEC SDK File | BlackIoT_BME690_Arduino_Library Directory|
|--|--|
|algo/bsec_IAQ_Sel/inc/bsec_datatypes.h| src/inc/bsec_datatypes.h|
|algo/bsec_IAQ_Sel/inc/bsec_interface.h| src/inc/bsec_interface.h|
|algo/bsec_IAQ_Sel/bin/esp/esp32_s3/libalgobsec.a | src/esp32s3/libalgobsec.a|
|algo/bsec_IAQ_Sel/bin/esp/esp32_s3/libalgobsec.a.Size.log | src/esp32s3/libalgobsec.a.Size.log|

Once the files are copied into place, this library is available for use.

## License Information

This product is ***open source***!

This product is licensed using the [MIT Open Source License](https://opensource.org/license/mit).
