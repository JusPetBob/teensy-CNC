#! /bin/bash

export WORKDIR="$PWD"
export SOURCEDIR=config
export FILENAME=config
export EXPORTDIR=include


if [ ! -f $WORKDIR/helper/.venv/bin/activate ]; then
    python -m venv $WORKDIR/helper/.venv
fi

source $WORKDIR/helper/.venv/bin/activate

python -m pip install -r $WORKDIR/helper/requirements.txt

echo "config.h :"

python -c "import helper; helper.generate_config()"

export FILENAME=com_config

echo "com_config.h :"

python -c "import helper; helper.generate_com_config()"