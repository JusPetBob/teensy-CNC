@echo off

set WORKDIR=%cd%
set SOURCEDIR=config
set FILENAME=config
set EXPORTDIR=include

if not exist %WORKDIR%/helper/.venv/Scripts/activate.bat (
    python -m venv %WORKDIR%/helper/.venv
)

call %WORKDIR%/helper/.venv/Scripts/activate.bat

python -m pip install -r %WORKDIR%/helper/requirements.txt

echo config.h :

python -c "import helper; helper.generate_config()"

set FILENAME=com_config

echo com_config.h :

python -c "import helper; helper.generate_com_config()"