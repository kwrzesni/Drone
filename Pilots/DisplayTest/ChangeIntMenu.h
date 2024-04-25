#ifndef CHANGE_INT_MENU
#define CHANGE_INT_MENU

#include "IMenu.h"
#include "ButtonMenuItem.h"
#include "ValueMenuItem.h"

enum class MenuControlCommand;

class ChangeIntMenu : public IMenu
{
public:
  ChangeIntMenu(MenuManager& menuManager);
  void draw();
  void update(MenuControlCommand menuControlCommand) override;
  void resetSelection() override;
  String getValue() const override;
  String getValue(int ind) const override;
  void setValue(const String& value) override;
  void setSelectedItemValue(const String& value) override;
  void updateValue(const String& value) override;
  void showValueError() override;
private:
  const String title = "Value";
  ValueMenuItem valueItem;
  const int nRows = 5;
  const int nCols = 3;
  const int nButtons = 13;
  ButtonMenuItem buttons[13];
  int selectedButtonInd = 12;
};

#endif
