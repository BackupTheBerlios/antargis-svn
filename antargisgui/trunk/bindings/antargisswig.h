/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.22
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
    SwigDirector_AGSurface(VALUE self, SDL_Surface *s, int w, int h, bool disown = false);
    SwigDirector_AGSurface(VALUE self, SDL_Surface *s, bool disown = false);
    SwigDirector_AGSurface(VALUE self, bool disown = false);
    SwigDirector_AGSurface(VALUE self, int w, int h, bool disown = false);
    SwigDirector_AGSurface(VALUE self, AGSurface const &p, bool disown = false);
};


class SwigDirector_AGTexture : public AGTexture, public Swig::Director {

public:
    SwigDirector_AGTexture(VALUE self, bool disown = false);
    SwigDirector_AGTexture(VALUE self, AGSurface const &pSurface, bool disown = false);
    SwigDirector_AGTexture(VALUE self, AGSurface const &pSurface, int W, int H, bool disown = false);
};


class SwigDirector_AGPainter : public AGPainter, public Swig::Director {

public:
    SwigDirector_AGPainter(VALUE self, bool disown = true);
    virtual void putPixel(int x, int y, AGColor const &c);
};


class SwigDirector_AGSurfacePainter : public AGSurfacePainter, public Swig::Director {

public:
    SwigDirector_AGSurfacePainter(VALUE self, AGSurface &pSurface, bool disown = false);
    virtual void blit(AGSurface const &pSource, AGRect const &pDest);
    virtual void tile(AGSurface const &pSource);
    virtual void tile(AGSurface const &pSource, AGRect const &pDest);
    virtual void putPixel(int x, int y, AGColor const &c);
    virtual void blit(AGSurface const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void tile(AGSurface const &pSource, AGRect const &pDest, AGRect const &pSrc);
};


class SwigDirector_AGTexturePainter : public AGTexturePainter, public Swig::Director {

public:
    SwigDirector_AGTexturePainter(VALUE self, AGTexture &pSurface, bool disown = false);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest);
    virtual void putPixel(int x, int y, AGColor const &c);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual void tile(AGTexture const &pSource);
};


class SwigDirector_AGGScreen : public AGGScreen, public Swig::Director {

public:
    SwigDirector_AGGScreen(VALUE self, bool disown = true);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual AGSurface loadSurface(std::string const &pFilename);
    virtual void tile(AGTexture const &pSource);
    virtual void drawGradientAlpha(AGRect const &rect, AGColor const &ul, AGColor const &ur, AGColor const &dl, AGColor const &dr);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest);
    virtual SDL_Surface *newSurface(int x, int y);
    virtual void drawBorder(AGRect const &rect, int W, AGColor const &c1, AGColor const &c2);
    virtual void putPixel(int x, int y, AGColor const &c);
    virtual void drawRect(AGRect const &pRect, AGColor const &c);
    virtual void flip();
};


class SwigDirector_AGSDLScreen : public AGSDLScreen, public Swig::Director {

public:
    SwigDirector_AGSDLScreen(VALUE self, SDL_Surface *s, bool disown = false);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest, AGRect const &pSrc);
    virtual AGSurface loadSurface(std::string const &pFilename);
    virtual void tile(AGTexture const &pSource);
    virtual void drawGradientAlpha(AGRect const &rect, AGColor const &ul, AGColor const &ur, AGColor const &dl, AGColor const &dr);
    virtual void blit(AGTexture const &pSource, AGRect const &pDest);
    virtual void tile(AGTexture const &pSource, AGRect const &pDest);
    virtual void flip();
    virtual void drawRect(AGRect const &pRect, AGColor const &c);
    virtual void drawBorder(AGRect const &rect, int W, AGColor const &c1, AGColor const &c2);
    virtual void putPixel(int x, int y, AGColor const &c);
    virtual SDL_Surface *newSurface(int x, int y);
};


class SwigDirector_AGListener : public AGListener, public Swig::Director {

public:
    SwigDirector_AGListener(VALUE self, bool disown = false);
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGCPPListener : public AGCPPListener, public Swig::Director {

public:
    SwigDirector_AGCPPListener(VALUE self, bool disown = true);
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller) const;
};


class SwigDirector_AGMessageObject : public AGMessageObject, public Swig::Director {

public:
    SwigDirector_AGMessageObject(VALUE self, bool disown = false);
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
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGWidget : public AGWidget, public Swig::Director {

public:
    SwigDirector_AGWidget(VALUE self, AGWidget *pParent, AGRect const &r, bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChild(AGWidget *w);
    virtual void addChildBack(AGWidget *w);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void draw(AGRect const &r);
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void setTop(int y);
    virtual void setLeft(int x);
};


class SwigDirector_AGButton : public AGButton, public Swig::Director {

public:
    SwigDirector_AGButton(VALUE self, AGWidget *pParent, AGRect const &r, std::string const &pText, int id = -1, std::string const &pStyle = "button", bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual std::string getName() const;
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
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
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void draw(AGRect const &r);
    virtual void drawAll(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
};


class SwigDirector_AGApplication : public AGApplication, public Swig::Director {

public:
    SwigDirector_AGApplication(VALUE self, bool disown = false);
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
    virtual bool eventIdle();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGText : public AGText, public Swig::Director {

public:
    SwigDirector_AGText(VALUE self, AGWidget *pParent, AGRect const &pRect, std::string const &pText, AGFont const &pFont, bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChild(AGWidget *w);
    virtual void addChildBack(AGWidget *w);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual void draw(AGRect const &r);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void setTop(int y);
    virtual void setLeft(int x);
};


class SwigDirector_AGVTiler : public AGVTiler, public Swig::Director {

public:
    SwigDirector_AGVTiler(VALUE self, AGWidget *pParent, AGRect const &pRect, bool pAdaptMyHeight = true, bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
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
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void drawAll(AGRect const &r);
    virtual void draw(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChild(AGWidget *pWidget);
    virtual void addChildBack(AGWidget *w);
};


class SwigDirector_AGHTiler : public AGHTiler, public Swig::Director {

public:
    SwigDirector_AGHTiler(VALUE self, AGWidget *pParent, AGRect const &pRect, bool pAdaptMyHeight = true, bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
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
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void drawAll(AGRect const &r);
    virtual void draw(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChild(AGWidget *pWidget);
    virtual void addChildBack(AGWidget *w);
};


class SwigDirector_AGTheme : public AGTheme, public Swig::Director {

public:
    SwigDirector_AGTheme(VALUE self, bool disown = false);
};


class SwigDirector_AGMenu : public AGMenu, public Swig::Director {

public:
    SwigDirector_AGMenu(VALUE self, AGWidget *pParent, AGPoint pWishPos, std::string const &pName, bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
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
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void drawAll(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void draw(AGRect const &r);
    virtual void addChild(AGWidget *pWidget);
    virtual void addChildBack(AGWidget *w);
};


class SwigDirector_AGScreenWidget : public AGScreenWidget, public Swig::Director {

public:
    SwigDirector_AGScreenWidget(VALUE self, bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChild(AGWidget *w);
    virtual void addChildBack(AGWidget *w);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual void draw(AGRect const &r);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void setTop(int y);
    virtual void setLeft(int x);
};


class SwigDirector_AGEditLine : public AGEditLine, public Swig::Director {

public:
    SwigDirector_AGEditLine(VALUE self, std::string const &pText, AGFont pFont, bool pHardEnd, bool disown = false);
    virtual void draw(AGPoint const &pPoint, AGRect const &pClip);
    virtual void drawCursor(int cx, AGPoint const &pPoint, AGRect const &pClip, AGColor const &c);
};


class SwigDirector_AGEdit : public AGEdit, public Swig::Director {

public:
    SwigDirector_AGEdit(VALUE self, AGWidget *pParent, AGRect const &pRect, bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
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
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void drawBackground(AGRect const &pRect);
    virtual void draw(AGRect const &pRect);
    virtual void drawAll(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
};


class SwigDirector_AGCheckBox : public AGCheckBox, public Swig::Director {

public:
    SwigDirector_AGCheckBox(VALUE self, AGWidget *pParent, AGRect pRect, std::string const &pName, std::string const &pType = "checkBox", bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void drawAll(AGRect const &r);
    virtual void draw(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChild(AGWidget *pWidget);
    virtual void addChildBack(AGWidget *w);
};


class SwigDirector_AGRadioGroup : public AGRadioGroup, public Swig::Director {

public:
    SwigDirector_AGRadioGroup(VALUE self, bool disown = false);
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
    virtual void eventChange(std::string const &p);
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
};


class SwigDirector_AGRadio : public AGRadio, public Swig::Director {

public:
    SwigDirector_AGRadio(VALUE self, AGWidget *pParent, AGRect pRect, AGRadioGroup *pGroup, std::string pName, bool disown = false);
    virtual bool eventLostFocus();
    virtual bool eventGotFocus();
    virtual bool eventShow();
    virtual void rePosition();
    virtual int minHeight() const;
    virtual int minWidth() const;
    virtual bool canFocus() const;
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventActive(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setLeft(int x);
    virtual void setTop(int y);
    virtual void setHeight(int w);
    virtual void setWidth(int w);
    virtual void drawAll(AGRect const &r);
    virtual void draw(AGRect const &r);
    virtual bool eventMouseLeave();
    virtual bool eventHide();
    virtual void addChild(AGWidget *pWidget);
    virtual void addChildBack(AGWidget *w);
};


class SwigDirector_AGTable : public AGTable, public Swig::Director {

public:
    SwigDirector_AGTable(VALUE self, AGWidget *pWidget, AGRect const &pRect, bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChild(AGWidget *w);
    virtual void addChildBack(AGWidget *w);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void draw(AGRect const &r);
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setTop(int y);
    virtual void setLeft(int x);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
};


class SwigDirector_AGWindow : public AGWindow, public Swig::Director {

public:
    SwigDirector_AGWindow(VALUE self, AGWidget *pWidget, AGRect const &pRect, std::string const &pTitle = "", bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChildBack(AGWidget *w);
    virtual void addChild(AGWidget *w);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void draw(AGRect const &r);
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setTop(int y);
    virtual void setLeft(int x);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
};


class SwigDirector_AGImage : public AGImage, public Swig::Director {

public:
    SwigDirector_AGImage(VALUE self, AGWidget *pParent, AGPoint const &p, AGSurface pSurface, bool pTile, AGRect const &pRect = AGRect(0,0,0,0), bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChild(AGWidget *w);
    virtual void addChildBack(AGWidget *w);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual void draw(AGRect const &r);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void setTop(int y);
    virtual void setLeft(int x);
};


class SwigDirector_AGCaption : public AGCaption, public Swig::Director {

public:
    SwigDirector_AGCaption(VALUE self, AGWidget *pParent, AGRect const &pRect, std::string const &pText, AGFont const &pFont, AGBackground const &pBG, bool disown = false);
    virtual bool eventResize(AGEvent const *m);
    virtual bool eventSysWM(AGEvent const *m);
    virtual bool eventQuitModal(AGEvent const *m);
    virtual bool eventQuit(AGEvent const *m);
    virtual bool eventKeyUp(AGEvent const *m);
    virtual bool eventKeyDown(AGEvent const *m);
    virtual bool eventActive(AGEvent const *m);
    virtual bool acceptEvent(SDL_Event const *pEvent);
    virtual bool eventMouseMotion(AGEvent const *m);
    virtual bool eventMouseClick(AGEvent const *m);
    virtual bool eventMouseButtonDown(AGEvent const *m);
    virtual bool eventMouseButtonUp(AGEvent const *m);
    virtual void addChild(AGWidget *w);
    virtual void addChildBack(AGWidget *w);
    virtual bool eventMouseEnter();
    virtual bool eventGotFocus();
    virtual bool eventLostFocus();
    virtual bool eventHide();
    virtual bool eventMouseLeave();
    virtual bool canFocus() const;
    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual void drawAll(AGRect const &r);
    virtual bool eventDragBy(AGEvent const *event, AGPoint const &pDiff);
    virtual void draw(AGRect const &pRect);
    virtual bool eventShow();
    virtual bool signal(char const *pName, AGEvent const *m, AGMessageObject *pCaller);
    virtual void setWidth(int w);
    virtual void setHeight(int w);
    virtual void setTop(int y);
    virtual void setLeft(int x);
};


#endif
