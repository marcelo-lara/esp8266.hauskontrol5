'use strict'

class Controller {
    constructor(args){
        this.name = args.name || "";
        this.uri = args.uri || "";
    }

    discover(){
        if(this.name=="" || this.uri==""){
            console.log("ERROR: controller missing parameters")
        }

        fetch("http://"+this.uri+'/status')
        .then(response => response.json())
        .then((devs)=>{
            console.log("received", devs);
            

            devs.lights.forEach(light => {
                console.log("- light:", light.name );
            });

        });        
    }

}