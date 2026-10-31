#include "arm_math.h"
#include <math.h>
#include <sys/types.h>
#include <stdint.h>

#ifndef CON_M_LIB_H_
#define CON_M_LIB_H_

class Config {
  public:
    Config(){config = {};}

    // get config from pi and write to "CONFIG config"
    bool get_config(uint8_t retrys) {
      uint8_t t = 0;
      bool suc = false;

      // getting config with 1-255 retrys
      while (t<retrys and !suc) {
        uint8_t buffer[config_expected_length+4];
        
        // get config_buffer from complex data
        size_t n = S_PI.get_complex(com_config.type.command,com_config.message.command.get_config,buffer,sizeof(buffer));

        logging.info("response size ",false);
        logging.send(n,true);

        // parse buffer to "CONFIG config"
        suc = load_config(buffer,n,&config);
        if (!suc) {
          logging.warning("config parsing failed, retrying");
        }
        // increment trys
        t++;
      }
      return suc;
    }

    // configure pins 
    void setup_config() {
      //estop pin 
      pinMode(config.hardware.es_pin,INPUT);

      // enable pin + disable stepper
      pinMode(config.hardware.stepper.en_pin,OUTPUT);
      digitalWrite(config.hardware.stepper.en_pin, !config.hardware.stepper.enable);

      // steppers
      setup_stepper(&config.hardware.stepper.x); // x
      setup_stepper(&config.hardware.stepper.y); // y
      setup_stepper(&config.hardware.stepper.z); // z

      // limits
      setup_limits(&config.hardware.limit.x); // x
      setup_limits(&config.hardware.limit.y); // y
      setup_limits(&config.hardware.limit.z); // z
    }

    CONFIG config;
    protected:
    void setup_stepper(CONFIG::hardware::stepper::Steppers* st){
      pinMode(st->s_pin,OUTPUT); // setup step pin
      digitalWrite(st->s_pin,LOW);

      pinMode(st->d_pin,OUTPUT); // setup dir pin
      digitalWrite(st->d_pin,st->dir);
    }
    void setup_limits(CONFIG::hardware::limit::Switches* st){
      pinMode(st->pin,INPUT); // setup limit switches
    }
};

Config config = Config();

#endif