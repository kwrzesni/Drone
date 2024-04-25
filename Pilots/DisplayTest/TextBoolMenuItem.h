#ifndef TEXT_BOOL_MENU_ITEM
#define TEXT_BOOL_MENU_ITEM

#include "IMenuItem.h"

class TextBoolMenuItem : public IMenuItem
{
public:
  TextBoolMenuItem(IMenu& menu, IAction* action, const String& text, bool& value);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
  bool& value;
};

#endif
