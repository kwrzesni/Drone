#ifndef VALUE_MENU_ITEM
#define VALUE_MENU_ITEM

#include "IMenuItem.h"

class ValueMenuItem : public IMenuItem
{
public:
  ValueMenuItem(Action action, const String& value);
  void calculateSize(MenuManager& menuManager, int maxWidth) override;
  void draw(MenuManager& menuManager, bool isSelected) override;
  void redraw(MenuManager& menuManager, bool isSelected) override;
  String getValue() const override;
  String getText() const override;
  bool setValue(MenuManager& menuManager, String value) override;
  bool updateValue(MenuManager& menuManager, const String& value) override;
  bool isValidValue(const String& value) override;
private:
  String value;
  String previousText = "";
};

#endif
