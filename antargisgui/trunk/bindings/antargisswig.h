/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.24
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_Libantargisruby_WRAP_H_
#define SWIG_Libantargisruby_WRAP_H_

class Swig::Director;


class SwigDirector_AGSurface : public AGSurface, public Swig::Director {

public:
    SwigDirector_AGSurface(VALUE self);
    SwigDirector_AGSurface(VALUE self, int w, int h);
    SwigDirector_AGSurface(VALUE self, AGSurface const &p);
    virtual ~SwigDirector_AGSurface();
    virtual void putPixel(int x, int y, AGColor const &c);
    virtual void blit(AGSurface const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void blit(AGSurface const &pSource, AGRect const &pDest, AGRect const &pSrc, AGColor const &pColor);
};


class SwigDirector_AGScreen : public AGScreen, public Swig::Director {

public:
    SwigDirector_AGScreen(VALUE self);
    virtual void flip();
};


class SwigDirector_AGSDLScreen : public AGSDLScreen, public Swig::Director {

public:
    SwigDirector_AGSDLScreen(VALUE self, SDL_Surface *s);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual AGSurface loadSurface(std::string const &pFilename);
    virtual void tile(AGTexture const &pSource);
    virtual void drawGradientAlpha(AGRect const &rect, AGColor const &ul, AGColor const &ur, AGColor const &dl, AGColor const &dr);
    virtual void drawLine(AGPoint const &p0, AGPoint const &p1, AGColor const &c);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest);
    virtual SDL_Surface *newSurface(int x, int y);
    virtual void drawBorder(AGRect const &rect, int W, AGColor const &c1, AGColor const &c2);
    virtual void putPixel(int x, int y, AGColor const &c);
    virtual void drawRect(AGRect const &pRect, AGColor const &c);
    virtual void flip();
};


class SwigDirector_AGEvent : public AGEvent, public Swig::Director {

public:
    SwigDirector_AGEvent(VALUE self, AGListener *pCaller);
    virtual ~SwigDirector_AGEvent();
};


class SwigDirector_AGSDLEvent : public AGSDLEvent, public Swig::Director {

public:
    SwigDirector_AGSDLEvent(VALUE self, AGListener *pCaller, SDL_Event const *pEvent);
    virtual ~SwigDirector_AGSDLEvent();
};


class SwigDirector_AGMouseEvent : public AGMouseEvent, public Swig::Director {

public:
    SwigDirector_AGMouseEvent(VALUE self, AGListener *pCaller, SDL_Event *pEvent);
    virtual ~SwigDirector_AGMouseEvent();
};


class SwigDirector_AGListener : public AGListener, public Swig::Director {

public:
    SwigDirector_AGListener(VALUE self);
    virtual ~SwigDirector_AGListener();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGCPPListener : public AGCPPListener, public Swig::Director {

public:
    SwigDirector_AGCPPListener(VALUE self);
    virtual ~SwigDirector_AGCPPListener();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller) const;
};


class SwigDirector_AGSignal : public AGSignal, public Swig::Director {

public:
    SwigDirector_AGSignal(VALUE self);
    SwigDirector_AGSignal(VALUE self, AGMessageObject *pCaller);
    SwigDirector_AGSignal(VALUE self, AGMessageObject *pCaller, std::string const &pName);
    virtual ~SwigDirector_AGSignal();
};


class SwigDirector_AGMessageObject : public AGMessageObject, public Swig::Director {

public:
    SwigDirector_AGMessageObject(VALUE self);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual ~SwigDirector_AGMessageObject();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGWidget : public AGWidget, public Swig::Director {

public:
    SwigDirector_AGWidget(VALUE self, AGWidget *pParent, AGRect const &r);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGWidget();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGButton : public AGButton, public Swig::Director {

public:
    SwigDirector_AGButton(VALUE self, AGWidget *pParent, AGRect const &r, std::string const &pText, int id = -1);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual std::string getCaption() const;
    virtual ~SwigDirector_AGButton();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGApplication : public AGApplication, public Swig::Director {

public:
    SwigDirector_AGApplication(VALUE self);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m2);
    virtual bool eventFrame(float pTime);
    virtual bool eventFrameEnd(float pTime);
    virtual ~SwigDirector_AGApplication();
    virtual bool eventIdle();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGText : public AGText, public Swig::Director {

public:
    SwigDirector_AGText(VALUE self, AGWidget *pParent, AGRect const &pRect, std::string const &pText, AGFont const &pFont);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGText();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGVTiler : public AGVTiler, public Swig::Director {

public:
    SwigDirector_AGVTiler(VALUE self, AGWidget *pParent, AGRect const &pRect, bool pAdaptMyHeight = true);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGVTiler();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void removeChild(AGWidget *w);
    virtual void addChild(AGWidget *pWidget);
};


class SwigDirector_AGHTiler : public AGHTiler, public Swig::Director {

public:
    SwigDirector_AGHTiler(VALUE self, AGWidget *pParent, AGRect const &pRect, bool pAdaptMyHeight = true);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGHTiler();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void removeChild(AGWidget *w);
    virtual void addChild(AGWidget *pWidget);
};


class SwigDirector_AGTheme : public AGTheme, public Swig::Director {

public:
    SwigDirector_AGTheme(VALUE self);
    virtual ~SwigDirector_AGTheme();
};


class SwigDirector_AGMenuEvent : public AGMenuEvent, public Swig::Director {

public:
    SwigDirector_AGMenuEvent(VALUE self, AGListener *pListener, std::string const &pName);
    virtual ~SwigDirector_AGMenuEvent();
};


class SwigDirector_AGMenu : public AGMenu, public Swig::Director {

public:
    SwigDirector_AGMenu(VALUE self, AGWidget *pParent, AGPoint pWishPos, std::string const &pName);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGMenu();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void removeChild(AGWidget *w);
    virtual void addChild(AGWidget *pWidget);
};


class SwigDirector_AGScreenWidget : public AGScreenWidget, public Swig::Director {

public:
    SwigDirector_AGScreenWidget(VALUE self);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual ~SwigDirector_AGScreenWidget();
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGEditLine : public AGEditLine, public Swig::Director {

public:
    SwigDirector_AGEditLine(VALUE self, std::string const &pText, AGFont pFont, bool pHardEnd);
    virtual ~SwigDirector_AGEditLine();
    virtual void drawCursor(AGPainter &p, int cx, AGPoint const &pPoint, AGRect const &pClip, AGColor const &c);
    virtual void draw(AGPainter &p, AGPoint const &pPoint, AGRect const &pClip);
};


class SwigDirector_AGEdit : public AGEdit, public Swig::Director {

public:
    SwigDirector_AGEdit(VALUE self, AGWidget *pParent, AGRect const &pRect);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGEdit();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void drawBackground(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGCheckBox : public AGCheckBox, public Swig::Director {

public:
    SwigDirector_AGCheckBox(VALUE self, AGWidget *pParent, AGRect pRect);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual std::string getCaption() const;
    virtual ~SwigDirector_AGCheckBox();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGRadioGroup : public AGRadioGroup, public Swig::Director {

public:
    SwigDirector_AGRadioGroup(VALUE self, AGWidget *pParent, AGRect const &pr);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual void eventChange(std::string const &p);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual ~SwigDirector_AGRadioGroup();
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGRadio : public AGRadio, public Swig::Director {

public:
    SwigDirector_AGRadio(VALUE self, AGWidget *pParent, AGRect pRect);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual std::string getCaption() const;
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual ~SwigDirector_AGRadio();
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGTable : public AGTable, public Swig::Director {

public:
    SwigDirector_AGTable(VALUE self, AGWidget *pWidget, AGRect const &pRect);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual ~SwigDirector_AGTable();
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGWindow : public AGWindow, public Swig::Director {

public:
    SwigDirector_AGWindow(VALUE self, AGWidget *pWidget, AGRect const &pRect, std::string const &pTitle = "");
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGWindow();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void addChildBack(AGWidget *w);
    virtual void removeChild(AGWidget *w);
    virtual void addChild(AGWidget *w);
};


class SwigDirector_AGImage : public AGImage, public Swig::Director {

public:
    SwigDirector_AGImage(VALUE self, AGWidget *pParent, AGPoint const &p, AGSurface pSurface, bool pTile, AGRect const &pRect = AGRect(0,0,0,0));
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual ~SwigDirector_AGImage();
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGCaption : public AGCaption, public Swig::Director {

public:
    SwigDirector_AGCaption(VALUE self, AGWidget *pParent, AGRect const &pRect, std::string const &pText, AGFont const &pFont, AGBackground const &pBG);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGCaption();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGLayout : public AGLayout, public Swig::Director {

public:
    SwigDirector_AGLayout(VALUE self, AGWidget *pgParent, std::string const &pXMLData);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGLayout();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGLayoutCreator : public AGLayoutCreator, public Swig::Director {

public:
    SwigDirector_AGLayoutCreator(VALUE self, std::string const &pName);
    virtual AGWidget *create(AGWidget *pParent, AGRect const &pRect, xmlpp::Node const &pNode);
};


class SwigDirector_AGSound : public AGSound, public Swig::Director {

public:
    SwigDirector_AGSound(VALUE self);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGSound();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGListBox : public AGListBox, public Swig::Director {

public:
    SwigDirector_AGListBox(VALUE self, AGWidget *pParent, AGRect const &pRect);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual ~SwigDirector_AGListBox();
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(std::string const &pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void drawAll(AGPainter &p);
    virtual void draw(AGPainter &p);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual void removeChild(AGWidget *w);
};


class SwigDirector_AGPainter : public AGPainter, public Swig::Director {

public:
    SwigDirector_AGPainter(VALUE self);
    SwigDirector_AGPainter(VALUE self, AGPainter const &p);
    SwigDirector_AGPainter(VALUE self, AGPaintTarget &pTarget);
    virtual void blit(AGSurface const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc, AGColor const &pColor);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest, AGColor const &pColor);
    virtual void tile(AGSurface const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void tile(AGTexture const &pSource);
    virtual ~SwigDirector_AGPainter();
    virtual void drawLine(AGPoint const &p0, AGPoint const &p1, AGColor const &c);
    virtual void blit(AGSurface const &pSource, AGRect const &pDest);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest);
    virtual void tile(AGSurface const &pSource, AGRect const &pDest);
    virtual void tile(AGSurface const &pSource);
    virtual void putPixel(AGPoint const &p, AGColor const &c);
    virtual void drawRect(AGRect const &pRect, AGColor const &c);
    virtual void blitTri(AGTexture const &pSource, AGTriangle const &pSrc, AGTriangle const &pDest);
};


#endif
