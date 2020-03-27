TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    boardoperations.cpp \
    playgame.cpp \
    user.cpp \
    carrier.cpp \
    battleship.cpp \
    ship.cpp \
    cruiser.cpp \
    submarine.cpp \
    destroyer.cpp

HEADERS += \
    user.h \
    ship.h \
    carrier.h \
    battleship.h \
    cruiser.h \
    submarine.h \
    destroyer.h
