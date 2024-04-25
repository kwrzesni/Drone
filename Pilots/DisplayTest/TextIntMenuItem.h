#ifndef TEXT_INT_MENU_ITEM
#define TEXT_INT_MENU_ITEM

#include "IMenuItem.h"

class TextIntMenuItem : public IMenuItem
{
public:
  TextIntMenuItem(IMenu& menu, IAction* action, const String& text, int& value, int minValue, int maxValue);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
  int& value;
  int minValue;
  int maxValue;
};

#endif
