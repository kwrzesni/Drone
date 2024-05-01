#include "DrawingUtils.h"
#include "Utils.h"

void clearScreen(MenuManager& menuManager)
{
  menuManager.tft.fillScreen((uint16_t)menuManager.menuFormatSettings.backgroundColor);
}

void clearPage(MenuManager& menuManager)
{
  clearScreenFromHeight(menuManager, menuManager.menuFormatSettings.headerHeight);
}

void clearScreenFromHeight(MenuManager& menuManager, int height)
{
  menuManager.tft.fillRect(0, height, menuManager.displaySettings.width, menuManager.displaySettings.height - height, (uint16_t)menuManager.menuFormatSettings.backgroundColor);
}

void drawBorder(MenuManager& menuManager, int x, int y, int width, int height, int borderSize, uint16_t color)
{
  for (int i = 0; i < borderSize; ++i)
  {
    if (width <= 0 || height <= 0)
    {
      return;
    }
    menuManager.tft.drawRect(x + i, y + i, width, height, color);
    width -= 2;
    height -= 2;
  }
}

void drawHeaderBorder(MenuManager& menuManager)
{
  drawBorder(menuManager, 0, 0, menuManager.displaySettings.width, menuManager.menuFormatSettings.headerHeight, menuManager.menuFormatSettings.headerBorderSize, (uint16_t)menuManager.menuFormatSettings.headerBorderColor);
}

void drawItemBorder(MenuManager& menuManager, int x, int y, int width, int height, bool isSelected)
{
  drawBorder(menuManager, x, y, width, height, menuManager.menuFormatSettings.itemBorderSize, (uint16_t)(isSelected ? menuManager.menuFormatSettings.itemBorderSelectionColor : menuManager.menuFormatSettings.itemBorderColor));
}

void drawText(MenuManager& menuManager, int x, int y, int maxWidth, int maxHeight, const String& text, int textSizeX, int textSizeY, int linePadding, uint16_t color, bool multiline, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment, bool allowTrimmedCharacters)
{
  if (text.length() == 0)
  {
    return;
  }
  const int charWidth = menuManager.displaySettings.xTextScalar * textSizeX;
  const int charHeight = menuManager.displaySettings.yTextScalar * textSizeY;
  const int nCharsInLine = maxWidth / charWidth;
  if (nCharsInLine == 0)
  {
    return;
  }
  const int nFittingLines = allowTrimmedCharacters ? ((maxHeight + linePadding - 1) / (charHeight + linePadding) + 1) : ((maxHeight + linePadding) / (charHeight + linePadding));
  if (nFittingLines == 0)
  {
    return;
  }
  const int nNeededLines = ((int)text.length() - 1 ) / nCharsInLine + 1;
  const int maxNumberOfLineToDraw = multiline ? nFittingLines : 1;
  const int nLines = nNeededLines <= maxNumberOfLineToDraw ? nNeededLines : maxNumberOfLineToDraw;
  const int textWidth = charWidth * nCharsInLine;
  const int textHeight = nLines * (charHeight + linePadding) - linePadding;

  Serial.print("text: "); Serial.println(text);
  Serial.print("charWidth: "); Serial.println(charWidth);
  Serial.print("charHeight: "); Serial.println(charHeight);
  Serial.print("nCharsInLine: "); Serial.println(nCharsInLine);
  Serial.print("nFittingLines: "); Serial.println(nFittingLines);
  Serial.print("nNeededLines: "); Serial.println(nNeededLines);
  Serial.print("maxNumberOfLineToDraw: "); Serial.println(maxNumberOfLineToDraw);
  Serial.print("nLines: "); Serial.println(nLines);
  Serial.print("textWidth: "); Serial.println(textWidth);
  Serial.print("textHeight: "); Serial.println(textHeight);

  switch (verticalAlignment)
  {
    case VerticalAlignment::up:
      break;
    case VerticalAlignment::center:
      y = y + (maxHeight - textHeight) / 2;
      break;
    case VerticalAlignment::down:
      y = y + maxHeight - textHeight;
      break;
  }

  menuManager.tft.setTextSize(textSizeX, textSizeY);
  menuManager.tft.setTextColor(color);

  const int dy = charHeight + linePadding;
  int ind = 0;
  for (int line = 0; line < nLines - 1; ++line)
  {
    menuManager.tft.setCursor(x, y);
    menuManager.tft.print(text.substring(ind, ind + nCharsInLine));
    y += dy;
    ind += nCharsInLine;
  }

  const int lastLineWidth = (text.length() - ind) * charWidth;
  switch (horizontalAlignment)
  {
    case HorizontalAlignment::left:
      break;
    case HorizontalAlignment::center:
      x = x + (maxWidth - lastLineWidth) / 2;
      break;
    case HorizontalAlignment::right:
      x = x + maxWidth - lastLineWidth;
      break;
  }
  menuManager.tft.setCursor(x, y);
  menuManager.tft.print(text.substring(ind));
}

void drawHeaderText(MenuManager& menuManager, const String& text)
{
  drawText(menuManager, menuManager.menuFormatSettings.headerBorderSize, menuManager.menuFormatSettings.headerBorderSize, menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.headerBorderSize, menuManager.menuFormatSettings.headerHeight - 2 * menuManager.menuFormatSettings.headerBorderSize, text, menuManager.menuFormatSettings.headerTextSizeX, menuManager.menuFormatSettings.headerTextSizeY, menuManager.menuFormatSettings.headerLinePadding, (uint16_t)menuManager.menuFormatSettings.headerTextColor, menuManager.menuFormatSettings.multilineHeaderText, HorizontalAlignment::center, VerticalAlignment::center, false);
}

void drawItemText(MenuManager& menuManager, int x, int y, int maxWidth, int maxHeight, const String& text, bool isSelected, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment, bool allowTrimmedCharacters)
{
  drawText(menuManager, x, y, maxWidth, maxHeight, text, menuManager.menuFormatSettings.itemTextSizeX, menuManager.menuFormatSettings.itemTextSizeX, menuManager.menuFormatSettings.itemsLinePadding, (uint16_t)(isSelected ? menuManager.menuFormatSettings.itemTextSelectionColor : menuManager.menuFormatSettings.itemTextColor), menuManager.menuFormatSettings.multilineItemsText, horizontalAlignment, verticalAlignment, allowTrimmedCharacters);
}

void drawHeader(MenuManager& menuManager, const String& text)
{
  drawHeaderBorder(menuManager);
  menuManager.tft.fillRect(menuManager.menuFormatSettings.headerBorderSize, menuManager.menuFormatSettings.headerBorderSize, menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.headerBorderSize, menuManager.menuFormatSettings.headerHeight - 2 * menuManager.menuFormatSettings.headerBorderSize, (uint16_t)menuManager.menuFormatSettings.headerBackgroundColor);
  drawHeaderText(menuManager, text);
}