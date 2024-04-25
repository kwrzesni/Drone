#ifndef TEXT_UNSIGNED_INT_MENU_ITEM
#define TEXT_UNSIGNED_INT_MENU_ITEM

#include "IMenuItem.h"

class TextUnsignedIntMenuItem : public IMenuItem
{
public:
  TextUnsignedIntMenuItem(IMenu& menu, IAction* action, const String& text, unsigned int& value, unsigned int minValue, unsigned int maxValue);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
  unsigned int& value;
  unsigned int minValue;
  unsigned int maxValue;
};

#endif
