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
  const Action& getAction() const override;
  void onLeft() override;
  void onRigth() override;
  void onUp() override;
  void onDown() override;
  void draw();
  void resetSelection() override;
  String getValue() const override;
  String getItemValue() const override;
  bool setValue(const String& value) override;
  bool setItemValue(const String& value) override;
  bool updateValue(const String& value) override;
  bool updateItemValue(const String& value) override;
  bool isValidValue(const String& value) override;
  bool isValidItemValue(const String& value) override;
  void showValueError() override;
private:
  void updateLayout();
private:
  const String title = "Value";
  ValueMenuItem valueItem;
  const int nRows = 5;
  const int nCols = 3;
  const int nButtons = 14;
  ButtonMenuItem buttons[14];
  int selectedButtonInd = nButtons - 1;
};

#endif
