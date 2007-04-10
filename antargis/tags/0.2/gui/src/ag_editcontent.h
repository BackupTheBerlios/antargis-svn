#ifndef AG_EDITCONTENT_H
#define AG_EDITCONTENT_H

class AGEditElement
{
public:
  virtual ~AGEditElement(){}
  virtual size_t size()
  {
    return 0;
  }

  virtual int width()
  {
    return 0;
  }
  virtual int height()
  {
    return 0;
  }
  virtual void render(AGPainter &p,int x,int y)
  {
  }
  
};
class AGEditString:public AGEditElement
{
public:
  std::string s;
  AGFont font;
  
  AGEditString(const std::string &p,const AGFont &pFont):s(p),font(pFont)
  {
  }

  size_t size()
  {
    return s.size();
  }
  int width()
  {
    //    cdebug(s);
    return font.getWidth(s);
  }
  int height()
  {
    return font.getHeight(s);
  }
  int widthTil(size_t i)
  {
    assert(i<s.length());
    return font.getWidth(s.substr(0,i));
  }
  virtual void render(AGPainter &p,int x,int y)
  {
    p.renderText(s,AGVector2(x,y),font);
  }
};

class AGEditWhiteSpace:public AGEditElement
{
public:
  AGFont font;
  AGEditWhiteSpace(const AGFont &pFont):font(pFont)
  {
  }
  size_t size()
  {
    return 1;
  }
  int width()
  {
    //    cdebug(font.getWidth(" "));
    return font.getWidth(" ");
  }
  int height()
  {
    return font.getHeight(" ");
  }
};

class AGEditNewLine:public AGEditElement
{
public:
  size_t size()
  {
    return 1;
  }
};
class AGEditSurface:public AGEditElement
{
public:
  AGSurface surface;
  AGTexture texture;

  AGEditSurface(const AGSurface &pSurface):surface(pSurface)
  {
  }
  size_t size()
  {
    return 1;
  }
  int width()
  {
    return surface.width();
  }
  int height()
  {
    return surface.height();
  }
  void render(AGPainter &p,int x,int y)
  {
    texture=AGTexture(surface);
    p.blit(texture,AGRect2(x,y,texture.width(),texture.height()));
  }
};

class AGEditElement;

class AGEditContent
{
 public:
  typedef std::pair<int,int> CursorPos;

  void wrap(size_t lineStart);

  size_t toAt(const CursorPos &p);
  CursorPos toCP(size_t at);

  void clear();
  void setFont(const AGFont &pFont);
  void setText(const std::string &s);

  void insertSurface(const AGSurface &pSurface,size_t at);
  void insertChar(char c,size_t at);
  void insertWhiteSpace(size_t at);
  void removeChar(size_t at);
  void insertNewLine(size_t at);
  size_t size();

  Words getLine(size_t line);


    
 private:

  void split(size_t at);

  std::vector<AGEditElement*> mWords;
  std::vector<size_t> mLineStarts;
};

#endif

