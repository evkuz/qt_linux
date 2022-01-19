#! /bin/bash

if [ ! -d '.venv' ]; then
  python3 -m venv .venv --prompt='vehicle-http' || {
      echo "can't create virtual enviroment!"
      exit 1
  }
  python3 -m pip install -r requirements.txt || {
      echo "can't install requirements!"
      exit 2
  }
fi

source .venv/bin/activate || {
      echo "can't activate virtual enviroment!"
      exit 3
} && {
  export FLASK_ENV=config.ProductionConfig
  gunicorn --threads 5 --workers 1 --bind 0.0.0.0:5050 runner:app
}
