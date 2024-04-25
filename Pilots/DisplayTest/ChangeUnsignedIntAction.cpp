#include "ChangeUnsignedIntAction.h"
#include "IMenu.h"
#include "MenuManager.h"

ChangeUnsignedIntAction::ChangeUnsignedIntAction(MenuManager& menuManager)
  : menuManager(menuManager)
{}

void ChangeUnsignedIntAction::run() const
{
  String value = menuManager.currentMenu->getValue();
  menuManager.currentMenu->getParent()->setSelectedItemValue(value);
  ++menuManager.menuFormatSettings.menuVersion;
  menuManager.changeMenu(*menuManager.currentMenu->getParent(), false);
}