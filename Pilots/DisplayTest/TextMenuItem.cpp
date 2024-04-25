#include "TextMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextMenuItem::TextMenuItem(IMenu& menu, IAction* action, const String& text)
  : IMenuItem(menu, action), text(text)
{}

void TextMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, text);
}

void TextMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (menuManager.currentMenu == &menu)
  {
    ;
  }
  text = value;
}

String TextMenuItem::getValue() const
{
  return text;
}

void TextMenuItem::setValue(String value)
{
  text = value;
}

String TextMenuItem::getText() const
{
  return text;
}