import serial, struct, json, yaml
from yaml import Loader
from os import path

p = path.dirname(path.abspath(__file__))


# CRC-16/CCITT-FALSE
def crc16(data : bytearray):
    length = len(data)
    if data is None :
        return 0
    crc = 0xFFFF
    for i in range(0, length):
        crc ^= data[i] << 8
        for j in range(0,8):
            if (crc & 0x8000) > 0:
                crc =(crc << 1) ^ 0x1021
            else:
                crc = crc << 1
    return crc & 0xFFFF



def get_config():
    global p
    with open(path.join(p,"config_map.yaml"), "r") as f:
        conf_map:dict = yaml.load(f,Loader)
    
    with open(path.join(p,"config.yaml"), "r") as f:
        conf:dict = yaml.load(f,Loader)
    
    with open(path.join(p,"config_default.yaml"), "r") as f:
        conf_default:dict = yaml.load(f,Loader)
    
    data = {"exp":["<"],"data":[]}
    
    def itt_key(n,m,c,d,out):
        if not isinstance(m,dict):
            out["exp"].append(m)
            if c is None:
                ##################
                print(f"warning:'{n}' is not defined in config, using default: {d}")
                out["data"].append(d)
            else:
                out["data"].append(c)
            return
        
        for i in m.keys():
            itt_key(i,m[i],c.get(i),d.get(i),out)
    
    itt_key("",conf_map,conf,conf_default,data)
    
    return "".join(data["exp"]), data["data"]

teensy = serial.Serial("/dev/ttyAMA0",250000)

teensy.read_all()

int64 = struct.pack("<i",20)

with open(path.join(p,"com_config.json"), "r") as f:
    com_conf:dict = json.load(f)


to_byte = lambda i: struct.pack("<B",i)

terminator = to_byte(0)

if __name__ == "__main__":
    print(hex(crc16("123456789".encode())))
    
    while True:
        t,m = teensy.read(2)
        
        print(t,m)
        
        if t==com_conf["type"]["command"]:
            if m == com_conf["message"]["command"]["ping"]:
                teensy.write(to_byte(com_conf["return_data"]["accept"])+terminator)
            elif m == com_conf["message"]["command"]["get_config"]:
                exp,d = get_config()
                packed_conf = struct.pack(exp,*d)

                packed_conf = struct.pack("<H",struct.calcsize(exp)) + packed_conf + struct.pack("<H",crc16(packed_conf))
                
                print(struct.calcsize(exp))
                
                teensy.write(to_byte(com_conf["return_data"]["complex_data"]))
                teensy.write(packed_conf)
                teensy.write(terminator)
            elif m == 4:
                teensy.write(to_byte(com_conf["return_data"]["simple_data"]))
                teensy.write(int64)
                teensy.write(terminator)
        
        teensy.flush()
            
        
        terminator_received = teensy.read(1)
        if terminator_received != terminator:
            print("Invalid request terminator:", terminator_received)