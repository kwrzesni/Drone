#include "UpdateValueAction.h"
#include "IMenu.h"
#include "IMenuItem.h"
#include "MenuManager.h"

UpdateValueAction::UpdateValueAction(MenuManager& menuManager, int ind)
  : menuManager(menuManager), ind(ind)
{}

void UpdateValueAction::run() const
{
  menuManager.currentMenu->updateValue(menuManager.currentMenu->getValue(ind));
}