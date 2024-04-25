#include "IMenu.h"
#include "MenuManager.h"

IMenu::IMenu(MenuManager& menuManager)
  : menuManager(menuManager)
{}

void IMenu::setParent(IMenu* parent)
{
  this->parent = parent;
}

IMenu* IMenu::getParent() const
{
  return parent;
}

void IMenu::lowerMenuVersion()
{
  --menuVersion;
}

void IMenu::showValueError()
{
}