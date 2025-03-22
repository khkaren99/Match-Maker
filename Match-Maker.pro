TEMPLATE = app

QT += widgets core gui

ROOT = .
DESTDIR = ""
TARGET_DIR = $$ROOT/build


TARGET = $$TARGET_DIR/out

SRC_PATH = $$ROOT/src
MOC_DIR = $$SRC_PATH/moc


INCLUDEPATH += $$SRC_PATH/include

SOURCES += $$SRC_PATH/*.cpp
HEADERS += $$SRC_PATH/include/*h

CONFIG(debug, debug|release) {
    TARGET_DIR = build-g
    DEFINES += DEBUG
    OBJECTS_DIR = $$SRC_PATH/obj-g

    QMAKE_CXXFLAGS = -g
}

CONFIG(release, debug|release) {
    TARGET_DIR = build
    DEFINES += QT_NO_DEBUG_OUTPUT NDEBUG
    OBJECTS_DIR = $$SRC_PATH/obj

    QMAKE_CXXFLAGS = -O3
}

QMAKE_CXXFLAGS = -O3 -g

QMAKE_CLEAN_EXTRA_FLAG = -r
QMAKE_CLEAN += $$QMAKE_CLEAN_EXTRA_FLAG
QMAKE_CLEAN += .qmake.stash
QMAKE_CLEAN += $$TARGET_DIR
QMAKE_CLEAN += $$MOC_DIR
QMAKE_CLEAN += $$OBJECTS_DIR
