'use strict'

/* ////////////////////////////
/  120 office
/  121 office-ac
/  122 living
/  126 suite
/   61 stage3 
*/

class Fan {
    constructor(args){
        if(args){
            this.name = "fan";
            this.status = args.status || false;
            this.controller = args.controller || undefined;
            this.html = undefined;
            this.overlay_menu= undefined;
        }
    }

    update(newDev){

        //update device
        this.status = newDev.status;
        this.name = newDev.name;
        
        let curr_speed = newDev.status.speed;
        if(this.overlay_menu.querySelector("div.on")){
            this.overlay_menu.querySelector("div.on").classList.remove("on");
        }
        this.overlay_menu.querySelector("div[val='"+curr_speed+"']").classList.add("on");

        //render
        if(this.status>0){
            this.html.classList.add("on");
        }else{
            this.html.classList.remove("on");
        }

    }
    
    render(){
        if(!this.html){
            let div=document.createElement("div");
            div.className="fan";

            let inner_menu = "";

            for (let i = 0; i < 5; i++) {
                inner_menu += "<div class=\"button box\" val=\""+i+"\">"+(i==0?"off":i)+"</div>";                
            }


            div.innerHTML=
                "<div class=\"overlay\">" + inner_menu + "</div>" +
                "<div class=\"button box pwr\">fan</div>";

            div.querySelector(".pwr").addEventListener("click", this.overlay.bind(this));

            this.overlay_menu = div.querySelector(".overlay");
            for (const el of this.overlay_menu.querySelectorAll(".button")) {
                el.addEventListener("click", this.set_speed.bind(this))
            };
            
            //div.addEventListener("click", this.toggle.bind(this));
            this.html = div;
        }
        this.update(this);
        return this.html;
    }

    overlay(){
        this.overlay_menu.classList.add("visible");
    }

    set_speed(src){
        let val = src.srcElement.getAttribute("val");
        
        if(val=="0"){
            this.controller.send("/set/fan/off");
        }else{
            this.controller.send("/set/fan/speed/" + val);
        }
        this.overlay_menu.classList.remove("visible");
    }

    toggle(){
        this.controller.send("/set/fan/" + this.name + "/" + (this.status=="1"?"off":"on"));
    }

}