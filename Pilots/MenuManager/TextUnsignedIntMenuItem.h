#ifndef TEXT_UNSIGNED_INT_MENU_ITEM
#define TEXT_UNSIGNED_INT_MENU_ITEM

#include "IMenuItem.h"

class TextUnsignedIntMenuItem : public IMenuItem
{
public:
  TextUnsignedIntMenuItem(Action action, const String& text, unsigned int& value, unsigned int minValue, unsigned int maxValue);
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
  unsigned int& value;
  unsigned int minValue;
  unsigned int maxValue;
};

#endif
