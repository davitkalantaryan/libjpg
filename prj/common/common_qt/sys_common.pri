# File sys_common.pri
# File created : 12 Feb 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
#

#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-variable
#QMAKE_CXXFLAGS_WARN_ON += -Wno-sign-compare
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
#QMAKE_CXXFLAGS_WARN_ON -= -Wunused-function

#CONFIG += c++17
#QMAKE_CXXFLAGS += -std=c++14

optionsLib = $$find(TEMPLATE, "lib")

count(optionsLib, 1){
equals(TARGET_EXT,"mex*"){
    TARGET_PATH=mbin
    message("Matlab mex file creation")
}else{
    TARGET_PATH=lib
    message("Shared library creation")
}
}else{
    TARGET_PATH=bin
    message("Binary file creation")
}


win32{
    contains(QMAKE_TARGET.arch, x86_64): CODENAME = win64
    else:   CODENAME = win32
} else:macx {
    CODENAME = mac
} else:unix {
    CODENAME = $$system(lsb_release -c | cut -f 2)
}

message("!!! sys_common.pri: CODENAME=$$CODENAME")
