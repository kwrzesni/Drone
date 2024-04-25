#include "TextIntMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextIntMenuItem::TextIntMenuItem(IMenu& menu, IAction* action, const String& text, int& value, int minValue, int maxValue)
  : IMenuItem(menu, action), text{text}, value{value < minValue ? minValue : (value > maxValue ? maxValue : value)}, minValue{minValue}, maxValue{maxValue}
{}

void TextIntMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, getText());
}

void TextIntMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (menuManager.currentMenu == &menu)
  {
  }
}

String TextIntMenuItem::getValue() const
{
  return String(value);
}

void TextIntMenuItem::setValue(String value)
{
  int temp = value.toInt();
  this->value = temp < minValue ? minValue : (temp > maxValue ? maxValue : temp);
}

String TextIntMenuItem::getText() const
{
  return text + String(value);
}