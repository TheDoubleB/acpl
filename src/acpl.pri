# Qt Toolkit include

BUILD_TYPE =
win32:Debug {
	BUILD_TYPE = Debug
}
win32:Release {
	BUILD_TYPE = Release
}

win32:LIBS += -L$$(ACPL_LIB_ROOT)/acpl/lib/$${BUILD_TYPE}
unix:LIBS += -L$${PWD}/../lib
LIBS += -lacpl

win32:INCLUDEPATH+= $$(ACPL_LIB_ROOT)/acpl/include
unix:INCLUDEPATH += $${PWD}/../include
