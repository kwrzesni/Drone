#include "ChangeFloatAction.h"
#include "IMenu.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

ChangeFloatAction::ChangeFloatAction(MenuManager& menuManager)
  : menuManager(menuManager)
{}

void ChangeFloatAction::run() const
{
  String value = menuManager.currentMenu->getValue();
  if (isFloat(value))
  {
    menuManager.currentMenu->getParent()->setSelectedItemValue(value);
    ++menuManager.menuFormatSettings.menuVersion;
    menuManager.changeMenu(*menuManager.currentMenu->getParent(), false);
  }
  menuManager.currentMenu->showValueError();
}