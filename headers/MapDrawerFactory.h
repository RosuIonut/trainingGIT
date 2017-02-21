#ifndef MAPDRAWERFACTORY_H
#define MAPDRAWERFACTORY_H

class QWidget;
class MapDrawer;

class MapDrawerFactory
{
///////////////////////////////////////////////////////////////////////////////////////
public:
    enum DrawerType
    {
        QT = 0,
        OPENGL,
        GRAPHIC_SCENE,
        NUM_DRAWERS
    };

//////////////////////////////////////////////////////////////////////////////////////////////////////////
public: // methods
    MapDrawerFactory();
    virtual ~MapDrawerFactory();

    MapDrawer* GetDrawer(QWidget* parent, int iMapDrawWidth, int iMapDrawHeight, unsigned int drawerType);
};

#endif // MAPDRAWERFACTORY_H





























