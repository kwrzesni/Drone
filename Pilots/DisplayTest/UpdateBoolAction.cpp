#include "UpdateBoolAction.h"
#include "IMenu.h"
#include "IMenuItem.h"
#include "MenuManager.h"

UpdateBoolAction::UpdateBoolAction(MenuManager& menuManager)
  : menuManager(menuManager)
{}

void UpdateBoolAction::run() const
{
  menuManager.currentMenu->updateValue("");
}