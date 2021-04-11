#pragma once

class WebApi {
public:
    enum Controller : int {Living, Office, Suite, OfficeAc, Stage3};
    Controller controller;

    WebApi(Controller _controller);


};