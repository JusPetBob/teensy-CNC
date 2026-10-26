/*
*/

#define config_expected_length 35


struct CONFIG{

  struct hardware{
    uint8_t es_pin;
    bool e_stop;

    struct stepper{
      uint8_t en_pin;
      bool enable;

      struct x{
        uint8_t s_pin;
        uint8_t d_pin;
        bool dir;
        uint32_t steps;
      } x;

      struct y{
        uint8_t s_pin;
        uint8_t d_pin;
        bool dir;
        uint32_t steps;
      } y;

      struct z{
        uint8_t s_pin;
        uint8_t d_pin;
        bool dir;
        uint32_t steps;
      } z;
    } stepper;

    struct limit{

      struct x{
        uint8_t pin;
        bool dir;
      } x;

      struct y{
        uint8_t pin;
        bool dir;
      } y;

      struct z{
        uint8_t pin;
        bool dir;
      } z;
    } limit;
  } hardware;

  struct motion{
    uint16_t accelerartion;
    uint16_t max_speed;
  } motion;
};


bool load_config(uint8_t* buffer, size_t len, CONFIG* config) {
  if (buffer == nullptr || config == nullptr || len != config_expected_length) return false;
  if (
    !(buffer[1] <= 1) ||
    !(buffer[3] <= 1) ||
    !(buffer[6] <= 1) ||
    !(buffer[13] <= 1) ||
    !(buffer[20] <= 1) ||
    !(buffer[26] <= 1) ||
    !(buffer[28] <= 1) ||
    !(buffer[30] <= 1) 
  ) return false; // bool validation

  config->hardware.es_pin = buffer[0];
  config->hardware.e_stop = buffer[1];

  config->hardware.stepper.en_pin = buffer[2];
  config->hardware.stepper.enable = buffer[3];

  config->hardware.stepper.x.s_pin = buffer[4];
  config->hardware.stepper.x.d_pin = buffer[5];
  config->hardware.stepper.x.dir = buffer[6];
  config->hardware.stepper.x.steps = (uint32_t)buffer[7] | ((uint32_t)buffer[8] << 8) | ((uint32_t)buffer[9] << 16) | ((uint32_t)buffer[10] << 24);

  config->hardware.stepper.y.s_pin = buffer[11];
  config->hardware.stepper.y.d_pin = buffer[12];
  config->hardware.stepper.y.dir = buffer[13];
  config->hardware.stepper.y.steps = (uint32_t)buffer[14] | ((uint32_t)buffer[15] << 8) | ((uint32_t)buffer[16] << 16) | ((uint32_t)buffer[17] << 24);

  config->hardware.stepper.z.s_pin = buffer[18];
  config->hardware.stepper.z.d_pin = buffer[19];
  config->hardware.stepper.z.dir = buffer[20];
  config->hardware.stepper.z.steps = (uint32_t)buffer[21] | ((uint32_t)buffer[22] << 8) | ((uint32_t)buffer[23] << 16) | ((uint32_t)buffer[24] << 24);

  config->hardware.limit.x.pin = buffer[25];
  config->hardware.limit.x.dir = buffer[26];

  config->hardware.limit.y.pin = buffer[27];
  config->hardware.limit.y.dir = buffer[28];

  config->hardware.limit.z.pin = buffer[29];
  config->hardware.limit.z.dir = buffer[30];

  config->motion.accelerartion = (uint16_t)buffer[31] | ((uint16_t)buffer[32] << 8);
  config->motion.max_speed = (uint16_t)buffer[33] | ((uint16_t)buffer[34] << 8);

  return true;
};