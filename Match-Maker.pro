include (./Match-Maker.pri)

TEMPLATE = subdirs

SUBDIRS += src src/games

QMAKE_CLEAN += -r $$DEST_DIR
