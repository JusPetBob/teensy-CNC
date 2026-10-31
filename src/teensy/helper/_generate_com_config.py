from os import path,environ
import json

def generate(source_dir:str,file_to_translate:str,export_dir:str,p:str):
    global out_def,out_init
    
    out_def = ""
    out_init = f"constexpr {file_to_translate.upper()} {file_to_translate} = "

    def generate_out(n: str,d: dict|int,level: int):
        global out_def,out_init
        
        if isinstance(d,int):
            out_init += f"{'  '*level} {d},\n"
            out_def += f"{'  '*level}uint8_t {n};\n"
        
        else:
            out_def += f"{'  '*level}struct {n}{'{'}\n"
            out_init += f"{'  '*level}{'{'}\n"
            
            for i in d.keys():
                generate_out(i,d[i],level+1)
                
            out_def += f"{'  '*level}{'}'}{' '+n if level!=0 else ''};\n"
            if out_init.endswith(",\n"):
                out_init = out_init[:-2]+"\n"
            out_init += f"{'  '*level}{'}'},\n"


    with open(path.join(p,source_dir,file_to_translate+".json"),"r") as f:
        file = json.load(f)

    generate_out(file_to_translate.upper(),file, 0)

    if out_init.endswith(",\n"):
        out_init = out_init[:-2]+";\n"

    out_comment = "/*\n"

    comment_file = path.join(p,source_dir,file_to_translate+".txt")

    if path.exists(comment_file):
        with open(comment_file,"r") as f:
            out_comment += f.read()

    out_comment += f"*/\n\n#ifndef {file_to_translate.upper()}_LIB_H_\n#define {file_to_translate.upper()}_LIB_H_"
    
    print(f"{out_comment}\n\n{out_def}\n\n{out_init}\n\n#endif")

    with open(path.join(p,export_dir,file_to_translate+".h"), "w") as f:
        f.write(f"{out_comment}\n\n{out_def}\n\n{out_init}\n\n#endif")
        

if __name__ == "__main__":
    from os import getcwd
    
    source_dir = "config"

    file_to_translate = "com_config"

    export_dir = "include"
    
    abs_path = environ.get("WORKDIR",getcwd())
    
    generate(source_dir,file_to_translate,export_dir,abs_path)