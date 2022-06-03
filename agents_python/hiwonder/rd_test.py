from time import sleep
import requests

ADDR = "http://159.93.44.84:8383/"
#ADDR = "http://192.168.1.175:8383/"

def send_get_request(url:str, timeout:float)->dict:
    try:
        resp = requests.get(url=url, timeout=timeout)
    except Exception as e:
        raise e
    try:
        data = resp.json()
    except Exception as e:
        return {"value": resp.text}
    return data

def update_state():
    url = ADDR + "run?cmd=status&"
    return send_get_request(url, 0.2)

def run_action(name):
    url = f"{ADDR}run?cmd={name}&"
    return send_get_request(url, 0.5)


last_n_actions = -1
for i in range(1,150):
    try:
        if i == 20:
            st = run_action("start")
            print(st)
            sleep(0.1)
            continue
        
        st = update_state()
        if not isinstance(st, dict):
            print("iteration:", i, "Device answer isn't dict type!")
        if not 'action_list' in st.keys():
            print("iteration:", i, "Device answer hasn't action_list!")
            sleep(0.1)
            continue
        n_actions = len(st['action_list'])
        if n_actions != last_n_actions:
            print("iteration:", i, "; n_actions:", n_actions)
            last_n_actions = n_actions
        sleep(0.1)
    except Exception as e:
        print("iteration:", i, "error:")
        print(e)