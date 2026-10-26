#include "arm_math.h"
#include <math.h>
#include <sys/types.h>
#include <stdint.h>


CONFIG config = {};

bool get_config() {
  uint8_t buffer[config_expected_length+4];

  size_t n = S_PI.get_complex(com_config.type.command,com_config.message.command.get_config,buffer,sizeof(buffer));

  logging.info("response size ",false);
  logging.send(n,true);

  return load_config(buffer,n,&config);
}