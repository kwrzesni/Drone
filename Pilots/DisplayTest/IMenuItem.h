#ifndef IMENU_ITEM
#define IMENU_ITEM

#include <ILI9488.h>
#include "IAction.h"

class MenuManager;
class IMenu;

class IMenuItem
{
public:
  IMenuItem(IMenu& menu, IAction* action);
  void runAction();
  virtual void setPosition(int x, int y, int width, int height);
  virtual void draw(MenuManager& menuManager, bool isSelected) = 0;
  virtual void update(MenuManager& menuManager, const String& value) = 0;
  virtual String getValue() const = 0;
  virtual void setValue(String value) = 0;
  virtual String getText() const = 0;
  virtual ~IMenuItem();
protected:
  IMenu& menu;
  IAction* action = nullptr;
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;;
};

#endif
