#ifndef VALUE_MENU_ITEM
#define VALUE_MENU_ITEM

#include "IMenuItem.h"

class ValueMenuItem : public IMenuItem
{
public:
  ValueMenuItem(IMenu& menu, IAction* action, const String& value);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String value;
};

#endif
