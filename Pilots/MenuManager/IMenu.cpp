#include "IMenu.h"
#include "MenuManager.h"

IMenu::IMenu(MenuManager& menuManager)
  : menuManager(menuManager)
{
}

void IMenu::setParent(IMenu* parent)
{
  this->parent = parent;
}

IMenu* IMenu::getParent() const
{
  return parent;
}

int IMenu::getMenuVersion() const
{
  return menuVersion;
}

void IMenu::onLeft()
{
}

void IMenu::onRigth()
{
}

void IMenu::onUp()
{
}

void IMenu::onDown()
{
}

void IMenu::showValueError()
{
}

void IMenu::updateLayout()
{
}

IMenu::~IMenu()
{
}