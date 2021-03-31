'use strict';
const controllers = new Array();

function get_json(uri){
    return fetch("http://"+uri)
    .then(response => response.json());
}

(()=>{
    [
        "192.168.1.122",
        "192.168.1.126",
        "192.168.1.120",
        "192.168.1.121",
        "192.168.1.61"
    ].forEach(_uri => {
        let ctrl=new Controller({uri: _uri});
        ctrl.discover();
        controllers.push(ctrl);
    });
        
})();