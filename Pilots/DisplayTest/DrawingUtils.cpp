#include "DrawingUtils.h"

int calculateWidth(const MenuManager& menuManager, const String& text, int maxWidth)
{
  int textWidth = text.length() * menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX;
  if (textWidth < maxWidth)
  {
    return textWidth;
  }
  int dx = menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX;
  return maxWidth - maxWidth % dx;
}

int calculateHeight(const MenuManager& menuManager, const String& text, int width, int maxHeight)
{
  int nCharsInLine = width / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
  int nLines = menuManager.menuFormatSettings.isMultiLine ? (text.length() / nCharsInLine + (text.length() % nCharsInLine != 0)) : 1;
  int textHeight = nLines * (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY + menuManager.menuFormatSettings.linePadding) - menuManager.menuFormatSettings.linePadding;
  if (textHeight <= maxHeight)
  {
    return textHeight;
  }
  int dy = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY + menuManager.menuFormatSettings.linePadding;
  maxHeight += menuManager.menuFormatSettings.linePadding;
  nLines = maxHeight / dy;
  return nLines * (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY + menuManager.menuFormatSettings.linePadding) - menuManager.menuFormatSettings.linePadding;
}

int calculateTextHeight(const MenuManager& menuManager, const String& text, int width)
{
  if (!menuManager.menuFormatSettings.isMultiLine)
  {
    if (text.length() && width >= menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX)
    {
      return menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
    }
    return 0;
  }
  int nCharsInLine = width / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
  int nLines = (text.length() - 1) / nCharsInLine + 1;
  return nLines * (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY + menuManager.menuFormatSettings.linePadding) - menuManager.menuFormatSettings.linePadding;
}

void clearScreen(MenuManager& menuManager)
{
  menuManager.tft.fillScreen((uint16_t)menuManager.menuFormatSettings.backgroundColor);
}

void clearPage(MenuManager& menuManager)
{
  menuManager.tft.fillRect(0, menuManager.menuFormatSettings.headerHeight, menuManager.displaySettings.width, menuManager.displaySettings.height - menuManager.menuFormatSettings.headerHeight, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
}

void drawHeader(MenuManager& menuManager, const String& text)
{
  for (int i = 0; i < menuManager.menuFormatSettings.headerBorderSize; ++i)
  {
    menuManager.tft.drawRect(i, i, menuManager.displaySettings.width - 2 * i, menuManager.menuFormatSettings.headerHeight - 2 * i, (uint16_t)menuManager.menuFormatSettings.headerBorderColor);
  }
  menuManager.tft.fillRect(menuManager.menuFormatSettings.headerBorderSize, menuManager.menuFormatSettings.headerBorderSize, menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.headerBorderSize, menuManager.menuFormatSettings.headerHeight - 2 * menuManager.menuFormatSettings.headerBorderSize, (uint16_t)menuManager.menuFormatSettings.headerBackgroundColor);
  int textWidth = text.length() * menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.headerTextSizeX;
  int textHeight = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.headerTextSizeY;
  menuManager.tft.setTextSize(menuManager.menuFormatSettings.headerTextSizeX, menuManager.menuFormatSettings.headerTextSizeY);
  menuManager.tft.setTextColor((uint16_t)menuManager.menuFormatSettings.headerTextColor);
  menuManager.tft.setCursor((menuManager.displaySettings.width - textWidth) / 2, (menuManager.menuFormatSettings.headerHeight - textHeight) / 2);
  menuManager.tft.print(text);
}

void drawText(MenuManager& menuManager, int x, int y, int width, int height, bool isSelected, const String& text)
{
  int startY = y;
  menuManager.tft.setTextSize(menuManager.menuFormatSettings.itemTextSizeX, menuManager.menuFormatSettings.itemTextSizeY);
  menuManager.tft.setTextColor(isSelected ? (uint16_t)menuManager.menuFormatSettings.itemTextSelectionColor : (uint16_t)menuManager.menuFormatSettings.itemTextColor);
  if (menuManager.menuFormatSettings.isMultiLine)
  {
    int textDy = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
    int nCharsInLine = width / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
    if (nCharsInLine == 0)
    {
      return;
    }
    int maxLines = (height + menuManager.menuFormatSettings.linePadding - 1) / (textDy + menuManager.menuFormatSettings.linePadding) + 1;
    int nLines = (text.length() - 1) / nCharsInLine + 1;
    nLines = nLines <= maxLines ? nLines : maxLines;
    int dy = textDy + menuManager.menuFormatSettings.linePadding;
    for (int line = 0, ind = 0; line < nLines; ++line)
    {
      int lineEndInd = ind + nCharsInLine < text.length() ? ind + nCharsInLine : text.length();
      menuManager.tft.setCursor(x, y);
      menuManager.tft.print(text.substring(ind, lineEndInd));
      ind = lineEndInd;
      y += dy;
    }
  }
  else
  {
    menuManager.tft.setCursor(x, y);
    int maxTextLength = width / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
    if (maxTextLength < text.length())
    {
      menuManager.tft.print(text.substring(0, maxTextLength));
    }
    else
    {
      menuManager.tft.print(text);
    }
  }
  menuManager.tft.drawRect(x, startY, width, height, (uint16_t)menuManager.menuFormatSettings.itemBorderColor);
}

void drawButton(MenuManager &menuManager, int x, int y, int width, int height, const String& text, bool isSelected)
{
  for (int i = 0; i < menuManager.menuFormatSettings.itemBorderSize; ++i)
  {
    menuManager.tft.drawRect(x + i, y + i, width - 2 * i, height - 2 * i, (uint16_t)(isSelected ? menuManager.menuFormatSettings.itemBorderSelectionColor : menuManager.menuFormatSettings.itemBorderColor));
  }
  x = x + menuManager.menuFormatSettings.itemBorderSize;
  y = y + menuManager.menuFormatSettings.itemBorderSize;
  width = width - 2 * menuManager.menuFormatSettings.itemBorderSize;
  height = height - 2 * menuManager.menuFormatSettings.itemBorderSize;
  menuManager.tft.fillRect(x, y, width, height, (uint16_t)menuManager.menuFormatSettings.itemBackgroundColor);

  menuManager.tft.setTextSize(menuManager.menuFormatSettings.itemTextSizeX, menuManager.menuFormatSettings.itemTextSizeY);
  menuManager.tft.setTextColor(isSelected ? (uint16_t)menuManager.menuFormatSettings.itemTextSelectionColor : (uint16_t)menuManager.menuFormatSettings.itemTextColor);
  if (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY > height)
  {
    return;
  }
  int nCharsInLine = width / (menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX);
  if (nCharsInLine == 0)
  {
    return;
  }
  if (menuManager.menuFormatSettings.isMultiLine)
  {
    int nCharsToPrint = text.length() < nCharsInLine ? text.length() : nCharsInLine;
    int textDy = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
    int maxLines = (height + menuManager.menuFormatSettings.linePadding - 1) / (textDy + menuManager.menuFormatSettings.linePadding) + 1;
    int nLines = (text.length() - 1) / nCharsInLine + 1;
    nLines = nLines <= maxLines ? nLines : maxLines;
    int dy = textDy + menuManager.menuFormatSettings.linePadding;
    int textWidth = nCharsToPrint * menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX;
    int textHeight = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
    x = x + (width - textWidth) / 2;
    y = y + (height - textHeight) / 2;
    for (int line = 0, ind = 0; line < nLines; ++line)
    {
      int lineEndInd = ind + nCharsInLine < text.length() ? ind + nCharsInLine : text.length();
      menuManager.tft.setCursor(x, y);
      menuManager.tft.print(text.substring(ind, lineEndInd));
      ind = lineEndInd;
      y += dy;
    }
  }
  else
  {
    int nCharsToPrint = text.length() < nCharsInLine ? text.length() : nCharsInLine;
    int textWidth = nCharsToPrint * menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX;
    int textHeight = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;

    menuManager.tft.setCursor(x + (width - textWidth) / 2, y + (height - textHeight) / 2);
    menuManager.tft.print(text.substring(0, nCharsToPrint));
  }
}

void drawValue(MenuManager &menuManager, int x, int y, int width, int height, const String& value, bool isSelected)
{
  for (int i = 0; i < menuManager.menuFormatSettings.itemBorderSize; ++i)
  {
    menuManager.tft.drawRect(x + i, y + i, width - 2 * i, height - 2 * i, (uint16_t)(isSelected ? menuManager.menuFormatSettings.itemBorderSelectionColor : menuManager.menuFormatSettings.itemBorderColor));
  }
  x = x + menuManager.menuFormatSettings.itemBorderSize;
  y = y + menuManager.menuFormatSettings.itemBorderSize;
  width = width - 2 * menuManager.menuFormatSettings.itemBorderSize;
  height = height - 2 * menuManager.menuFormatSettings.itemBorderSize;
  menuManager.tft.fillRect(x, y, width, height, (uint16_t)menuManager.menuFormatSettings.itemBackgroundColor);

  menuManager.tft.setTextSize(menuManager.menuFormatSettings.itemTextSizeX, menuManager.menuFormatSettings.itemTextSizeY);
  menuManager.tft.setTextColor(isSelected ? (uint16_t)menuManager.menuFormatSettings.itemTextSelectionColor : (uint16_t)menuManager.menuFormatSettings.itemTextColor);
  if (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY > height)
  {
    return;
  }
  int textHeight = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
  menuManager.tft.setCursor(x + menuManager.menuFormatSettings.itemsPaddingX, y + (height - textHeight) / 2);
  menuManager.tft.print(value);
}

void updateDrawnValue(MenuManager &menuManager, int x, int y, int width, int height, const String& value, int previousTextLength)
{
  x = x + menuManager.menuFormatSettings.itemBorderSize;
  y = y + menuManager.menuFormatSettings.itemBorderSize;
  width = width - 2 * menuManager.menuFormatSettings.itemBorderSize;
  height = height - 2 * menuManager.menuFormatSettings.itemBorderSize;
  if (menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY > height)
  {
    return;
  }

  int textHeight = menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY;
  x = x + menuManager.menuFormatSettings.itemsPaddingX;
  y = y + (height - textHeight) / 2;
  if (value.length() < previousTextLength)
  {
    menuManager.tft.fillRect(x + value.length() * menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX, y, menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX, menuManager.displaySettings.yTextScalar * menuManager.menuFormatSettings.itemTextSizeY, (uint16_t)menuManager.menuFormatSettings.itemBackgroundColor);
  }
  else
  {
     menuManager.tft.setTextSize(menuManager.menuFormatSettings.itemTextSizeX, menuManager.menuFormatSettings.itemTextSizeY);
    menuManager.tft.setTextColor((uint16_t)menuManager.menuFormatSettings.itemTextColor);
    menuManager.tft.setCursor(x + previousTextLength * menuManager.displaySettings.xTextScalar * menuManager.menuFormatSettings.itemTextSizeX, y);
    menuManager.tft.print(value.substring(previousTextLength, previousTextLength + 1));
  }
}

bool isInteger(const String& value)
{
  if (!value.length())
  {
    return false;
  }
  if (value.length() == 1 && value[0] == '-')
  {
    return false;
  }
  for (int i = 1; i < value.length(); ++i)
  {
    if (value[i] == '-')
    {
      return false;
    }
  }
  return true;
}

bool isFloat(const String& value)
{
  if (!value.length())
  {
    return false;
  }
  if (value[value.length() - 1] == '-' || value[value.length() - 1] == '.' || value[0] == '.')
  {
    return false;
  }
  if (value[0] == '-' && value[1] == '.')
  {
    return false;
  }
  int nDots = 0;
  for (int i = 1; i < value.length() - 1; ++i)
  {
    if (value[i] == '-')
    {
      return false;
    }
    if (value[i] == '.')
    {
      ++nDots;
    }
  }
  if (nDots > 1)
  {
    return false;
  }
  return true;
}