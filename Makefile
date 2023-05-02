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

installModule:
	pip install . -v
	pybind11-stubgen --ignore-invalid=signature context
	cp stubs/context-stubs/context/__init__.pyi $$(pip show context | grep Location | cut -d " " -f2)/context
	rm -r stubs

packageModule:
	pip install . -v
	pybind11-stubgen --ignore-invalid=signature context
	cp stubs/context-stubs/context/__init__.pyi src/
	python setup.py sdist
	rm -r stubs

publishModule: packageModule #need to define org variable as an azure devops organization
	twine upload --repository-url https://pkgs.dev.azure.com/${org}/_packaging/python-feed/pypi/upload/ dist/context-$$(cat version.txt).tar.gz --verbose
# az artifacts universal publish --organization=https://dev.azure.com/cytocast --feed=python-feed --name=context --version=$$(cat version.txt) --description="GO-context library" --path=./dist
