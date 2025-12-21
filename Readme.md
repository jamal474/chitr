Commands :

## Install
```shell
conan install . --build=missing
```

## CMAKE
Inside ``build`` folder
```shell
cmake .. -DCMAKE_TOOLCHAIN_FILE="Release/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
```
for each subsequent changes
```shell
cmake --build . 
```
