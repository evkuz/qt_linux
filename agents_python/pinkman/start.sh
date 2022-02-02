#! /bin/bash

if [ ! -d '.venv' ]; then
  python3 -m venv .venv --prompt='robot-http' || {
      echo "can't create virtual enviroment!"
      exit 1
  }
  source .venv/bin/activate
  python -m pip install pip --upgrade
  python -m pip install -r requirements.txt || {
      echo "can't install requirements!"
      exit 2
  }
fi

source .venv/bin/activate || {
      echo "can't activate virtual enviroment!"
      exit 3
} && {
#  export FLASK_ENV=config.ProductionConfig
  python runner.py
}
