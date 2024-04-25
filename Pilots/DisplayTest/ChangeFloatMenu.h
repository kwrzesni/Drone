#ifndef CHANGE_FLOAT_MENU
#define CHANGE_FLOAT_MENU

#include "IMenu.h"
#include "ButtonMenuItem.h"
#include "ValueMenuItem.h"

enum class MenuControlCommand;

class ChangeFloatMenu : public IMenu
{
public:
  ChangeFloatMenu(MenuManager& menuManager);
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
  const int nButtons = 14;
  ButtonMenuItem buttons[14];
  int selectedButtonInd = 13;
};

#endif
