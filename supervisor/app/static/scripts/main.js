function httpGet(url) {
    return new Promise(function(resolve, reject) {
 
      var xhr = new XMLHttpRequest();
      xhr.open('GET', url, true);
      xhr.responseType = 'json';
      xhr.timeout = iqrSupervisor.updateStateInterval;
  
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