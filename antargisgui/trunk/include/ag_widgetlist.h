#ifndef AG_WIDGETLIST_H
#define AG_WIDGETLIST_H

class AGWidget;

class AGWidgetList
{
 public:
  AGWidgetList();
  
  typedef std::list<AGWidget*>::iterator iterator;
  iterator begin();
  iterator end();
  typedef std::list<AGWidget*>::reverse_iterator reverse_iterator;
  reverse_iterator rbegin();
  reverse_iterator rend();

  AGWidget *top();
  AGWidget *bottom();

 private:
  std::list<AGWidget*> mChildren;
};

#endif
