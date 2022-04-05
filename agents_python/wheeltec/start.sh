#! /bin/bash

export FLASK_ENV=config.ProductionConfig
export PYTHONPATH=/home/wheeltec/agents_python/wheeltec

/home/wheeltec/agents_python/.venv/bin/python runner.py >> "http-server.log"
