#ifndef TEXT_VALUE_MENU_ITEM
#define TEXT_VALUE_MENU_ITEM

#include "IMenuItem.h"

class TextValueMenuItem : public IMenuItem
{
public:
  TextValueMenuItem(IMenu& menu, IAction* action, const String& text, const String& value);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const;
private:
  String text;
  String value;
};

#endif
