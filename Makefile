.PHONY: build test format

build:
	mkdir -p build;
	cd build; cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_TEST=ON
	cd build; make -j

test: build
	./build/test/testContext

format:
	docker run --rm --mount src=${CURDIR},target=/app,type=bind khbence/format
