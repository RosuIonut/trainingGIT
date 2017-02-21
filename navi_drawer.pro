#-------------------------------------------------
#
# Project created by QtCreator 2011-07-05T09:39:53
#
#-------------------------------------------------

QT +=	core \
        gui \
        dbus \
        opengl

TARGET = navi_drawer
TEMPLATE = app 


# The application is supported on multiple platforms, as listed in PLATFORM_LIST.
# The platform is selected by setting PLATFORM on the qmake command line, for example:
#   qmake "PLATFORM=pc"
#  
# PLATFORM is checked that it contains one and only one of the platforms from PLATFORM_LIST.
PLATFORM_LIST = pc rvc a3
PLATFORM = $$find(PLATFORM_LIST, $$PLATFORM)
!count(PLATFORM, 1): error(Set PLATFORM to one of [ $$PLATFORM_LIST ].)

SOURCES += src/main.cpp \
	src/TileServer.cpp \
	src/WaysServer.cpp \
	src/NodesServer.cpp \
	src/BoundariesServer.cpp \
	src/MapInfo.cpp \
	src/WaysList.cpp \
	src/NodesList.cpp \
	src/BoundariesList.cpp \
	src/MapDraw.cpp \
	src/NaviWindow.cpp \
	src/MapImageDataServer.cpp \
	src/DrawStyle.cpp \
	src/AreasServer.cpp \
	src/AreasList.cpp \
	src/MapRegion.cpp \
	src/WayInfo.cpp \
	src/NodePlaceServer.cpp \
	src/NodePlaceList.cpp \
	src/POIServer.cpp \ 
	src/POIList.cpp \
	src/MapController.cpp \
	src/PosDotDraw.cpp \
	src/GpsServiceIf.cpp \
	src/AddressList.cpp \
	src/AddressServer.cpp \
	menu/src/MainMenu.cpp \
	menu/src/MenuButton.cpp \
	menu/src/SearchAddress.cpp \
	menu/src/MenuServer.cpp \
	menu/src/SearchCountry.cpp \
	menu/src/SearchCity.cpp \
	menu/src/SearchStreet.cpp \
	menu/src/SearchStreetNo.cpp \
	menu/src/OnScreenKeyboard.cpp \
	menu/src/SearchPOI.cpp \
        src/MapDrawerQT.cpp \
        src/MapDrawerGL.cpp \
        src/MapFBODataServer.cpp \
# LibUtils
	$$PWD/../libUtils/DataSerializers.c \
	$$PWD/../libUtils/DataStructures.c \
	$$PWD/../libUtils/FileUtils.c \
	$$PWD/../libUtils/MapUtils.c \
	$$PWD/../libUtils/StringParsers.c \
        src/MapDrawerGS.cpp \
        src/MapDrawerFactory.cpp \
        src/MapDrawer.cpp

HEADERS += headers/DataTypes.h \
	headers/DrawStyle.h \
	headers/MapImageDataServer.h \
	headers/MapDraw.h \
	headers/MapInfo.h \
	headers/MapSettings.h \
	headers/NaviWindow.h \
	headers/NodesList.h \
	headers/NodesServer.h \
	headers/BoundariesServer.h \
	headers/TileServer.h \
	headers/WaysList.h \
	headers/WaysServer.h\
	headers/AreasServer.h \
	headers/AreasList.h \
	headers/BoundariesList.h \
	headers/MapRegion.h \
	headers/WayInfo.h \
	headers/NodePlaceList.h \
	headers/NodePlaceServer.h \
	headers/POIServer.h \ 
	headers/POIList.h \
	headers/MapController.h \
	headers/PosDotDraw.h \
	headers/GpsServiceIf.h \
	headers/AddressList.h \ 
	headers/AddressServer.h \
	menu/headers/MainMenu.h \
	menu/headers/MenuButton.h \
	menu/headers/SearchAddress.h \
	menu/headers/MenuServer.h \
	menu/headers/SearchCountry.h \
	menu/headers/SearchCity.h \
	menu/headers/SearchStreet.h \
	menu/headers/SearchStreetNo.h \
	menu/headers/OnScreenKeyboard.h \
	menu/headers/SearchPOI.h \
                headers/MapDrawerQT.h \
                headers/MapDrawerGL.h \
                headers/MapFBODataServer.h \
                headers/MapDrawerGS.h \
                headers/MapDrawer.h \
                headers/MapDrawerFactory.h
	

INCLUDEPATH += headers \
	   $$PWD/../include

contains(PLATFORM, rvc): INCLUDEPATH += \
	$$(ROOTFS_RVC)/usr/include \
	$$(ROOTFS_RVC)/usr/src/linux/include

contains(PLATFORM, a3): INCLUDEPATH += \
	$$(ROOTFS_A3)/usr/include \
	$$(ROOTFS_A3)/usr/src/linux/include

DEPENDPATH += $$PWD/../libUtils

LIBS += -L../libUtils -lmap_utils #-lGLU

contains(PLATFORM, rvc): LIBS += \
	-L$$(ROOTFS_RVC)/usr/lib \
	-L$$(ROOTFS_RVC)/lib \
	-lasound \
	-lz \
	-lpng \
	-lSM \
	-lICE \
	-lXrender \
	-lfontconfig \
	-lXext \
	-lX11 \
	-lxcb \
 	-lXdmcp \
	-lxcb-xlib \
	-lXau \
	-lexpat \
	-lfreetype \
	-lQtGui 
 
contains(PLATFORM, a3): LIBS += \
	-L$$(ROOTFS_A3)/usr/lib \
	-L$$(ROOTFS_A3)/lib \
#	-lasound \
	-lz \
	-lpng \
#	-lSM \
#	-lICE \
#	-lXrender \
#	-lfontconfig \
#	-lXext \
#	-lX11 \
#	-lxcb \
#	-lXdmcp \
#	-lxcb-xlib \
#	-lXau \
#	-lexpat \
	-lts \
	-lfreetype \
	-lQtGui 
 
# Static linkage at the moment
PRE_TARGETDEPS += ../libUtils/libmap_utils.a

 #use flags for debug
QMAKE_CFLAGS = -g
QMAKE_CXXFLAGS = $$QMAKE_CFLAGS
