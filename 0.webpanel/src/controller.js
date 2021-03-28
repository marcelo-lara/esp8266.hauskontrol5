'use strict'

class Controller {
    constructor(args){
        this.name = args.name || "";
        this.uri = args.uri || "";
        this.lights = new Array();
    }

    discover(){
        if(this.name=="" || this.uri==""){
            console.log("ERROR: controller missing parameters")
        }

        fetch("http://"+this.uri+'/status')
        .then(response => response.json())
        .then((devs)=>{
            console.log(this.name, devs);
            
            // lights
            if (devs.lights){
                if(Array.isArray(devs.lights)){
                    devs.lights.forEach(light => {
                        console.log("- light:", light.name );
                    })
                }else{
                    console.log("- light:", devs.lights[0] );

                }
            }

        });        
    }

}