'use strict'

/* ////////////////////////////
/  120 office
/  121 office-ac
/  122 living
/  126 suite
/   61 stage3 
*/

class AC {
    constructor(args){
        if(args){
            this.name = "ac";
            this.status = args.status || false;
            this.controller = args.controller || undefined;
            this.html = undefined;
        }
    }

    update(newStatus){
    }
    
    render(){
        if(!this.html){
            let div=document.createElement("div");
            div.className="button box ac";
            div.innerText = this.name;

            if(this.status>0){
                div.classList.add("on");
            }

            div.addEventListener("click", this.toggle());

            this.html = div;
            if(this.controller){
                this.controller.add_device(this.html);
            }
        }
        return this.html;
    }


    toggle(){
        
    }

}