#ifndef MENU_MANAGER
#define MENU_MANAGER

#include <ILI9488.h>
#include "IMenu.h"
#include "ListMenu.h"
#include "Colors.h"
#include "ChangeUnsignedIntMenu.h"
#include "ChangeIntMenu.h"
#include "ChangeFloatMenu.h"

class Action;

enum class MenuControlCommand
{
  left,
  right,
  up,
  down,
  ok,
  home,
  menu,
  back
};

struct DisplaySettings
{
  int width = 480;
  int height = 320;
  int xTextScalar = 6;
  int yTextScalar = 8;
};

struct MenuFormatSettings
{
  Color backgroundColor = Color::yellow;
  Color headerBackgroundColor = Color::white;
  Color headerTextColor = Color::black; 
  Color headerBorderColor = Color::black;
  Color itemBackgroundColor = Color::purple;
  Color itemBorderSelectionColor = Color::red;
  Color itemTextColor = Color::green;
  Color itemTextSelectionColor = Color::red;
  Color itemBorderColor = Color::lightGrey;
  unsigned int headerHeight = 50;
  unsigned int headerBorderSize = 3;
  unsigned int headerTextSizeX = 4;
  unsigned int headerTextSizeY = 4;
  unsigned int itemBorderSize = 1;
  unsigned int itemTextSizeX = 2;
  unsigned int itemTextSizeY = 2;
  unsigned int headerPadding = 10;
  unsigned int itemsPaddingX = 5;
  unsigned int itemsPaddingY = 7;
  unsigned int headerLinePadding = 2;
  unsigned int itemsLinePadding = 2;
  bool multilineHeaderText = false;
  bool multilineItemsText = false;
  int menuVersion = 1;
};

bool isMenuLayoutParameter(const MenuManager& menuManager, const void* value);

class MenuManager
{
public:
  MenuManager(ILI9488 tft);
  void begin();
  void update(MenuControlCommand menuControlCommand);
  void changeMenu(IMenu& targetMenu, bool selectionResetRequired);
  void runAction(const Action& action);
  void runChangeMenuAction(IMenu& targetMenu);
  void runUpdateCurrentMenuValue();
  void runUpdateCurrentItemValue();
  void runUpdateParentMenuValue();
  void runUpdateParentItemValue();
public:
  const DisplaySettings displaySettings;
  MenuFormatSettings menuFormatSettings;
  ILI9488 tft;
  // menus
  ListMenu mainManu;
  ListMenu formatingMenu;
  ListMenu colorMenu;
  ChangeUnsignedIntMenu changeUnsignedIntMenu;
  ChangeIntMenu changeIntMenu;
  ChangeFloatMenu changeFloatMenu;
  IMenu* currentMenu = &mainManu;
  IMenu& homeMenu = mainManu;
};

#endif
