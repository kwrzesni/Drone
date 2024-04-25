#include "TextColorMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextColorMenuItem::TextColorMenuItem(IMenu& menu, IAction* action, const String& text, Color& color)
  : IMenuItem(menu, action), text{text}, color{color}
{}

void TextColorMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, getText());
}

void TextColorMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (menuManager.currentMenu == &menu)
  {
  }
}

String TextColorMenuItem::getValue() const
{
  return colorToString(color);
}

void TextColorMenuItem::setValue(String value)
{
  color = stringToColor(value);
}

String TextColorMenuItem::getText() const
{
  return text + colorToString(color);
}