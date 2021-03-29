#!/bin/bash

docker build -t hauskontrol-webpanel .

docker run -dit --name hauskontrol-webpanel -p 8080:80 hauskontrol-webpanel