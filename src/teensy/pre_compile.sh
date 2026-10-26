#! /bin/bash

export WORKDIR="$PWD"
export SOURCEDIR=config
export FILENAME=config
export EXPORTDIR=include

echo "config.h :"

python -c "import helper; helper.generate_config()"

export FILENAME=com_config

echo "com_config.h :"

python -c "import helper; helper.generate_com_config()"