#ifndef IMENU_ITEM
#define IMENU_ITEM

#include <ILI9488.h>
#include "Action.h"

class MenuManager;

class IMenuItem
{
public:
  IMenuItem(Action action);
  const Action& getAction() const;
  int getX() const;
  int getY() const;
  int getWidth() const;
  int getHeight() const;
  void setPosition(int x, int y);
  virtual void setSize(int width, int height);
  virtual void trim(MenuManager& menuManager, int maxHeight);
  virtual void calculateSize(MenuManager& menuManager, int maxWidth) = 0;
  virtual void draw(MenuManager& menuManager, bool isSelected) = 0;
  virtual void redraw(MenuManager& menuManager, bool isSelected) = 0;
  virtual String getValue() const = 0;
  virtual String getText() const = 0;
  virtual bool setValue(MenuManager& menuManager, String value) = 0;
  virtual bool updateValue(MenuManager& menuManager, const String& value) = 0;
  virtual bool isValidValue(const String& value) = 0;
  virtual ~IMenuItem() = default;
protected:
  Action action;
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  int maxWidth = 0;
  int trimmedHeight = 0;
  bool trimmed = false;
};

#endif
