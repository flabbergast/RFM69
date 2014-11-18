/// @dir radioBlip
/// Send out radio packets in set intervals; respond with an ACK to any received packet.
///  Useful pretty much only for testing (attiny84).
//
// Uses LowPowerLab's RFM69 library
// or the modified version by flabbergast which runs also on
//  attiny84 (eg. JeeNode Micro).
//
// Based on radioBlip from jeelib: https://github.com/jcw/jeelib
//  (2010-08-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php)

#define NODE_ID       22    //unique for each node on same network
#define GATEWAY_ID    1     // where to send the readings?
#define NETWORK_ID    108   //the same on all nodes that talk to each other
#define FREQUENCY     RF69_868MHZ
#define ENCRYPTKEY    "beleampanchineto" //exactly the same 16 characters/bytes on all nodes!
//#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define RF_TX_POWER 17 // 0-31, the Tx power is -18+value (max 13) dB

#define DELAY_BETWEEN_BLIPS 5000 // milliseconds

#include <RFM69_f.h>

RFM69 radio;
static long payload;
unsigned long last_blip;

void setup() {
#if defined(__AVR_ATtiny84__)
    // power up the radio on JMv3
    bitSet(DDRB, 0);
    bitClear(PORTB, 0);
#endif

    radio.initialize(FREQUENCY,NODE_ID,NETWORK_ID);
    radio.encrypt(ENCRYPTKEY);

    // adjust the TX output to save power
    radio.setPowerLevel(RF_TX_POWER);

    // sleep
    radio.sleep();
}

void loop() {
  ++payload;

  // send the payload
  radio.send(GATEWAY_ID, (const void*)(&payload), sizeof(payload), false); // last param = requestACK
  // sleep
  radio.sleep();

  // let's respond to packets by ACK between blips
  last_blip = millis();

  while( (millis() - last_blip) < DELAY_BETWEEN_BLIPS ) {
    if (radio.receiveDone()) {
      radio.sendACK();
    }
  }
}
