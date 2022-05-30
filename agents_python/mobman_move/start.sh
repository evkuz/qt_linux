#! /bin/bash

export FLASK_ENV=config.ProductionConfig
export PYTHONPATH=/home/xrrobot/agents_python/mobman_move

/home/xrrobot/agents_python/.venv/bin/python runner.py >> "http-server.log"
