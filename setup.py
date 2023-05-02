from skbuild import setup

moduleNameAndPath = "context"
version = "Unknown"

with open("version.txt", "r") as f:
    version = f.read().strip()

setup(
    name=moduleNameAndPath,
    version=version,
    description="a minimal example package (cpp version)",
    author='The scikit-build team',
    license="MIT",
    packages=[moduleNameAndPath],
    python_requires=">=3.7",
    package_dir={"context": "src"},
    cmake_install_dir="src",
    data_files=[("src", ["src/module.cpp", "src/__init__.pyi", "version.txt"])],
    cmake_args=['-DBUILD_PY_MODULE:BOOL=ON', '-DENABLE_UNITY:BOOL=ON', '-DENABLE_LTO:BOOL=ON', '-DENABLE_TESTING=OFF'],
)
