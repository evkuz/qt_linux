var device_addr = 'http://159.93.69.188:5001'
var calibrationInProgress = false;
var calibZoneX1 = -1;
var calibZoneY1 = -1;
var calibZoneX2 = -1;
var calibZoneY2 = -1;

function set_device_addr(addr){
    device_addr = addr
}

var canvas = document.getElementById('videobox_canvas');
var context = canvas.getContext("2d");
context.strokeStyle = "blue";
context.lineWidth = 3;

mousemove = function(e) { 
    if (!calibrationInProgress){
        return
    }
    var x = e.pageX - canvas.offsetLeft;
    var y = e.pageY - canvas.offsetTop;
    
    calibZoneX2 = x;
    calibZoneY2 = y;
    
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.strokeRect(calibZoneX1, calibZoneY1, calibZoneX2-calibZoneX1, calibZoneY2-calibZoneY1);
}

var videobox = document.getElementById('videobox');
videobox.onmousemove = mousemove;
videobox.onmousedown = function(e) {
    var x = e.pageX - canvas.offsetLeft;
    var y = e.pageY - canvas.offsetTop;

    calibZoneX1 = x;
    calibZoneY1 = y;
    calibZoneX2 = -1;
    calibZoneY2 = -1;  
    calibrationInProgress = true;
}
videobox.onmouseup = function(e) { 
    if( true) { //(calibZoneX2 - calibZoneX1) > 0 && (calibZoneY2 - calibZoneY1) > 0 ) {
        let url = device_addr + "service?name=camcalib";
        url += "&x1=" + calibZoneX1;
        url += "&y1=" + calibZoneY1;
        url += "&x2=" + calibZoneX2;
        url += "&y2=" + calibZoneY2;
        let response = fetch(url);
    }
    calibrationInProgress = false;
    context.clearRect(0, 0, canvas.width, canvas.height);
}

videobox.onmouseleave = function(e){
    calibrationInProgress = false;
    context.clearRect(0, 0, canvas.width, canvas.height);
}
