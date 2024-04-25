#ifndef TEXT_FLOAT_MENU_ITEM
#define TEXT_FLOAT_MENU_ITEM

#include "IMenuItem.h"

class TextFloatMenuItem : public IMenuItem
{
public:
  TextFloatMenuItem(IMenu& menu, IAction* action, const String& text, float& value, float minValue, float maxValue);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
  float& value;
  float minValue;
  float maxValue;
};

#endif
