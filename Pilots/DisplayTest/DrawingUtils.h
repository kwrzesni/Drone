#ifndef DRAWING_UTILS
#define DRAWING_UTILS

#include <ILI9488.h>
#include "MenuManager.h"

int calculateWidth(const MenuManager& menuManager, const String& text, int maxWidth);
int calculateHeight(const MenuManager& menuManager, const String& text, int width, int maxHeight);
int calculateTextHeight(const MenuManager& menuManager, const String& text, int width);
void clearScreen(MenuManager& menuManager);
void clearPage(MenuManager& menuManager);
void drawHeader(MenuManager& menuManager, const String& text);
void drawText(MenuManager& menuManager, int x, int y, int width, int height, bool isSelected, const String& text);
void drawButton(MenuManager &menuManager, int x, int y, int width, int height, const String& text, bool isSelected);
void drawValue(MenuManager &menuManager, int x, int y, int width, int height, const String& value, bool isSelected);
void updateDrawnValue(MenuManager &menuManager, int x, int y, int width, int height, const String& value, int previousTextLength);
bool isInteger(const String& value);
bool isFloat(const String& value);

#endif
