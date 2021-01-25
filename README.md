# Edge Impulse's Simple Example for Lacuna Space's LS200

This example firmware sends [LoRa](https://lora-alliance.org/) messages to a
[The Things Network](https://www.thethingsnetwork.org/) application via both
earth-bound LoRa gateways and [Lacuna Space's](https://lacuna.space/) satellite.

The message sent are the classification results of the [Bird songs model](https://studio.edgeimpulse.com/studio/16060)
running on a Arduino Nano 33 BLE Sense. The model classifies audio as either from the **house sparrow**, **rose-ringed parakeet** or as **background noise**.
Only when either bird has been classified with a result greater than `0.8` will the Nano 33 BLE sense transmit the results to the LS200.

The LS200 then adds the `satellite` field and sets it to `true`. This enables us
to determine which messages pass through the satellite.


## Requirements

- Lacuna Space LS200 development kit
- Arduino Nano 33 BLE Sense
- (Optional) The Things Network Indoor Gateway

Get in touch with [Lacuna Space](https://lacuna.space/get-involved/) for hardware and software setup.

## Gettings Started

## Things Network Application and Device

1. Follow [these steps](https://www.thethingsnetwork.org/docs/applications/add.html) to create a TTN application.

1. Add a device by following [these steps](https://www.thethingsnetwork.org/docs/devices/registration.html).
   Also, choose `ABP` as Activation Method.

1. Disable `Frame Counter Checks` for your device (`Device > Settings`).
   
## Add a Data Storage Integration

1. Add a [Data Storage
Integration](https://www.thethingsnetwork.org/docs/applications/integrations.html#add-an-integration)
to the application to store data for the last 7 days.

1. Add a decoder payload function 

In TTN console go to `Applications > Payload Format > decoder` and paste the
contents of `ttn-payload-function.js` into the text field and save the payload functions.

## Add `deviceAddress`, `networkKey` and `appKey`

From the TTN console copy (`<> msb`):

- Device Address
- Network Session Key
- App Session Key

and add it to `deviceAddress`, `networkKey` and `appKey` respectively in `example-lacuna-ls200.ino`.

## Building and Flashing the applications

### LS200 application

1. Build the application:

    ```
    ./arduino-build.sh --build
    ```

1. Flash the application:

    ```
    ./arduino-build.sh --flash
    ```

### Arduino Nano 33 BLE Sense firmware

1. Clone the Edge Impulse Inc.'s [housesparrow-roseringedparakeet-noise-100 model](https://studio.edgeimpulse.com/studio/16060) from [Edge Impulse Studio](https://studio.edgeimpule.com)

1. Follow [running your impulse arduino](https://docs.edgeimpulse.com/docs/running-your-impulse-arduino) guide to export the model as an Arduino library.

1. Create an sketch from the `nano_ble33_sense_microphone_continuous` example.

1. Copy the contents of `nano_ble33_sense_microphone_continuous/nano_ble33_sense_microphone_continuous.ino` (found in this repo) into your sketch.

1. Choose the right board, build and flash using Arduino IDE.

### Troubleshooting

* Not flashing? You can double tap the button on the board to put it in bootloader mode.

## Board to Board Interface

- Nano __(TX)__ -> __(RX)__ LS200
- TX on pin `D4`.
- RX on pin 5 on `J4`.
- UART (115200 8N1)

## Running the application

1. Use a [Tracker](https://www.n2yo.com/) or [Lacuna Space's Dashboard](https://dashboard.lacuna.space) to determine the next good passing of satellite.
    
    Note: Only run this application when the satellite is in range.

1. Reset the LS200. Note via the debug serial that the LS200 displays `Waiting for audio trigger..`.

    Note: ensure LS200 is always resetted after powering the Nano to mitigate synchronization and data corruption issues.

1. Now _just_ wait for the bird songs to be detected.

1. Notice that the message is received in your TTN application (`Application > Data`). The data counter is an even number and the `satellite` field is set to `false`.

E.g:
``` json
    {
        "housesparrow": "0.91406",
        "redringedparakeet": "0.05078",
        "noise": "0.03125",
        "satellite": false,
    }
```

1. Confirm via the serial output that LS200 the sent the message via the satellite. The message should arrive moments later (~6 minutes) in your TTN application with an even counter value and the `satellite` field being `true`.

E.g:

``` json
    {
        "housesparrow": "0.91406",
        "redringedparakeet": "0.05078",
        "noise": "0.03125",
        "satellite": true,
    }
```


## Fair Access Policy

Consider complying to the [Fair Access Policy](https://www.thethingsnetwork.org/docs/lorawan/duty-cycle.html)
and use tools like [Airtime Calculator](https://avbentem.github.io/airtime-calculator/ttn) (`e.g. SF8BW125`) to determine the maximum amount of LoRa messages per 24 hours that can be sent in you're region.

## (Optional) TTN Indoor Gateway

Consider using a Lora Gateway if there are few in your area. See [this map](https://www.thethingsnetwork.org/map) to see gateways near you.
