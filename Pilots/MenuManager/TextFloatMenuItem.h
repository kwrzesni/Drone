#ifndef TEXT_FLOAT_MENU_ITEM
#define TEXT_FLOAT_MENU_ITEM

#include "IMenuItem.h"

class TextFloatMenuItem : public IMenuItem
{
public:
  TextFloatMenuItem(Action action, const String& text, float& value, float minValue, float maxValue);
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
  float& value;
  float minValue;
  float maxValue;
};

#endif
