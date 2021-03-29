'use strict';

const haus = {
    controllers:[
        {"name": "living", "uri": "192.168.1.122"},
        {"name": "suite", "uri": "192.168.1.126"},
        {"name": "office", "uri": "192.168.1.120"},
        {"name": "officeac", "uri": "192.168.1.121"},
        {"name": "stage3", "uri": "192.168.1.61"}
    ]
};


(()=>{

    haus.controllers.forEach(controller => {
        console.log("::", controller.name, "["+ controller.uri+ "]");
        controller.obj = new Controller({name: controller.name, uri: controller.uri});
        controller.obj.discover();
    });
        
})();