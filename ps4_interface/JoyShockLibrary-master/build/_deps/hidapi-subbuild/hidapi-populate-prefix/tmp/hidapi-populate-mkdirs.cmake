# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-src"
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-build"
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix"
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix/tmp"
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix/src/hidapi-populate-stamp"
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix/src"
  "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix/src/hidapi-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix/src/hidapi-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/build/_deps/hidapi-subbuild/hidapi-populate-prefix/src/hidapi-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
