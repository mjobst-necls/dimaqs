#-------------------------------------------------
#
# Project created by QtCreator 2018-04-16T16:25:30
#
#-------------------------------------------------

QT -= core gui

TARGET = dimaqs
TEMPLATE = lib

DEFINES += LIBANUBIS_LIBRARY MYSQL_DYNAMIC_PLUGIN NO_EMBEDDED_ACCESS_CHECKS
DEFINES += DBUG_OFF DMYSQL_DYNAMIC_PLUGIN MYSQL_SERVER
LIBS += -L/source/mysql-5.7.28/libservices/
LIBS += -L/usr/local/lib/
LIBS += -lmysqlservices -lPetriNet
QMAKE_RPATHDIR += /usr/local/lib/

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -w

INCLUDEPATH += \
/source/mysql-5.7.28/include \
/source/mysql-5.7.28/sql \
/source/mysql-5.7.28/libbinlogevents/export \
/source/mysql-5.7.28/libbinlogevents/include \
/source/mysql-5.7.28/regex \
/usr/include/mysql \
/usr/include/mysql++

DESTDIR = $$top_builddir/bin
LIBS += -L$$top_builddir/bin

OBJECTS_DIR = $$PWD/obj

SOURCES += \
    AbstractClassifier.cpp \
    PetriNetClassifier.cpp \
    DatabasePetriNet.cpp \
    DatabasePetriNetData.cpp \
    transitionaction/data/AlwaysData.cpp \
    transitionaction/data/CreateTableData.cpp \
    transitionaction/data/DropDatabaseData.cpp \
    transitionaction/data/DropTableData.cpp \
    transitionaction/data/InsertValueData.cpp \
    transitionaction/data/ListColumnData.cpp \
    transitionaction/data/ListDatabaseData.cpp \
    transitionaction/data/ListTableData.cpp \
    transitionaction/data/ModifyTableData.cpp \
    transitionaction/AlwaysAction.cpp \
    transitionaction/CreateTableAction.cpp \
    transitionaction/DropDatabaseAction.cpp \
    transitionaction/DropTableAction.cpp \
    transitionaction/InsertAction.cpp \
    transitionaction/ListColumnAction.cpp \
    transitionaction/ListDatabaseAction.cpp \
    transitionaction/ListTableAction.cpp \
    transitionaction/ModifyTableAction.cpp \
    DatabaseTokenData.cpp \
    DatabasePetriNetQuery.cpp \
    TransitionHelper.cpp \
    placeaction/BackupAction.cpp \
    RelatedObjectTransitionCondition.cpp \
    dmaqs_plugin.cpp \
    dmaqs_udf.cpp \
    Controller.cpp \
    PlaceTimeoutThread.cpp \
    TriggerCreator.cpp \
    services.cc \
    dmaqs_variable.cpp \
    QueryRewriter.cpp \
    RewriteResult.cpp \
    dmaqs_global.cpp \
    placeaction/NotificationAction.cpp \
    TableFunctions.cpp \
    LiteralCollector.cpp \
    placeaction/CreateTriggerAction.cpp \
    placeaction/RewriteQueryAction.cpp \
    AdminHandler.cpp \
    transitionaction/SetVariableAction.cpp \
    transitionaction/data/SetVariableData.cpp \
    transitionaction/ShowVariableAction.cpp \
    transitionaction/data/ShowVariableData.cpp \
    placeaction/AdminModeAction.cpp

HEADERS +=\
    AbstractClassifier.h \
    PetriNetClassifier.h \
    DatabasePetriNet.h \
    DatabasePetriNetData.h \
    transitionaction/data/AlwaysData.h \
    transitionaction/data/CreateTableData.h \
    transitionaction/data/DropDatabaseData.h \
    transitionaction/data/DropTableData.h \
    transitionaction/data/InsertValueData.h \
    transitionaction/data/ListColumnData.h \
    transitionaction/data/ListDatabaseData.h \
    transitionaction/data/ListTableData.h \
    transitionaction/data/ModifyTableData.h \
    transitionaction/AlwaysAction.h \
    transitionaction/CreateTableAction.h \
    transitionaction/DropDatabaseAction.h \
    transitionaction/DropTableAction.h \
    transitionaction/InsertAction.h \
    transitionaction/ListColumnAction.h \
    transitionaction/ListDatabaseAction.h \
    transitionaction/ListTableAction.h \
    transitionaction/ModifyTableAction.h \
    DatabaseTokenData.h \
    DatabasePetriNetQuery.h \
    TransitionHelper.h \
    placeaction/BackupAction.h \
    RelatedObjectTransitionCondition.h \
    ValueComparision.h \
    dmaqs_plugin.h \
    Controller.h \
    dmaqs_global.h \
    dmaqs_variable.h \
    PlaceTimeoutThread.h \
    TriggerCreator.h \
    services.h \
    QueryRewriter.h \
    RewriteResult.h \
    placeaction/NotificationAction.h \
    TableFunctions.h \
    LiteralCollector.h \
    placeaction/CreateTriggerAction.h \
    placeaction/RewriteQueryAction.h \
    AdminHandler.h \
    transitionaction/SetVariableAction.h \
    transitionaction/data/SetVariableData.h \
    transitionaction/ShowVariableAction.h \
    transitionaction/data/ShowVariableData.h \
    placeaction/AdminModeAction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
