function httpGet(url, timeout) {
    return new Promise(function(resolve, reject) {
 
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'json';
    xhr.timeout = timeout;

    xhr.onload = function() {
      if (this.status == 200) {
        resolve(this.response);
      } else {
        var error = new Error(this.statusText);
        error.code = this.status;
        reject(error);
      }
    };
    
    xhr.ontimeout = function() {
      reject(new Error("Connection timeout"));
    };

    xhr.onerror = function() {
      reject(new Error("Network Error"));
    };

    xhr.send();
  });
}

async function sendCommand(addr, cmd){
  let url = addr + "/run?cmd=" + cmd + "&";
  let  = await fetch(url);
  if (response.ok) { // если HTTP-статус в диапазоне 200-299
    // получаем тело ответа (см. про этот метод ниже)
    //let json = await response.json();
    return true;
  } else {
    return false;
  }
}

async function getState(addr){
  let url = addr + "/run?cmd=status&";
  let  = await fetch(url);
  let res = {};
  res.good = false;
  res.text = "Connection timeout";
  if (response.ok) { // если HTTP-статус в диапазоне 200-299
    // получаем тело ответа (см. про этот метод ниже)
    let json = await response.json();
    res.good = true;
    res.text = "Connected";
  }
  return res;
}


//onclick="alert('Клик!')"

async function bmStart(conf){
  let res = await sendCommand(conf.manipulatorAddr, "start");
}

async function bmReset(conf){
  let res = await sendCommand(conf.manipulatorAddr, "reset");
}

async function m1Transfer(conf){
  let res = await sendCommand(conf.wheeltecAddr, "transfercube");
}

async function m1MoveBack(conf){
  let res = await sendCommand(conf.wheeltecAddr, "moveback");
}

async function m1Reset(conf){
  let res = await sendCommand(conf.wheeltecAddr, "reset");
}

async function pmCatchCube(conf){
  let res = await sendCommand(conf.pinkmanAddr, "catchcube");
}

async function pmPutCube(conf){
  let res = await sendCommand(conf.pinkmanAddr, "putcube");
}

async function pmReset(conf){
  let res = await sendCommand(conf.pinkmanAddr, "reset");
}

async function m2Transfer(conf){
  let res = await sendCommand(conf.xrrobotAddr, "transfercube");
}

async function m2Reset(conf){
  let res = await sendCommand(conf.xrrobotAddr, "reset");
}