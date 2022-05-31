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


Problem with certificates:

# repos (https://askubuntu.com/questions/1095266/apt-get-update-failed-because-certificate-verification-failed-because-handshake):
sudo apt install ca-certificates

# ros2 (https://answers.ros.org/question/379190/apt-update-signatures-were-invalid-f42ed6fbab17c654/):
curl http://repo.ros2.org/repos.key | sudo apt-key add -


# reconfigure locales:
sudo locale
sudo locale-gen en_US
sudo locale-gen en_US.UTF-8
sudo dpkg-reconfigure locales
sudo update-locale LANG=en_US.UTF-8

cp ~/.pam_environment ~/.pam_environment.backup
vi ~/.pam_environment


# problems with pip

wget https://bootstrap.pypa.io/get-pip.py
python get-pip.py