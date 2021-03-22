'use strict';

(()=>{

    let office_light = new Light({node_url:"http://192.168.1.120", name:"Office Light"});
    document.getElementById("ui").appendChild(office_light.html);
    office_light.refresh_status();

    let living_main_light = new Light({node_url:"http://192.168.1.120", name:"Living Main Light"});
    document.getElementById("ui").appendChild(living_main_light.html);
    living_main_light.refresh_status();

        
})();