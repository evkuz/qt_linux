if (typeof sv_config == "undefined") {
    throw new Error("sv_config must be defined befor this script!");
}

if(sv_config.devices.hasOwnProperty('pinkman')) {
    document.getElementById("pinkman_vb_img").src=sv_config.devices.pinkman+"/video_feed";
}
if(sv_config.devices.hasOwnProperty('hiwonder')) {
    document.getElementById("hiwonder_vb_img").src=sv_config.devices.hiwonder+"/video_feed";
}
if(sv_config.devices.hasOwnProperty('mobman_camera')) {
    document.getElementById("mobman_camera_vb_img").src=sv_config.devices.mobman_camera+"/video_feed";
}
if(sv_config.devices.hasOwnProperty('blueman')) {
    document.getElementById("blueman_vb_img").src=sv_config.devices.blueman+"/video_feed";
}


let statusTimeoutIsRuning = false;
let statusTimeoutId = null;
let devices_prev_state = {};

function updateState(){
    getState().then(st => {
        if (typeof st == "undefined") {
            return;
        }

        const devices = Object.keys(st);
        devices.forEach((key, index) => {
            let device = st[key];
            let needToUpdate = false;
            
            if (devices_prev_state.hasOwnProperty(key)){
                if (devices_prev_state[key] != device.connected){
                    devices_prev_state[key] = device.connected;
                    needToUpdate = true;
                }
            } else {
                devices_prev_state[key] = device.connected;
                needToUpdate = true;
            }
            if (needToUpdate){
                var buttons = document.querySelectorAll('*[id^="' +  key + '_btn"]');
                if(device.connected){
                    buttons.forEach(function(btnElem) {
                        btnElem.classList.remove('btn-gray');
                        btnElem.classList.add('btn-green');
                    });
                } else {
                    buttons.forEach(function(btnElem) {
                        btnElem.classList.remove('btn-green');
                        btnElem.classList.add('btn-gray');
                    });
                } 
            }
           
            let st_elem = document.getElementById(key+"_status");
            st_elem.innerHTML = device.state;
        });
    }).catch(err => {
        // got error
    });
}

function statusTimeoutFunction(interval){
    try {
        updateState();
    } catch (err) {
        console.error(err);
    }
    if (statusTimeoutIsRuning){
        statusTimeoutId = setTimeout(
            statusTimeoutFunction,
            interval,
            interval
        );
    }
};
  
function StartStatusUpdate(interval){
  ubtn = document.getElementById("update_btn");
  if (ubtn == null) {
    return;
  }
  ubtn.classList.remove('btn-red');
  ubtn.classList.add('btn-gray');
  ubtn.innerHTML = "Stop updating";
  statusTimeoutIsRuning = true;
  statusTimeoutFunction(interval);
};

function StopStatusUpdate(){
  statusTimeoutIsRuning = false;
  if (statusTimeoutId!=null) {clearTimeout(statusTimeoutId);}
  ubtn = document.getElementById("update_btn");
  if (ubtn == null) {
    return;
  }
  ubtn.classList.remove('btn-gray');
  ubtn.classList.add('btn-red');
  ubtn.innerHTML = "Start updating";
};



updateBtn = document.getElementById("update_btn")
if (updateBtn != null) {
    updateBtn.onclick = function(){
        if(statusTimeoutIsRuning){ StopStatusUpdate(); }
        else { StartStatusUpdate(sv_config.updateStateInterval); }
    };
}