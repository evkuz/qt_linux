# Example robote code

This is description of example robot code that hepls you to create your own IQR Device code.

## Description of directory structure

You can find several subdirectories in project dir:

* **daemon** - This directory contains an example of systemd module and instruction how to make your code works as a -system daemon
* **features** - this directory contains list of services and actions implemented for your robot. **__init__.py** file from this directory is very important, case it is something like global configuration of your robot.
* **templates** contains tamplate of web page that will be shown for visitors.  

And several files:

* **__init__.py** - python module initialisation file. You will not have to edit it.
* **.gitignore** - if you heve some files you need for local tests and you don't want to upload it to GIT you can add this files/directories to this file
* **config.py** - special configurations you need for your project 
* **README.md** - this file
* **runner.py** - special script to start your robot web-server. You can edit port number and server address here.
* **start.sh** - system script to start your web server
