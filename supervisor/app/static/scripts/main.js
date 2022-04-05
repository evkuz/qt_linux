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

async function runAction(addr, name){
  let url = addr + "/action?name=" + name;
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

async function sendReset(addr){
  let url = addr + "/reset";
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

async function getState(addr){
  let url = addr + "/status";
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

async function step1(conf){
  let res1 = await pmCatchCube(conf);
  let res2 = await xrGoToPinkman(conf);
}

async function step2(conf){
  let res = await pmSetCube(conf);
}

async function step3(conf){
  let res1 = await xrGoToHiwounder(conf);
  let res2 = await pmGoToStart(conf);
}

async function step4(conf){
  let res = await hwTakeCube(conf);
}

async function step5(conf){
  let res1 = await hwSetCube(conf);
  let res2 = await mmGoToTakeCube(conf);
}

async function step6(conf){
  let res = await mmcTakeCube(conf);
}

async function step7(conf){
  let res1 = await mmGoToBlueman(conf);
  let res2 = await wtGoToBlueman(conf);
}

async function step8(conf){
  let res = await bmCatchCube(conf);
}

async function step9(conf){
  let res = await bmSetCube(conf);
}

async function step10(conf){
  let res1 = await wtGoToPinkman(conf);
  let res2 = await pmGoToStart(conf);
}

// ===================================
//1. PinkMan
async function pmCatchCube(conf){
  let res = await runAction(conf.pinkman_addr, "catchcube");
  return res;
}
async function pmSetCube(conf){
  let res = await runAction(conf.pinkman_addr, "putcube");
  return res;
}
async function pmGoToStart(conf){
  let res = await sendReset(conf.pinkman_addr);
  return res;
}
async function pmReset(conf){
  let res = await sendReset(conf.pinkman_addr);
  return res;
}

// ===================================
//2. XRRobot
async function xrGoToHiwounder(conf){
  let res = await runAction(conf.xrrobot_addr, "movetoa");
  return res;
}
async function xrGoToPinkman(conf){
  let res = await runAction(conf.xrrobot_addr, "movetob");
  return res;
}

// ===================================
//3. Hiwonder


// ===================================
//4. Mobman_camera


// ===================================
//5. Mobman_manip


// ===================================
//6. Mobman


// ===================================
//7. Blueman
async function bmCatchCube(conf){
  let res = await runAction(conf.blueman_addr, "catchcube");
  return res;
}
async function pmSetCube(conf){
  let res = await runAction(conf.blueman_addr, "putcube");
  return res;
}
async function pmGoToStart(conf){
  let res = await runAction(conf.blueman_addr, "movehome");
  return res;
}
async function pmReset(conf){
  let res = await sendReset(conf.blueman_addr);
  return res;
}

// ===================================
//8. WheelTec







async function m1Transfer(conf){
  let res = await sendCommand(conf.wheeltecAddr, "transfercube");
}

async function m1MoveBack(conf){
  let res = await sendCommand(conf.wheeltecAddr, "moveback");
}

async function m1Reset(conf){
  let res = await sendCommand(conf.wheeltecAddr, "reset");
}



async function m2Transfer(conf){
  let res = await sendCommand(conf.xrrobotAddr, "transfercube");
}

async function m2Reset(conf){
  let res = await sendCommand(conf.xrrobotAddr, "reset");
}