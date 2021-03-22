'use strict'

/* ////////////////////////////
/  120 office
/  121 office-ac
/  122 living
/  126 suite
/   61 stage3 
*/

class Light {
    constructor(args){
        
      // controller
        this.node_url = args.node_url || "";
        this.device_name = args.name || "device";
        this.uri_path = args.uri_path || "light";

      // properties
        this.is_on = false;

      // html elements
        this.html = document.createElement("div");
        this.html.classList.add("button");
        this.html.innerHTML = this.device_name;


        this.html.addEventListener('click', function (event) {
            this.set_status(!this.is_on);
          }.bind(this));

    }

    toggle_status(){

    }

    update_html(){
        if(this.is_on){
            this.html.classList.add("on");
        }else{
            this.html.classList.remove("on");
        };
    }

    refresh_status(){
        fetch(this.node_url+'/status')
        .then(response => response.json())
        .then((data)=>{
            this.is_on = data.light=="1";
            this.update_html(); 
        });
    }

    set_status(light_status){
        let post_status = light_status?"on":"off";
        fetch(this.node_url + '/set/' + this.uri_path + '/' + post_status)
        .then(response => response.json())
        .then((data)=>{this.refresh_status();});
        console.log("set_status:", post_status);
    }

}