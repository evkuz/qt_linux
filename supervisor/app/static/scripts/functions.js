async function runAction(device, action){
  let url = "/run_action?device=" + device + "&action=" + action;
  let response = await fetch(url);
  if (response.ok) { // если HTTP-статус в диапазоне 200-299
    // получаем тело ответа (см. про этот метод ниже)
    let json = await response.json();
    if (json.rc == 0) {
        return true;
    }
  }
  return false;
}

async function sendReset(device){
  let url = "/reset?device=" + device;
  let response = await fetch(url);
  if (response.ok) { // если HTTP-статус в диапазоне 200-299
    // получаем тело ответа (см. про этот метод ниже)
    //let json = await response.json();
    let json = await response.json();
    if (json.rc == 0) {
        return true;
    }
  }
  return false;
}

async function getState(){
  let url = "/status";
  let response = await fetch(url);
  let res = null;
  if (response.ok) { // если HTTP-статус в диапазоне 200-299
    // получаем тело ответа (см. про этот метод ниже)
    res = await response.json();
  }
  return res;
}

async function getInfo(device, service){
  let url = "/get_info?device=" + device + "&service=" + service;
  let response = await fetch(url);
  if (response.ok) { // если HTTP-статус в диапазоне 200-299
    // получаем тело ответа (см. про этот метод ниже)
    let json = await response.json();
    if (json.rc == 0) {
      alert(JSON.stringify(json.data)); 
      return true;
    }
  }
  return false;
}