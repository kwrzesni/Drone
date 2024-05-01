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
  const int nRows = 4;
  const int nCols = 3;
  const int nButtons = 12;
  ButtonMenuItem buttons[12];
  int selectedButtonInd = 11;
};

#endif
