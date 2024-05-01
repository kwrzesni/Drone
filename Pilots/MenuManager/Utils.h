#ifndef UTILS
#define UTILS

class MenuManager;
class String;

int calculateTextWidth(const MenuManager& menuManager, int textLength, int textSizeX, int maxWidth);
int calculateHeaderTextWidth(const MenuManager& menuManager, int textLength);
int calculateItemTextWidth(const MenuManager& menuManager, int textLength, int maxWidth);
int calculateTextHeight(const MenuManager& menuManager, int textLength, int textSizeX, int width, int textSizeY, bool isMultiLine, int linePadding);
int calculateHeaderTextHeight(const MenuManager& menuManager, int textLength, int width);
int calculateItemTextHeight(const MenuManager& menuManager, int textLength, int width);
int calculateTrimmedTextHeight(const MenuManager& menuManager, int textSizeY, int textHeight, int maxTextHeight, bool allowTrimmedCharacters = false);
int calculateTrimmedHeaderTextHeight(const MenuManager& menuManager, int textHeight);
int calculateTrimmedItemTextHeight(const MenuManager& menuManager, int textHeight, int maxTextHeight, bool allowTrimmedCharacters = false);
bool isUnsignedInt(const String& value);
bool isInt(const String& value);
bool isFloat(const String& value);

#endif
