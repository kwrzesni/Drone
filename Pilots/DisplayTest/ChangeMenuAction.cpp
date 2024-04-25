#include "ChangeMenuAction.h"
#include "IMenu.h"
#include "MenuManager.h"

ChangeMenuAction::ChangeMenuAction(IMenu& targetMenu, MenuManager& menuManager, int ind)
  : targetMenu(targetMenu), menuManager(menuManager), ind{ind}
{}

void ChangeMenuAction::run() const
{
  targetMenu.setParent(menuManager.currentMenu);
  if (ind >= 0)
  {
    targetMenu.setValue(menuManager.currentMenu->getValue(ind));
    menuManager.changeMenu(targetMenu, false);
  }
  else
  {
    menuManager.changeMenu(targetMenu, true);
  }
  
}