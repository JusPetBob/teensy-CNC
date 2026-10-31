#define infinit_loop while(true)

#include <Arduino.h>

#include "com_config_gegerated.h"
#include "com.h"

#include "config_gegerated.h"
#include "config.h"

void setup() {
  digitalWrite(LED_BUILTIN, HIGH);

  logging.begin(115200);

  logging.info("InIt started");

  S_PI.begin(250000);

  logging.info("ping... ", false);


  if(!S_PI.wait_for_ping(300000)) {
    logging.send("failed", true);
    logging.error("PI not reachable, reset required");
    infinit_loop;
  }

  logging.send("success", true);

  logging.info("getting config... ", false);

  if (!config.get_config(10)) {
    logging.send("failed", true);
    logging.error("PI did not return a valid config, reset required");
    infinit_loop;
  }

  logging.send("success", true);

  logging.info("applying config");
  config.setup_config();


  Serial.println("init done");
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delayNanoseconds(10000);
  digitalWriteFast(config.config.hardware.stepper.x.s_pin, LOW);
  delayNanoseconds(10000);
  digitalWriteFast(config.config.hardware.stepper.x.s_pin, HIGH);
}
