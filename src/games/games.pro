include (../../Match-Maker.pri)

TEMPLATE = app

DESTDIR = $$DEST_DIR
TARGET_DIR = ./bin
SRC_PATH = ./
INCLUDEPATH += $$SRC_PATH/include

SOURCES += $$SRC_PATH/*.cpp
HEADERS += $$SRC_PATH/include/*h

CONFIG(release, debug|release) {
    TARGET = $$TARGET_DIR/game

    OBJECTS_DIR = $$DESTDIR/release/games/obj
    MOC_DIR = $$DESTDIR/release/games/moc

    QMAKE_CXXFLAGS += -O3 -DNDEBUG
}

CONFIG(debug, debug|release) {
    TARGET = $$TARGET_DIR-g/game

    OBJECTS_DIR = $$DESTDIR/debug/games/obj
    MOC_DIR = $$DESTDIR/debug/games/moc

    QMAKE_CXXFLAGS += -g
}

PRE_TARGETDEPS += $$OBJECTS_DIR $$MOC_DIR

QMAKE_CLEAN += -r $$OBJECTS_DIR $$MOC_DIR
