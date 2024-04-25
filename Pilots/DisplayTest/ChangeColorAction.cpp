#include "ChangeColorAction.h"
#include "IMenu.h"
#include "MenuManager.h"

ChangeColorAction::ChangeColorAction(MenuManager& menuManager)
  : menuManager(menuManager)
{}

void ChangeColorAction::run() const
{
  String colorString = menuManager.currentMenu->getValue();
  menuManager.currentMenu->getParent()->setSelectedItemValue(colorString);
  menuManager.currentMenu->getParent()->lowerMenuVersion();
  menuManager.changeMenu(*menuManager.currentMenu->getParent(), false);
}