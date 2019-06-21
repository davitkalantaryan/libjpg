
include ( $${PWD}/../../common/common_qt/sys_common.pri )

TEMPLATE = lib
QT -= core
QT -= gui
QT -= widgets

INCLUDEPATH += $${PWD}/../../../include
INCLUDEPATH += $${PWD}/../../../src/include/$$CODENAME

SOURCES += \
    $${PWD}/../../../src/tij/jaricom.c \
    $${PWD}/../../../src/tij/jcapimin.c \
    $${PWD}/../../../src/tij/jcmainct.c \
    $${PWD}/../../../src/tij/jcmarker.c \
    $${PWD}/../../../src/tij/jcomapi.c \
    $${PWD}/../../../src/tij/jdapimin.c \
    $${PWD}/../../../src/tij/jdapistd.c \
    $${PWD}/../../../src/tij/jdarith.c \
    $${PWD}/../../../src/tij/jdatasrc.c \
    $${PWD}/../../../src/tij/jdcoefct.c \
    $${PWD}/../../../src/tij/jdcolor.c \
    $${PWD}/../../../src/tij/jddctmgr.c \
    $${PWD}/../../../src/tij/jdhuff.c \
    $${PWD}/../../../src/tij/jdinput.c \
    $${PWD}/../../../src/tij/jdmainct.c \
    $${PWD}/../../../src/tij/jdmarker.c \
    $${PWD}/../../../src/tij/jdmaster.c \
    $${PWD}/../../../src/tij/jdmerge.c \
    $${PWD}/../../../src/tij/jdpostct.c \
    $${PWD}/../../../src/tij/jdsample.c \
    $${PWD}/../../../src/tij/jerror.c \
    $${PWD}/../../../src/tij/jidctflt.c \
    $${PWD}/../../../src/tij/jidctfst.c \
    $${PWD}/../../../src/tij/jidctint.c \
    $${PWD}/../../../src/tij/jmemansi.c \
    $${PWD}/../../../src/tij/jmemmgr.c \
    $${PWD}/../../../src/tij/jquant1.c \
    $${PWD}/../../../src/tij/jquant2.c \
    $${PWD}/../../../src/tij/jutils.c


HEADERS += \
    $${PWD}/../../../src/include/$$CODENAME/jconfig.h
