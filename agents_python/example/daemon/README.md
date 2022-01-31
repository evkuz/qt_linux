# Example of daemon installation

[!] Please change "example" to your robot name

## Copy project code

The first you need to do is to copy your code to robot.
You need to copy at least two directories:

* **iqrdevice** - directory with base code library
* **example** - directory with all your code

Please copy both folders to one separated directory, for example:

|- ~/
    |-- agents/
        |-- iqrdevice/
        |-- example/
    |-- ...

## Installing packages

Now you need to make python virtual environment for your project.
Then you will need to install all required python packages.

You may try to use **start.sh** script for this purposes.
[!] Make sure you have python3, python3-venv and python3-pip packages installed on your system.

Please check that your robot server is working and can be started with **./start.sh**.

## Creating systemd module

After you checked that everything is working fix all paths in file **robot.example.service**.
Make sure your python environment paths specified correctly.

After file is ready copy it into */etc/systemd/system/*.

Enable your daemon:

```bash
sudo systemctl daemon-reload
sudo systemctl enable robot.example.service
```

Now you can start it and check:

```bash
sudo systemctl start robot.example.service
sudo systemctl status robot.example.service
```