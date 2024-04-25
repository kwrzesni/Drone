#include "TextValueMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextValueMenuItem::TextValueMenuItem(IMenu& menu, IAction* action, const String& text, const String& value)
  : IMenuItem(menu, action), text{text}, value{value}
{}

void TextValueMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, text + value);
}

void TextValueMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (menuManager.currentMenu == &menu)
  {
    ;
  }
  this->value = value;
}

String TextValueMenuItem::getValue() const
{
  return value;
}

void TextValueMenuItem::setValue(String value)
{
  this->value = value;
}

String TextValueMenuItem::getText() const
{
  return text + value;
}