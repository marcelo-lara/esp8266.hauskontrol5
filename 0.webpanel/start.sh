#!/bin/bash

# stop/remove current container
[ "$(docker ps -q -a -f name=hauskontrol-webpanel)" ] && docker rm -f hauskontrol-webpanel

# build image
docker build -t hauskontrol-webpanel .

# run container
docker run -dit --name hauskontrol-webpanel -p 80:80 hauskontrol-webpanel