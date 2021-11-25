Copy project code to /home/wheeltec/wheeltec

```bash
python3 -m venv .venv --prompt='http-server'
source .venv/bin/activate
pip install --upgrade pip setuptools wheel
pip install -r requirements.txt
```

Copy robot-http.service to /etc/systemd/system/
Copy rosrun.service to /etc/systemd/system/
Copy rosrun.sh to /home/wheeltec


```bash
sudo systemctl daemon-reload
sudo systemctl enable robot-http.service
sudo systemctl start robot-http.service
```
