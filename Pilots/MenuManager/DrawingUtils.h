#ifndef DRAWING_UTILS
#define DRAWING_UTILS

#include <ILI9488.h>
#include "MenuManager.h"

enum class HorizontalAlignment
{
  left,
  center,
  right
};

enum class VerticalAlignment
{
  up,
  center,
  down
};

void clearScreen(MenuManager& menuManager);
void clearPage(MenuManager& menuManager);
void clearScreenFromHeight(MenuManager& menuManager, int height);
void drawBorder(MenuManager& menuManager, int x, int y, int width, int height, int borderSize, uint16_t color);
void drawHeaderBorder(MenuManager& menuManager);
void drawItemBorder(MenuManager& menuManager, int x, int y, int width, int height, bool isSelected);
void drawText(MenuManager& menuManager, int x, int y, int maxWidth, int maxHeight, const String& text, int textSizeX, int textSizeY, int linePadding, uint16_t color, bool multiline, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment, bool allowTrimmedCharacters);
void drawHeaderText(MenuManager& menuManager, const String& text);
void drawItemText(MenuManager& menuManager, int x, int y, int maxWidth, int maxHeight, const String& text, bool isSelected, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment, bool allowTrimmedCharacters);
void drawHeader(MenuManager& menuManager, const String& text);

#endif
