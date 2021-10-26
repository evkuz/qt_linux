Copy project code to /home/xrrobot/httpserver

```bash
python3 -m venv .venv --prompt='http-server'
source .venv/bin/activate
pip install --upgrade pip setuptools wheel
pip install -r requirements.txt
```

Copy robot-http.service to /etc/systemd/system/


```bash
sudo systemctl daemon-reload
sudo systemctl enable robot-http.service
sudo systemctl start robot-http.service
```
