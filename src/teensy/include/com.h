#include <cstddef>
#include "usb_serial.h"
#include "HardwareSerial.h"
#include <stdint.h>
#include <CRC.h>

#ifndef COM_LIB_H_
#define COM_LIB_H_

#define PISERIAL Serial1

// main logging class, just advanced print for debugging
class Logger {
  public:
    Logger(bool a) : active(a) {}

    void begin(long baud, bool blocking = true) {
      Serial.begin(baud);
      if (blocking) {
        while(!Serial);
      };
    }

    template<typename T>
    void send(const T& d, bool nl = false) {
        if (!active) return;

        if (nl) {
            Serial.println(d);
        } else {
            Serial.print(d);
        }
    }

    template<typename T>
    void info(const T& d, bool nl = true) {
        send("info: ");
        send(d, nl);
    }

    template<typename T>
    void warning(const T& d, bool nl = true) {
        send("warning: ");
        send(d, nl);
    }

    template<typename T>
    void error(const T& d, bool nl = true) {
        send("error: ");
        send(d, nl);
    }

  protected:
    bool active;
};

//initialise logger if true -> print out else skip 
Logger logging = Logger(true);

// comunication class to pi
class PISerial{
  public:
    PISerial() {};

    void begin(uint32_t baud) {PISERIAL.begin(baud);};

    // send a simple command with or without data
    void send(uint8_t type, uint8_t message, const uint8_t* d = nullptr, size_t len = 0) {
      PISERIAL.write(type);
      PISERIAL.write(message);
      if(d != nullptr && len > 0) {
        PISERIAL.write(len);
        PISERIAL.write(d, len);
      }
      PISERIAL.write(0);
      PISERIAL.flush();
    }
    
    // get a complex byte array back from PI
    size_t get_complex(uint8_t type, uint8_t message, uint8_t* buffer, size_t b_len, const uint8_t* d = nullptr, size_t d_len = 0) {
      uint16_t len;
      PISERIAL.clear();
      send(type, message, d, d_len); // send data
      while(PISERIAL.available()<1); // wait on responetype byte

      //check response
      if (PISERIAL.read() == com_config.return_data.complex_data) {
        len = readUint16(); // get length of expected data

        // check if length to recieve bigger than buffer
        if ((size_t)len>b_len) {
          logging.error("buffer not large enough or unexpected data u idiot data: ",false);
          logging.send(len);
          logging.send(" buffer:");
          logging.send(b_len);
          infinit_loop;
        };

        // wait for all data to arrive
        while(PISERIAL.available()<len);

        // read data to buffer
        size_t n = PISERIAL.readBytes(buffer,len);

        // calculate CCITT_FALSE crc16
        uint16_t cal_crc_16 = calcCRC16(
          buffer,
          len,
          CRC16_CCITT_FALSE_POLYNOME,
          CRC16_CCITT_FALSE_INITIAL,
          CRC16_CCITT_FALSE_XOR_OUT,
          CRC16_CCITT_FALSE_REV_IN,
          CRC16_CCITT_FALSE_REV_OUT
        );

        // get crc16 from serial
        uint16_t crc_16 = readUint16();

        logging.info(cal_crc_16);
        logging.info(crc_16);
        logging.info(len);

        check_terminator();
        
        // check crc validity
        if (cal_crc_16!=crc_16) return 0;

        return n;

      } else return 0;
    }

    // get simple 4 byte data
    int32_t get_simple(uint8_t type, uint8_t message, const uint8_t* d = nullptr, size_t d_len = 0) {
      PISERIAL.clear();
      send(type, message, d, d_len); // send data
      while(PISERIAL.available()<1); // wait on return type

      if (PISERIAL.read() == com_config.return_data.simple_data) {
        int32_t n = readint32(); // read int32

        check_terminator();

        return n;

      } else return 0;
    }

    // ping PI
    bool ping() {
      uint32_t n = millis()+4000;
      int b = -1;

      PISERIAL.clear();
      send(com_config.type.command,com_config.message.command.ping); // send ping request

      // read 4 seconds until ping accepted
      while (n>millis()) {
        b = PISERIAL.read();
        if(b==com_config.return_data.accept) {
          check_terminator();
          return true;
        }
      }

      //return false if ping denyed or timed out
      return false;
    }

    // block until ping to PI succeds 
    bool wait_for_ping(uint32_t timeout) {
      uint32_t t = millis()+timeout;

      bool suc = false;

      // wait timeout and retry pinging
      while (!suc and t>millis()){
        suc = ping();
      }

      // return true for ping succeded, false for timed out
      return suc;
    }
  
  protected:
    // check for null terminator
    void check_terminator() {
      // wait for byte
      while (PISERIAL.available() < 1);

      // read null terminator
      if (PISERIAL.read() != 0U){
        logging.warning("wrong terminator byte");
      }
    }

    // reading a uint16_t from serial 
    uint16_t readUint16() {
      while (PISERIAL.available() < 2) {
          // Wait until all bytes arrive
      }

      uint8_t b1  = PISERIAL.read();
      uint8_t b2 = PISERIAL.read();

      return (uint16_t)b1 | ((uint16_t)b2 << 8);
    }

    // reading a uint16_t from serial 
    uint64_t readint32() {
      while (PISERIAL.available() < 4) {
          // Wait until all bytes arrive
      }

      uint32_t value = 0;

      for (uint8_t i = 0; i < 4; i++) {
          value |= (uint32_t)(uint8_t)PISERIAL.read() << (8 * i);
      }

      return (int32_t)value;
    }
};

// initilise PISerial class
PISerial S_PI = PISerial();

#endif