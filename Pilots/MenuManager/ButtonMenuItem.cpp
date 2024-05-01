#include "ButtonMenuItem.h"
#include "IMenu.h"
#include "DrawingUtils.h"

ButtonMenuItem::ButtonMenuItem(Action action, const String& text)
  : IMenuItem(action), text{text}
{}

void ButtonMenuItem::calculateSize(MenuManager& menuManager, int maxWidth)
{
  width = this->maxWidth = maxWidth;
  height = trimmedHeight = 2 * menuManager.menuFormatSettings.itemsPaddingY + menuManager.menuFormatSettings.itemBorderSize + menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
  trimmed = false;
}

void ButtonMenuItem::draw(MenuManager& menuManager, bool isSelected)
{
  drawItemBorder(menuManager, x, y, width, height, isSelected);
  drawItemText(menuManager, x + menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX, y + menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingY, width - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingX), trimmedHeight - 2 * (menuManager.menuFormatSettings.itemBorderSize + menuManager.menuFormatSettings.itemsPaddingY), text, isSelected, HorizontalAlignment::center, VerticalAlignment::center, false);
}

void ButtonMenuItem::redraw(MenuManager& menuManager, bool isSelected)
{
  draw(menuManager, isSelected);
}

String ButtonMenuItem::getValue() const
{
  return getText();
}

String ButtonMenuItem::getText() const
{
  return text;
}

bool ButtonMenuItem::setValue(MenuManager& menuManager, String value)
{
  text = value;
  return false;
}

bool ButtonMenuItem::updateValue(MenuManager& menuManager, const String& value)
{
  return setValue(menuManager, value);
}

bool ButtonMenuItem::isValidValue(const String& value)
{
  return true;
}