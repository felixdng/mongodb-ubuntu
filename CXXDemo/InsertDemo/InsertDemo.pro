TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/mongo-cxx-driver-r3.1.2/lib/release/ -lbsoncxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/mongo-cxx-driver-r3.1.2/lib/debug/ -lbsoncxx
else:unix: LIBS += -L$$PWD/../../../../../usr/local/mongo-cxx-driver-r3.1.2/lib/ -lbsoncxx

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/mongo-cxx-driver-r3.1.2/lib/release/ -lmongocxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/mongo-cxx-driver-r3.1.2/lib/debug/ -lmongocxx
else:unix: LIBS += -L$$PWD/../../../../../usr/local/mongo-cxx-driver-r3.1.2/lib/ -lmongocxx

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include
