/* Edge Impulse Arduino examples
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/*
 * replace with your LoraWan library
 */
//#include <SomeLoraSatLibrary.h>

// Minimal interval between satellite transmissions and number of messages
#define TX_INTERVAL 3

// expected data len from off-chip device
#define RX_DATA_LEN  12
// data packet len sent to TTN (also via Sat gateway)
#define TTN_PACKET_DATA_LEN (RX_DATA_LEN+1)

// byte   0 :  satellite             [ bool  ]
// byte 1-4 :  housesparrow         [ float ]
// byte 5-8 :  noise                [ float ]
// byte 9-12:  rose ringed parakeet [ float ]
static char rxData[TTN_PACKET_DATA_LEN];
static int payloadLength;

// Replace with your keys and device address
static byte networkKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static byte appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static byte deviceAddress[] = { 0x00, 0x00, 0x00, 0x00 };

int lora_wan_setup(/*some lora wan parameters*/);
int lora_sat_comms_setup(/*some satellite parameters*/);

void setup() {

  /*
   * setup some pins (e.g. leds)
   */
  // pinmode(..);

  // setup serial comms with off-chip device running ML model
  Serial3.begin(115200);

  Serial.begin(115200);

  while (!(Serial && Serial3) && (millis() < 3000))
    ;

  Serial.println("Edge Impulse's Simple LoRa (via Satellite) Example\n\n");
  Serial.println("Initializing");

  // setup LoRa comms
  // for you're region
  // EU868, US915 AS923, IN865, ..
  int result = lora_wan_setup(/*some lora wan parameters*/);
  result = lora_sat_comms_setup(/*some satellite parameters*/);
}

int wait_on_audio_trigger()
{
    Serial3.flush();

    int j = 1;
    while (j < TTN_PACKET_DATA_LEN) {
      if ((Serial3.available() > 0)) {
        rxData[j++] = Serial3.read();
      }
    }
    // bytes read
    return j-1;
}

void loop() {
  int result;

  // It's probably good to have some visual indicator
  // every cycle
  // toggle_leds();

  Serial.println("Waiting on audio trigger..");
  payloadLength = wait_on_audio_trigger();

  if (payloadLength != RX_DATA_LEN) {
    Serial.print("Insufficient data received: ");
    Serial.println(payloadLength);
  } else {

    payloadLength = TTN_PACKET_DATA_LEN;

    rxData[0] = false;

    Serial.println("Sending LoRa message");

    /*
     * replace with your library function
     */
    // result = lora_wan_send(/* some params & payload */);
    // if (result) { /* error handling */}

    // set `satellite` field to `true`.
    // This way we can discern which messages pass through the satellite.
    rxData[0] = true;

    Serial.println("Sending LoRa message via satellite");

    /*
     * replace with your library function
     */
    // result = lora_sat_send(/* some params & payload */);
    // if (result) { /* error handling */}
  }

  // Using `delay()` for this example.
  // But it's better to put the device in lower power state.
  Serial.println("Sleeping..");
  delay(TX_INTERVAL*1000);
  Serial.println("Awake");
}

int lora_wan_setup(/*some lora wan parameters*/)
{
  int result;

  /*
  ** Place your setup here
  */

  return result;
}

int lora_sat_comms_setup(/*some satellite parameters*/)
{
  int result;

  /*
  ** Place your setup here
  */

  return result;
}
