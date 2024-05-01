#include "IMenuItem.h"
#include "IMenu.h"

IMenuItem::IMenuItem(Action action)
  : action(action)
{
}

const Action& IMenuItem::getAction() const
{
  return action;
}

int IMenuItem::getX() const
{
  return x;
}


int IMenuItem::getY() const
{
  return y;
}


int IMenuItem::getWidth() const
{
  return width;
}

int IMenuItem::getHeight() const
{
  return trimmedHeight;
}

void IMenuItem::setPosition(int x, int y)
{
  this->x = x;
  this->y = y;
}

void IMenuItem::setSize(int width, int height)
{
  this->width = this->maxWidth = width;
  this->height = this->trimmedHeight = height;
  trimmed = false;
}

void IMenuItem::trim(MenuManager& menuManager, int maxHeight)
{
  trimmedHeight = maxHeight;
  trimmed = true;
}