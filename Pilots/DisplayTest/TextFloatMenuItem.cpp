#include "TextFloatMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextFloatMenuItem::TextFloatMenuItem(IMenu& menu, IAction* action, const String& text, float& value, float minValue, float maxValue)
  : IMenuItem(menu, action), text{text}, value{value < minValue ? minValue : (value > maxValue ? maxValue : value)}, minValue{minValue}, maxValue{maxValue}
{}

void TextFloatMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, getText());
}

void TextFloatMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (menuManager.currentMenu == &menu)
  {
  }
}

String TextFloatMenuItem::getValue() const
{
  return String(value);
}

void TextFloatMenuItem::setValue(String value)
{
  float temp = value.toFloat();
  this->value = temp < minValue ? minValue : (temp > maxValue ? maxValue : temp);
}

String TextFloatMenuItem::getText() const
{
  return text + String(value);
}