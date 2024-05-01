#include "Utils.h"
#include "MenuManager.h"

int calculateTextWidth(const MenuManager& menuManager, int textLength, int textSizeX, int maxWidth)
{
  int textWidth = textLength * menuManager.displaySettings.xTextScalar * textSizeX;
  if (textWidth < maxWidth)
  {
    return textWidth;
  }
  Serial.print("calculateTextWidth: "); Serial.println(maxWidth - maxWidth % (menuManager.displaySettings.xTextScalar * textSizeX));
  return maxWidth - maxWidth % (menuManager.displaySettings.xTextScalar * textSizeX);
}

int calculateHeaderTextWidth(const MenuManager& menuManager, int textLength)
{
  return calculateTextWidth(menuManager, textLength, menuManager.menuFormatSettings.itemTextSizeX, menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.headerBorderSize);
}

int calculateItemTextWidth(const MenuManager& menuManager, int textLength, int maxWidth)
{
  return calculateTextWidth(menuManager, textLength, menuManager.menuFormatSettings.headerTextSizeX, maxWidth);
}

int calculateTextHeight(const MenuManager& menuManager, int textLength, int textSizeX, int width, int textSizeY, bool isMultiLine, int linePadding)
{
  int nCharsInLine = width / (menuManager.displaySettings.xTextScalar * textSizeX);
  if (!nCharsInLine)
  {
    return 0;
  }
  int nLines = isMultiLine ? (textLength - 1) / nCharsInLine + 1 : 1;
  Serial.print("calculateTextHeight: "); Serial.println(nLines * (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY + linePadding) - linePadding);
  return nLines * (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY + linePadding) - linePadding;
}

int calculateHeaderTextHeight(const MenuManager& menuManager, int textLength, int width)
{
  return calculateTextHeight(menuManager, textLength, menuManager.menuFormatSettings.headerTextSizeX, width, menuManager.menuFormatSettings.headerTextSizeY, menuManager.menuFormatSettings.multilineHeaderText, menuManager.menuFormatSettings.headerLinePadding);
}

int calculateItemTextHeight(const MenuManager& menuManager, int textLength, int width)
{
  return calculateTextHeight(menuManager, textLength, menuManager.menuFormatSettings.itemTextSizeX, width, menuManager.menuFormatSettings.itemTextSizeY, menuManager.menuFormatSettings.multilineItemsText, menuManager.menuFormatSettings.itemsLinePadding);
}

int calculateTrimmedTextHeight(const MenuManager& menuManager, int textSizeY, int textHeight, int maxTextHeight, bool allowTrimmedCharacters)
{
  if (textHeight <= maxTextHeight)
  {
    return textHeight;
  }
  if (allowTrimmedCharacters)
  {
    return maxTextHeight;
  }
  return maxTextHeight - maxTextHeight % (menuManager.displaySettings.yTextScalar * textSizeY);
}

int calculateTrimmedHeaderTextHeight(const MenuManager& menuManager, int textHeight)
{
  calculateTrimmedTextHeight(menuManager, menuManager.menuFormatSettings.headerTextSizeY, textHeight, menuManager.menuFormatSettings.headerHeight - 2 * menuManager.menuFormatSettings.headerBorderSize, false);
}

int calculateTrimmedItemTextHeight(const MenuManager& menuManager, int textHeight, int maxTextHeight, bool allowTrimmedCharacters)
{
  calculateTrimmedTextHeight(menuManager, menuManager.menuFormatSettings.itemTextSizeY, textHeight, maxTextHeight, allowTrimmedCharacters);
}

bool isUnsignedInt(const String& value)
{
  if (!value.length())
  {
    return false;
  }
  for (int i = 0; i < value.length(); ++i)
  {
    if (!isDigit(value[i]))
    {
      return false;
    }
  }
  return false;
}

bool isInt(const String& value)
{
  if (!value.length())
  {
    return false;
  }
  if (value.length() == 1)
  {
    return isDigit(value[0]) ? true : false;
  }
  if (!isDigit(value[0]) && value[0] != '-')
  {
    return false;
  }
  for (int i = isDigit(value[0]) ? 0 : 1; i < value.length(); ++i)
  {
    if (!isDigit(value[i]))
    {
      return false;
    }
  }
}

bool isFloat(const String& value)
{
  if (!value.length())
  {
    return false;
  }
  if (value.length() == 1)
  {
    return isDigit(value[0]) ? true : false;
  }
  if (!isDigit(value[0]) && value[0] != '-')
  {
    return false;
  }
  int i = isDigit(value[0]) ? 0 : 1;
  if (!isDigit(value[i]))
  {
    return false;
  }
  ++i;
  while (i < value.length() && isDigit(value[i]))
  {
    ++i;
  }
  if (i == value.length())
  {
    return true;
  }
  if (value[i] != '.')
  {
    return false;
  }
  for (i = i + 1; i < value.length(); ++i)
  {
    if (!isDigit(value[i]))
    {
      return false;
    }
  }
}