#include "TextUnsignedIntMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextUnsignedIntMenuItem::TextUnsignedIntMenuItem(IMenu& menu, IAction* action, const String& text, unsigned int& value, unsigned int minValue, unsigned int maxValue)
  : IMenuItem(menu, action), text{text}, value{value < minValue ? minValue : (value > maxValue ? maxValue : value)}, minValue{minValue}, maxValue{maxValue}
{}

void TextUnsignedIntMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, getText());
}

void TextUnsignedIntMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (menuManager.currentMenu == &menu)
  {
  }
}

String TextUnsignedIntMenuItem::getValue() const
{
  return String(value);
}

void TextUnsignedIntMenuItem::setValue(String value)
{
  unsigned int temp = value.toInt();
  this->value = temp < minValue ? minValue : (temp > maxValue ? maxValue : temp);
}

String TextUnsignedIntMenuItem::getText() const
{
  return text + String(value);
}