include(C:/Users/elija/OneDriveWSU/EE324/Labs/finalProject/ps4_interface/JoyShockLibrary-master/cmake/CPM.cmake)
CPMAddPackage(NAME;hidapi;GITHUB_REPOSITORY;signal11/hidapi;VERSION;0;GIT_TAG;a6a622ffb680c55da0de787ff93b80280498330f;DOWNLOAD_ONLY;YES)
set(hidapi_FOUND TRUE)