#ifndef OSGVIEW_H
#define OSGVIEW_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QMenu>

#include <osgViewer/Viewer>
#include <osgUtil/IntersectionVisitor>
#include <osg/PolygonMode>

#include "ViewingCore.h"

class OsgItemModel;

class Osg3dView : public QOpenGLWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:
    Osg3dView(QWidget * parent = 0);

    enum MouseMode {
        MM_ORBIT = (1<<1),
        MM_PAN = (1<<2),
        MM_ZOOM = (1<<3),
        MM_ROTATE = (1<<4),
        MM_PICK_CENTER = (1<<5),
        MM_SELECT = (1<<6)
    };
    enum StandardView {
        V_TOP = (1<<0),
        V_BOTTOM = (1<<1),
        V_FRONT = (1<<2),
        V_BACK = (1<<3),
        V_RIGHT = (1<<4),
        V_LEFT = (1<<5)
    };
    enum Projection {
        P_ORTHO = (1<<0),
        P_PERSP = (1<<1)
    };


    /// Let others tell what scene graph we should be drawing
    void setScene(osg::Node *root);

    osg::ref_ptr<ViewingCore> getViewingCore() const { return m_viewingCore; }

    static QString mouseModeDescription(Osg3dView::MouseMode mouseMode);
public slots:
    void paintGL();
    void resizeGL(int w, int h);
    void hello();
    /// Set the m_mouseMode variable
    void setMouseMode(MouseMode mode);
    void setMouseMode();
    void setStandardView();
    void setDrawMode(osg::PolygonMode::Mode drawMode = (osg::PolygonMode::Mode)0);
    void setProjection();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void customMenuRequested(const QPoint &pos);

signals:
    /// Let the rest of the world (OsgView) know the current MouseMode
    void mouseModeChanged(Osg3dView::MouseMode);
    void updated();
    void pickObject(QVector< osg::ref_ptr<osg::Node> > nodePath);
    void nodePathClicked(osg::NodePath);
    void toggleToolBar();
    void toggleMenuBar();

private:
    osg::Vec2d getNormalized(const int ix, const int iy);

    /// OSG uses singleSided drawing/display by default.
    /// This is annoying when you are "inside" something and the back wall of
    /// it simply disappears. This gets called to set up to draw both front and
    /// back facing polygons so that the world behaves in a more normal fashion.
    /// Yes it's a performance hit.
    void setLightingTwoSided();
    void buildPopupMenu();

    /// fast convenience routine to get the first child of the root
    osg::Node *getLoadedModel();

    /// given that something was clicked, announce it to the world so
    /// that other classes can pay attention to the same object
    void pickAnObjectFromView();


    /// Each time the mouse is clicked we do a LineSegmentIntersector
    /// The results are stashed in m_clickIntersector.  Various
    /// methods allow convenient queries about what (if anything) was
    /// found as a result of this click
    /// XXX Some of this may be worth exposing to OsgForm at some point
    void findObjectsUnderMouseEvent(); ///< calls LineSegmentIntersector

    /// just how many things were found under the mouse event?
    unsigned numberOfIntersections();

    osg::ref_ptr<osgUtil::LineSegmentIntersector> m_clickIntersector;

    /// Find the first loaded item at the click location
    osg::NodePath getFirstLoadedItemClicked();

    /// find the very first thing clicked (if any) so we can manipulate controls
    osg::NodePath getFirstItemClicked();

    /// was it loaded or a control?  Note:  returns false if nothing clicked
    bool firstItemClickedWasLoaded();



    QMenu m_popupMenu;

    /// OSG graphics window
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_osgGraphicsWindow;

    /// Camera manager
    osg::ref_ptr<ViewingCore> m_viewingCore;

    /// Current mouse mode
    MouseMode m_mouseMode;

    bool m_mouseIsPressed;
    int m_timeToDrawLastFrame;
    osg::Vec2d m_savedEventNDCoords;

    // since we always want the 3D view to have focus when entered
    // it is polite to remember who had the focus when we stole it
    // and restore it when the cursor leaves the window
    QWidget *m_lastFocused;





};

#endif // OSGVIEW_H
