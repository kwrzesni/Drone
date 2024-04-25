#include "IMenuItem.h"
#include "IMenu.h"

IMenuItem::IMenuItem(IMenu& menu, IAction* action)
  : menu(menu), action(action)
{
}

void IMenuItem::runAction()
{
  if (action)
  {
    action->run();
  }
}

void IMenuItem::setPosition(int x, int y, int width, int height)
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

IMenuItem::~IMenuItem()
{
  delete action;
}