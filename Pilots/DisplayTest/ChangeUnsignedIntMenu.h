#ifndef CHANGE_UNSIGNED_INT_MENU
#define CHANGE_UNSIGNED_INT_MENU

#include "IMenu.h"
#include "ButtonMenuItem.h"
#include "ValueMenuItem.h"

enum class MenuControlCommand;

class ChangeUnsignedIntMenu : public IMenu
{
public:
  ChangeUnsignedIntMenu(MenuManager& menuManager);
  void draw();
  void update(MenuControlCommand menuControlCommand) override;
  void resetSelection() override;
  String getValue() const override;
  String getValue(int ind) const override;
  void setValue(const String& value) override;
  void setSelectedItemValue(const String& value) override;
  void updateValue(const String& value) override;
private:
  const String title = "Value";
  ValueMenuItem valueItem;
  const int nRows = 4;
  const int nCols = 3;
  const int nButtons = 12;
  ButtonMenuItem buttons[12];
  int selectedButtonInd = 11;
};

#endif
