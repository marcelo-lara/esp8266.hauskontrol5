'use strict'

class Controller {
    constructor(args){
        this.name = args.name || "";
        this.uri = args.uri || "";
        this.devices = new Array();
        this.devices_elem = undefined;
        this.html = undefined;

    }

    discover(){

        // build element
        if(!this.html){
            let wrapper = document.createElement("div");
            wrapper.classList.add("controller");
            wrapper.innerHTML = "<section><h2></h2><div class=\"devices\"></div></section>";
            this.html = wrapper;
            this.devices_elem = this.html.querySelector("div.devices");
            document.querySelector("div#ui").appendChild(this.html);
        }

        // query controller
        if(this.uri==""){console.log("ERROR: controller missing parameters")}
        let ctrl = this;
        fetch("http://"+this.uri+'/status')
        .then(response => response.json())
        .then((devs)=>{
            console.log(devs.name, devs);

            ctrl.name=devs.name;
            ctrl.html.querySelector("h2").innerText = ctrl.name;

            // lights
            if (devs.lights){
                if(Array.isArray(devs.lights)){
                    devs.lights.forEach(light => {
                        this.update_device(new Light({controller: this, name: "main", status: light.main=="1"}))
                    });
                }else{
                    for (const light in devs.lights) {
                        this.update_device(new Light({controller: this, name: light, status: devs.lights[light]}));
                    }
                }
            };

        // devices
            if (devs.fan) this.update_device(new Fan({controller: this, status: devs.fan}));
            if (devs.ac) this.update_device(new AC({controller: this, status: devs.ac}));

        });        
    }

    update_device(dev){

        let item = this.devices.find(x=>x.name==dev.name);

        if(item){
            item.update(dev);

        }else{
            console.log("not found", dev)
            //create device if not exists
            this.devices.push(dev);
            this.devices_elem.appendChild(dev.render());
     
        };


    }

    add_device(device){
    }

    send(api){
        let uri = this.uri + api;

        get_json(uri)
        .then(res => {
            if(res.result || res.result=="ok"){
                this.discover()
            }
        });
    }

}