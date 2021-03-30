'use strict'

class Controller {
    constructor(args){
        this.name = args.name || "";
        this.uri = args.uri || "";
        this.devices = new Array();
    }

    discover(){
        if(this.uri==""){console.log("ERROR: controller missing parameters")}
        let ctrl = this;
        fetch("http://"+this.uri+'/status')
        .then(response => response.json())
        .then((devs)=>{
            ctrl.name=devs.name;

            // lights
            if (devs.lights){
                if(Array.isArray(devs.lights)){
                    devs.lights.forEach(light => {
                        ctrl.devices.push(new Light({name: light.name, status: light.main=="1"}));
                    });
                }else{
                    for (const light in devs.lights) {
                        ctrl.devices.push(new Light({name: light, status: devs.lights[light]}));
                    }
                }

            
           };

        });        
    }

}