TEMPLATE = app

QT += widgets core gui

SRC_PATH = ./src
SOURCES += $$SRC_PATH/*.cpp

INCLUDEPATH += $$SRC_PATH/include
HEADERS += $$SRC_PATH/include/*h

TARGET_DIR = ./bin
DESTDIR = ./build

CONFIG(release, debug|release) {
    TARGET = $$TARGET_DIR/app

    OBJECTS_DIR = $$DESTDIR/release/obj
    MOC_DIR = $$DESTDIR/release/moc

    QMAKE_CXXFLAGS += -O3 -DNDEBUG
}

CONFIG(debug, debug|release) {
    TARGET = $$TARGET_DIR-g/app

    OBJECTS_DIR = $$DESTDIR/debug/obj
    MOC_DIR = $$DESTDIR/debug/moc

    QMAKE_CXXFLAGS += ""
}

QMAKE_CLEAN_EXTRA_FLAG = -r
QMAKE_CLEAN += $$QMAKE_CLEAN_EXTRA_FLAG
QMAKE_CLEAN += $$TARGET_DIR*
QMAKE_CLEAN += $$DESTDIR
