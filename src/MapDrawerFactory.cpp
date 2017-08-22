#include <MapDrawerFactory.h>

// osm_navigatopr
#include <MapDrawer.h>
#include <MapDrawerGL.h>
#include <MapDrawerGS.h>
#include <MapDrawerQT.h>

// qt
#include <QWidget>
#include <QDebug>

///////////////////////////////////////////////////////////////////////////////////////
MapDrawerFactory::MapDrawerFactory()
{
}

///////////////////////////////////////////////////////////////////////////////////////
MapDrawerFactory::~MapDrawerFactory()
{
}

///////////////////////////////////////////////////////////////////////////////////////
MapDrawer* MapDrawerFactory::GetDrawer(QWidget* parent, int iMapDrawWidth, int iMapDrawHeight, unsigned int drawerType)
{
    if(drawerType > NUM_DRAWERS)
        return NULL;

    switch (drawerType)
    {
    case QT :
        return  new MapDrawerQT(parent, iMapDrawWidth, iMapDrawHeight);
        break;
    case OPENGL:
        return  new MapDrawerGL(parent, iMapDrawWidth, iMapDrawHeight);
        break;
     }
}













