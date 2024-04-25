#ifndef TEXT_MENU_ITEM
#define TEXT_MENU_ITEM

#include "IMenuItem.h"

class TextMenuItem : public IMenuItem
{
public:
  TextMenuItem(IMenu& menu, IAction* action, const String& text);
  void draw(MenuManager& menuManager, bool isSelected) override;
  void update(MenuManager& menuManager, const String& value) override;
  String getValue() const override;
  void setValue(String value) override;
  String getText() const override;
private:
  String text;
};

#endif
