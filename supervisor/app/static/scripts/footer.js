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
let tpInProgress = false;

function updateState(){
    getState().then(st => {
        if (typeof st == "undefined") {
            return;
        }
        for (let key of Object.keys(st)) {
            try {
                let device = st[key];
                let needToUpdate = false;
                
                if (key == "supervisor") {
                    updateSupervisorState(device);
                    continue;
                }
                updateAgentState(key, device);
            } catch (err) {
                console.error(err);
            }
        };
    }).catch(err => {
        // got error
        console.error(err);
    });
}


function updateSupervisorState(device){
    if (tpInProgress != device.techprocess) {
        tpInProgress = device.techprocess
        tpBtn = document.getElementById("start_tp")
        if (tpInProgress) {
            tpBtn.onclick = function(){ sendTechProcess('false'); }
            tpBtn.classList.remove('btn-green');
            tpBtn.classList.add('btn-red');
            tpBtn.innerHTML = "Stop tech process";
        } else {
            tpBtn.onclick = function(){ sendTechProcess('true'); }
            tpBtn.classList.remove('btn-red');
            tpBtn.classList.add('btn-green');
            tpBtn.innerHTML = "Start tech process";
        }
    }
}


function updateAgentState(name, status){
    if (devices_prev_state.hasOwnProperty(name)){
        if (devices_prev_state[name] != status.connected){
            devices_prev_state[name] = status.connected;
            needToUpdate = true;
        }
    } else {
        devices_prev_state[name] = status.connected;
        needToUpdate = true;
    }
    if (needToUpdate){
        updateAgentButtons(name, status);
    }

    let st_elem = document.getElementById(name+"_status");
    st_elem.innerHTML = status.state;

    updateAgentHasCube(name, status.nodes);
}


function updateAgentButtons(name, status){
    var buttons = document.querySelectorAll('*[id^="' +  name + '_btn"]');
    if(status.connected){
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


function updateAgentHasCube(name, nodes){
    let has_cube = false;
    for (let i = 0; i < nodes.length; i++) {
        if (nodes[i].name == 'hascube') {
            has_cube = nodes[i].has_cube;   
        }
    }

    let hc_elem = document.getElementById(name+"_hascube");
    if (hc_elem != null) {
        if(has_cube) {
            hc_elem.innerHTML = "<div class='has_cube'>&nbsp;</div>";
        } else {
            hc_elem.innerHTML = "<div class='has_no_cube'>&nbsp;</div>";
        }
    }
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

tpBtn = document.getElementById("start_tp");
tpBtn.onclick = function(){ sendTechProcess('true'); };

StartStatusUpdate(sv_config.updateStateInterval);
