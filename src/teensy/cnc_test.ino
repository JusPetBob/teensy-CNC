/*
comms protocol:
see com_config.h


*/

#include "include/com_config.h"
#include "com.h"

#include "include/config.h"
#include "config.h"

const uint8_t sd_cs = 44;

const uint8_t motor_enable_pin = 38;

const uint8_t step_1_pin = 2;

const uint8_t dir_1_pin = 3;

void setup() {
  digitalWrite(LED_BUILTIN, HIGH);

  logging.begin(250000);

  logging.info("InIt started");

  S_PI.begin(250000);

  logging.warning(S_PI.get_simple(1, 4));

  logging.info("ping... ", false);

  uint32_t n = millis()+300000;
  bool suc = false;

  while (!suc and n>millis()){suc = S_PI.ping();}

  if(!suc) {
    logging.send("failed", true);
    logging.error("PI not reachable, reset required");
    while(true);
  }

  logging.send("success", true);

  logging.info("getting config");

  suc=false;
  while (!suc) {
    suc = get_config();
    if (!suc) {logging.warning("config parsing failed, retrying");}
  }

  pinMode(motor_enable_pin, OUTPUT);
  digitalWrite(motor_enable_pin, HIGH);
  
  pinMode(step_1_pin, OUTPUT);
  digitalWriteFast(step_1_pin, HIGH);

  pinMode(dir_1_pin, OUTPUT);
  digitalWriteFast(dir_1_pin, HIGH);


  Serial.println("init done");
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delayNanoseconds(10000);
  digitalWriteFast(step_1_pin, LOW);
  delayNanoseconds(10000);
  digitalWriteFast(step_1_pin, HIGH);
}
