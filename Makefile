
DIR:= ${CURDIR}

build-globy: docker-arm-gcc-toolchain
	docker run -it -v $(DIR)/globy-proj:/src --rm --name nrf51-builder dockerized-nrf51-dev:1.0
	#delete this line after see how we can get the openocd in every OS :/, by now go and download
	echo "now flash your output file in globy-proj/_build/nrf51822_xxac.hex" 
	
docker-arm-gcc-toolchain:
	docker build -t dockerized-nrf51-dev:1.0 docker-build/

clean:
	docker run -e J=12 -e OPT=clean -v $(DIR)/globy-proj:/src --rm --name nrf51-builder dockerized-nrf51-dev:1.0 