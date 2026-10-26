from ._generate_com_config import generate as __generate_com_config
from ._generate_config import generate as __generate_config
from os import environ,getcwd

def generate_config(source_dir:str="",file_to_translate:str="",export_dir:str=""):
    return __generate_config(environ.get("SOURCEDIR",source_dir),environ.get("FILENAME",file_to_translate),environ.get("EXPORTDIR",export_dir),environ.get("WORKDIR",getcwd()))

def generate_com_config(source_dir:str="",file_to_translate:str="",export_dir:str=""):
    return __generate_com_config(environ.get("SOURCEDIR",source_dir),environ.get("FILENAME",file_to_translate),environ.get("EXPORTDIR",export_dir),environ.get("WORKDIR",getcwd()))