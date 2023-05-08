.PHONY: build test format

build:
	mkdir -p build;
	cd build; cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_TEST=ON
	cd build; make -j

test: build
	./build/test/testContext

buildModule:
	mkdir -p build;
	cd build; cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=OFF -DBUILD_PY_MODULE=ON -DENABLE_UNITY:BOOL=ON -DENABLE_LTO:BOOL=ON
	cd build; make -j

installAndGenerateStub:
	pip install . -v --break-system-packages
	pybind11-stubgen --ignore-invalid=signature context

installModule: installAndGenerateStub
	cp stubs/context-stubs/context/__init__.pyi $$(pip show context | grep Location | cut -d " " -f2)/context
	rm -r stubs

packageModule: installAndGenerateStub
	cp stubs/context-stubs/context/__init__.pyi .
	python setup.py bdist_wheel
	rm -r stubs

publishPackage:
	twine upload --repository-url https://pkgs.dev.azure.com/${org}/_packaging/python-feed/pypi/upload/ dist/context-$$(cat version.txt)-cp311-cp311-linux_x86_64.whl --verbose

