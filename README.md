# Allied Vision Vimba SDK
## CMake implementation and OpenCV example
Vimba SDK delivers with only Makefile and Visual Studio support. 
To compile vimba based project use the **FindVimba.cmake** module and reference it on the *CMAKE_MODULE_PATH* of yours CMakeLists. 
From default Vimba in installed in /opt/Vimba_X_X/. If not change folder reference in the cmake module. 

### Opencv
Moreover in the Vimba example there is no clear use of OpenCV, and transfering from the two images domain, so the second example implements a dummy class object that show the conversion. 

## Requirements
-------
- Vimba SDK (v5.1)
- OpenCV (v4.5.5)

## Compile examples
-------
```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
```
