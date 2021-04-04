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
        if(args){
            this.name = args.name || "light";
            this.ref = args.ref || "main";
            this.status = args.status || false;
            this.controller = args.controller || undefined;
            this.html = undefined;
        }
    }

    update(newDev){
        
        //update device
        this.status = newDev.status;
        this.name = newDev.name;
        
        //render
        this.html.innerText = this.name;
        if(this.status>0){
            this.html.classList.add("on");
        }else{
            this.html.classList.remove("on");
        }
    }


    render(){
        if(!this.html){
            let div=document.createElement("div");
            div.className="button box light";
            div.addEventListener("click", this.toggle.bind(this));
            this.html = div;
        }
        this.update(this);
        return this.html;
    }

    toggle(){
        this.controller.send("/set/light/" + this.ref + "/" + (this.status=="1"?"off":"on"));
    }
}