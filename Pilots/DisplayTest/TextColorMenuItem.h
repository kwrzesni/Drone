#ifndef TEXT_COLOR_MENU_ITEM
#define TEXT_COLOR_MENU_ITEM

#include "IMenuItem.h"
#include "Colors.h"

class TextColorMenuItem : public IMenuItem
{
public:
  TextColorMenuItem(IMenu& menu, IAction* action, const String& text, Color& color);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
  Color& color;
};

#endif
