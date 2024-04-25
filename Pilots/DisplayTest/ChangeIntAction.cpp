#include "ChangeIntAction.h"
#include "IMenu.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

ChangeIntAction::ChangeIntAction(MenuManager& menuManager)
  : menuManager(menuManager)
{}

void ChangeIntAction::run() const
{
  String value = menuManager.currentMenu->getValue();
  if (isInteger(value))
  {
    menuManager.currentMenu->getParent()->setSelectedItemValue(value);
    ++menuManager.menuFormatSettings.menuVersion;
    menuManager.changeMenu(*menuManager.currentMenu->getParent(), false);
  }
  menuManager.currentMenu->showValueError();
}