from os import path,environ
import yaml
import json
from yaml import Loader

def generate(source_dir:str,file_to_translate:str,export_dir:str,p:str):
    global out_def,out_load,counter,out_load_bool_validation
    
    out_def = ""
    
    out_load = f"bool load_{file_to_translate}(uint8_t* buffer, size_t len, {file_to_translate.upper()}* {file_to_translate}) {'{'}\n  if (buffer == nullptr || config == nullptr || len != config_expected_length) return false;\n<bool_validation>"
    
    out_load_bool_validation = "  if (\n"
    
    counter = 0

    def generate_out(n: str,d,level: int,pos,m):
        global out_def,out_load,out_load_bool_validation,counter
        
        if not isinstance(d,dict):
            print(f"{n:<15} {d:>2} {level:>2}  {pos}")
            out_def += f"{'  '*level}{m["arduino"].get(d)} {n};\n"
            
            out_load += f"  {pos} = {' | '.join([f'{'('if k!=0 else ''}({m["arduino"].get(d)})buffer[{counter+k}]{f" << {k*8})" if k!=0 else ""}' for k in range(m["size"].get(d))]) if m["size"].get(d)!=1 else f"buffer[{counter}]"};\n"
            
            if d=="?":
                out_load_bool_validation += f"    !(buffer[{counter}] <= 1) ||\n"
            
            counter += m["size"].get(d)
            
        
        else:
            out_def += f"\n{'  '*level}struct {n}{'{'}\n"
            
            out_load += "" if out_load.endswith("\n\n") else "\n"
            
            for i in d.keys():
                generate_out(i,d[i],level+1,f"{pos}{f".{i}" if level!=0 else f"->{i}"}",m)
                
            out_def += f"{'  '*level}{'}'}{' '+n if level!=0 else ''};\n"


    with open(path.join(p,source_dir,file_to_translate+".yaml"),"r") as f:
        file = yaml.load(f,Loader)


    with open(path.join(p,source_dir,"type_map.json"),"r") as f:
        m = json.load(f)

    generate_out(file_to_translate.upper(), file, 0, file_to_translate, m)
    
    out_load_bool_validation = out_load_bool_validation[:-3]+"\n  ) return false; // bool validation"
    
    out_load += "\n  return true;\n};"
    
    out_load = out_load.replace("<bool_validation>",out_load_bool_validation)
    
    out_comment = "/*\n"

    comment_file = path.join(p,source_dir,file_to_translate+".txt")

    if path.exists(comment_file):
        with open(comment_file,"r") as f:
            out_comment += f.read()

    out_comment += "*/"

    print(f"{out_comment}\n\n#define config_expected_length{counter}\n\n{out_def}\n\n{out_load}")

    with open(path.join(p,export_dir,file_to_translate+".h"), "w") as f:
        f.write(f"{out_comment}\n\n#define config_expected_length {counter}\n\n{out_def}\n\n{out_load}")
        

if __name__ == "__main__":
    from os import getcwd
    source_dir = "config"

    file_to_translate = "config"

    export_dir = "include"
    
    abs_path = environ.get("WORKDIR",getcwd())
    
    generate(source_dir,file_to_translate,export_dir,abs_path)