#ifndef BUTTON_MENU_ITEM
#define BUTTON_MENU_ITEM

#include "IMenuItem.h"

class ButtonMenuItem : public IMenuItem 
{
public:
  ButtonMenuItem(IMenu& menu, IAction* action, const String& text);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
};

#endif
