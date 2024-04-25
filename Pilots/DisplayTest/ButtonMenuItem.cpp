#include "ButtonMenuItem.h"
#include "IMenu.h"
#include "DrawingUtils.h"

ButtonMenuItem::ButtonMenuItem(IMenu& menu, IAction* action, const String& text)
  : IMenuItem(menu, action), text{text}
{}

void ButtonMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawButton(menuManager, x, y, width, height, text, isSelected);
}

void ButtonMenuItem::update(MenuManager& menuManager, const String& value)
{

}

String ButtonMenuItem::getValue() const
{
  return text;
}

void ButtonMenuItem::setValue(String value)
{
  text = value;
}

String ButtonMenuItem::getText() const
{
  return text;
}