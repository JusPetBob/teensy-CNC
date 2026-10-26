@echo off

set WORKDIR=%cd%
set SOURCEDIR=config
set FILENAME=config
set EXPORTDIR=include

echo config.h :

python -c "import helper; helper.generate_config()"

set FILENAME=com_config

echo com_config.h :

python -c "import helper; helper.generate_com_config()"