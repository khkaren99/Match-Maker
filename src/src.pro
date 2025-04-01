include (../Match-Maker.pri)

TEMPLATE = app
QT += widgets core gui

DESTDIR = $$DEST_DIR
TARGET_DIR = ./bin
SRC_PATH = ./

SOURCES += $$SRC_PATH/*.cpp
HEADERS += $$SRC_PATH/include/*h

CONFIG(release, debug|release) {
    TARGET = $$TARGET_DIR/app

    OBJECTS_DIR = $$DESTDIR/release/src/obj
    MOC_DIR = $$DESTDIR/release/src/moc

    QMAKE_CXXFLAGS += -O3 -DNDEBUG
}

CONFIG(debug, debug|release) {
    TARGET = $$TARGET_DIR-g/app

    OBJECTS_DIR = $$DESTDIR/debug/src/obj
    MOC_DIR = $$DESTDIR/debug/src/moc

    QMAKE_CXXFLAGS += -g
}

QMAKE_CLEAN += -r $$OBJECTS_DIR $$MOC_DIR $$DEST_DIR/$$TARGET  $$DEST_DIR
