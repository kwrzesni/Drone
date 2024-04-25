#include "TextBoolMenuItem.h"
#include "MenuManager.h"
#include "DrawingUtils.h"

TextBoolMenuItem::TextBoolMenuItem(IMenu& menu, IAction* action, const String& text, bool& value)
  : IMenuItem(menu, action), text{text}, value{value}
{}

void TextBoolMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawText(menuManager, x, y, width, height, isSelected, getText());
}

void TextBoolMenuItem::update(MenuManager& menuManager, const String& value)
{
  if (value == "true")
  {
    this->value = true;
  }
  else if (value == "false")
  {
    this->value = false;
  }
  else
  {
    this->value = !this->value;
  }
  ++menuManager.menuFormatSettings.menuVersion;
  menuManager.currentMenu->draw();
}

String TextBoolMenuItem::getValue() const
{
  return String(value);
}

void TextBoolMenuItem::setValue(String value)
{
  if (value == "true")
  {
    this->value = true;
  }
  else if (value == "false")
  {
    this->value = false;
  }
  else
  {
    this->value = !this->value;
  }
}

String TextBoolMenuItem::getText() const
{
  return text + (value ? "true" : "false");
}