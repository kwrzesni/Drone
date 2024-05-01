#ifndef TEXT_COLOR_MENU_ITEM
#define TEXT_COLOR_MENU_ITEM

#include "IMenuItem.h"
#include "Colors.h"

class TextColorMenuItem : public IMenuItem
{
public:
  TextColorMenuItem(Action action, const String& text, Color& color);
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
  Color& color;
};

#endif
