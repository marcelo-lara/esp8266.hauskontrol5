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
            this.details_menu= undefined;
            this.button = undefined;
        }
    }

    update(newDev){

        //update device
        this.status = newDev.status;
        this.name = newDev.name;
        
        let curr_speed = newDev.status.speed;
        if(this.details_menu.querySelector("div.on")){
            this.details_menu.querySelector("div.on").classList.remove("on");
        }
        this.details_menu.querySelector("div[val='"+curr_speed+"']").classList.add("on");

        //render
        if(curr_speed=="0"){
            this.button.classList.remove("on");
        }else{
            this.button.classList.add("on");
        }

    }
    
    render(){
        if(!this.html){
            let div=document.createElement("div");
            div.className="fan";

            let inner_menu = "<h3>fan</h3>";

            for (let i = 0; i < 5; i++) {
                inner_menu += "<div class=\"button box\" val=\""+i+"\">"+(i==0?"off":i)+"</div>";                
            }

            div.innerHTML=
                "<div class=\"button box pwr\">fan</div>"+
                "<div class=\"details\">" + inner_menu + "</div>" +
                "";

            this.button = div.querySelector(".pwr");
            div.querySelector(".pwr").addEventListener("click", this.details.bind(this));

            this.details_menu = div.querySelector(".details");
            for (const el of this.details_menu.querySelectorAll(".button")) {
                el.addEventListener("click", this.set_speed.bind(this))
            };
            this.html = div;
        }
        this.update(this);
        return this.html;
    }

    details(){
        this.details_menu.classList.add("visible");
        this.button.classList.add("hide");
    }

    set_speed(src){
        let val = src.srcElement.getAttribute("val");
        
        if(val=="0"){
            this.controller.send("/set/fan/off");
        }else{
            this.controller.send("/set/fan/speed/" + val);
        }

        this.details_menu.classList.remove("visible");
        this.button.classList.remove("hide");
    }

    toggle(){
        this.controller.send("/set/fan/" + this.name + "/" + (this.status=="1"?"off":"on"));
    }

}