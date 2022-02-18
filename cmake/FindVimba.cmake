# find Vimba SDK lib
#
# exports:
#
#   Vimba_FOUND
#   Vimba_INCLUDE_DIRS
#   Vimba_LIBRARIES
#

include(FindPackageHandleStandardArgs)

get_filename_component( Vimba_ROOT "/opt/Vimba_5_1" ABSOLUTE )

# Include dir
find_path(Vimba_INCLUDE_DIR 
    NAMES VimbaCPP/Include/VimbaCPP.h
    PATHS ${Vimba_ROOT})

# The library itself
find_library(Vimba_LIBRARY
  NAMES libVimbaCPP.so
  PATHS ${Vimba_ROOT}/VimbaCPP/DynamicLib/x86_64bit/
)

# Library for image transformation
find_library(Vimba_TRANSFORM
  NAMES libVimbaImageTransform.so
  PATHS ${Vimba_ROOT}/VimbaImageTransform/DynamicLib/x86_64bit/
)


# Response handler when find-package is called
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vimba DEFAULT_MSG Vimba_LIBRARY Vimba_TRANSFORM Vimba_INCLUDE_DIR)

# Set the cmake variable
set(Vimba_LIBRARIES ${Vimba_LIBRARY} ${Vimba_TRANSFORM})
set(Vimba_INCLUDE_DIRS ${Vimba_INCLUDE_DIR} ${Vimba_ROOT})
set(Vimba_FOUND yes)