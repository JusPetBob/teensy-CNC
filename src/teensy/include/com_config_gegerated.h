/*
comms protocol:

1 byte type [command|info|warning|error]
1 byte message table below
(2 bytes length of extra data)
(up to 2^16-1 bytes for extra data)

message table:

---- command ----
1 ping
2 get config

---- info ----

---- warning ----

---- error ----



return coms protocol:
1 accept
2 deny
3 simple_data (followed by 4 byte of data)
4 complex_data (followed by 2 bytes length of extra data and up to 2^16-1 bytes for extra data and a 2 byte CRC-16/CCITT-FALSE)

*/

#ifndef COM_CONFIG_LIB_H_
#define COM_CONFIG_LIB_H_

struct COM_CONFIG{
  struct type{
    uint8_t command;
    uint8_t info;
    uint8_t warning;
    uint8_t error;
  } type;
  struct message{
    struct command{
      uint8_t ping;
      uint8_t get_config;
    } command;
    struct info{
    } info;
    struct warning{
    } warning;
    struct error{
    } error;
  } message;
  struct return_data{
    uint8_t accept;
    uint8_t deny;
    uint8_t simple_data;
    uint8_t complex_data;
  } return_data;
};


constexpr COM_CONFIG com_config = {
  {
     1,
     2,
     3,
     4
  },
  {
    {
       1,
       2
    },
    {
    },
    {
    },
    {
    }
  },
  {
     1,
     2,
     3,
     4
  }
};


#endif