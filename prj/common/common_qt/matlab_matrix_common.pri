#
# File matlab_matrix_common.pro
# File created : 19 Apr 2017
# Created by : Davit Kalantaryan (davit.kalantaryan@desy.de)
# This file can be used to produce Makefile for daqadcreceiver application
# for PITZ
#

MATLAB_VERSION = R2016b

include($${PWD}/../../common/common_qt/sys_common.pri)
message("!!! matlab_matrix_common.pri CODENAME=$$CODENAME")

#TEMPLATE = app
#TARGET = .


LIBS += -L$${PWD}/../../../sys/$$CODENAME/lib
INCLUDEPATH += $${PWD}/../../../include

message("R2016b is used")
LIBS += -lmx
LIBS += -lut
LIBS += -lmwresource_core
LIBS += -lmwi18n
LIBS += -lmwfl
#LIBS += -lboost_chrono
LIBS += -lboost_date_time
LIBS += -lboost_filesystem
LIBS += -lboost_log
LIBS += -lboost_regex
LIBS += -lboost_signals
LIBS += -lboost_system
LIBS += -lboost_thread
LIBS += -lmwcpp11compat
LIBS += -licudata
LIBS += -licuuc
LIBS += -licui18n
LIBS += -licuio
LIBS += -ltbb
LIBS += -ltbbmalloc
LIBS += -lboost_serialization
LIBS += -lunwind
#LIBS += -lssl
#LIBS += -lcrypto

#LIBS += -lmwMATLAB_res
#LIBS += -lboost_log_setup


QT -= core
QT -= gui

OTHER_FILES += \
    $${PWD}/../../../scripts/create_new_system.sh
