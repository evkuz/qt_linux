sudo apt install nginx

sudo mkdir /home/www-data

Copy project code to /home/www-data



sudo systemctl enable gunicorn.service



## Installing python 3.8.1

First of all you need to install all prerequsits

```bash
sudo apt-get update
sudo apt install cmake
sudo apt-get install -y build-essential tk-dev libncurses5-dev libncursesw5-dev libreadline6-dev libdb5.3-dev libgdbm-dev libsqlite3-dev libssl-dev libbz2-dev libexpat1-dev liblzma-dev zlib1g-dev libffi-dev ninja
```

Then you need to get python:

```bash
wget https://www.python.org/ftp/python/3.8.1/Python-3.8.1.tar.xz
tar xf Python-3.8.1.tar.xz
```

And Build it

```bash
cd Python-3.8.1
./configure --enable-optimizations --prefix=/usr
make
```

When operations will be compleated, install it:

```bash
sudo make altinstall
```

Now you can remove unneeded files:

```bash
cd ..
sudo rm -r Python-3.8.0
rm Python-3.8.0.tar.xz
. ~/.bashrc
```

Let's make python3.8 as default python3

```bash
sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1
```

Now we need to fix some future errors:
```bash
sudo nano /usr/bin/lsb_release
```

And fix there first line to `#!/usr/bin/python3.7`



python3 -m venv .venv --prompt='http-server'
source .venv/bin/activate
pip install --upgrade pip setuptools wheel
pip install -r requirements.txt
