TEMPLATE = app
QT += quick

SOURCES = main.cpp
RESOURCES = hellopathitem.qrc
OTHER_FILES = main.qml

target.path = $$[QT_INSTALL_EXAMPLES]/hellopathitem
INSTALLS += target
