#ifndef BUTTON_MENU_ITEM
#define BUTTON_MENU_ITEM

#include "IMenuItem.h"

class ButtonMenuItem : public IMenuItem 
{
public:
  ButtonMenuItem(Action action, const String& text);
  void calculateSize(MenuManager& menuManager, int maxWidth) override;
  void draw(MenuManager& menuManager, bool isSelected) override;
  void redraw(MenuManager& menuManager, bool isSelected) override;
  String getValue() const override;
  String getText() const override;
  bool setValue(MenuManager& menuManager, String value) override;
  bool updateValue(MenuManager& menuManager, const String& value) override;
  bool isValidValue(const String& value) override;
private:
  String text;
};

#endif
