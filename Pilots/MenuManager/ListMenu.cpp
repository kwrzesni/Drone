#include "MenuManager.h"
#include "DrawingUtils.h"
#include "Utils.h"

ListMenu::ListMenu(MenuManager& menuManager, String title)
  : IMenu(menuManager), title{title}
{
}

void ListMenu::assignMenuItems(std::initializer_list<IMenuItem*> menuItems)
{
  clear();
  nMenuItems = menuItems.size();
  if (nMenuItems == 0)
  {
    return;
  }
  menuItemList = new IMenuItem*[nMenuItems];
  int i = 0;
  for (IMenuItem* menuItem : menuItems)
  {
    menuItemList[i] = menuItem;
    ++i;
  }
}

void ListMenu::clear()
{
  if (menuItemList != nullptr)
  {
    for (int i = 0; i < nMenuItems; ++i)
    {
      delete menuItemList[i];
    }
    delete[] menuItemList;
    menuItemList = nullptr;
    nMenuItems = selectedMenuItemInd = 0;
  }
}

const Action& ListMenu::getAction() const
{
  return menuItemList[selectedMenuItemInd]->getAction();
}

void ListMenu::onUp()
{
  if (selectedMenuItemInd > 0)
  {
    if (selectedMenuItemInd == pageStartInds[pageInd])
    {
      --selectedMenuItemInd;
      --pageInd;
      drawPage(true);
    }
    else
    {
      menuItemList[selectedMenuItemInd]->draw(menuManager, false);
      --selectedMenuItemInd;
      menuItemList[selectedMenuItemInd]->draw(menuManager, true);
    }
  }
}
  
void ListMenu::onDown()
{
  if (selectedMenuItemInd < nMenuItems - 1)
  {
    if (selectedMenuItemInd + 1  == pageStartInds[pageInd + 1])
    {
      ++selectedMenuItemInd;
      ++pageInd;
      drawPage(true);
    }
    else
    {
      menuItemList[selectedMenuItemInd]->draw(menuManager, false);
      ++selectedMenuItemInd;
      menuItemList[selectedMenuItemInd]->draw(menuManager, true);
    }
  }
}

void ListMenu::draw()
{
  if (menuVersion < menuManager.menuFormatSettings.menuVersion)
  {    
    updateLayout();
    menuVersion = menuManager.menuFormatSettings.menuVersion;
  }
  clearScreen(menuManager);
  drawHeader(menuManager, title);
  drawPage(false);
}

void ListMenu::resetSelection()
{
  selectedMenuItemInd = 0;
  pageInd = 0;
}

String ListMenu::getValue() const
{
  return getItemValue();
}

String ListMenu::getItemValue() const
{
  return nMenuItems ? menuItemList[selectedMenuItemInd]->getValue() : "";
}

bool ListMenu::setValue(const String& value)
{
  for (int i = 0; i < nMenuItems; ++i)
  {
    if (value == menuItemList[i]->getText())
    {
      selectedMenuItemInd = i;
      break;
    }
  }
  for (pageInd = 0; pageStartInds[pageInd] <= selectedMenuItemInd; ++pageInd)
  {
  }
  --pageInd;
  if (menuManager.currentMenu != this)
  {
    return false;
  }
  return true;
}

bool ListMenu::setItemValue(const String& value)
{
  return menuItemList[selectedMenuItemInd]->setValue(menuManager, value);
}

bool ListMenu::updateValue(const String& value)
{
  return setValue(value);
}

bool ListMenu::updateItemValue(const String& value)
{
  return menuItemList[selectedMenuItemInd]->updateValue(menuManager, value);
}

bool ListMenu::isValidValue(const String& value)
{
  for (int i = 0; i < nMenuItems; ++i)
  {
    if (menuItemList[selectedMenuItemInd]->getValue() == value)
    {
      return true;
    }
  }
  return false;
}

bool ListMenu::isValidItemValue(const String& value)
{
  return menuItemList[selectedMenuItemInd]->isValidValue(value);
}

ListMenu::~ListMenu()
{
  clear();
  delete[] pageStartInds;
}

void ListMenu::drawPage(bool needToClearThePage)
{
  if (needToClearThePage)
  {
    clearPage(menuManager);
  }
  for (int i = pageStartInds[pageInd]; i < pageStartInds[pageInd + 1]; ++i)
  {
    menuItemList[i]->draw(menuManager, i == selectedMenuItemInd);
  }
}

void ListMenu::updateLayout()
{
  int* tempPageStartInds = new int[nMenuItems + 1];
  tempPageStartInds[0] = 0;

  int x = menuManager.menuFormatSettings.itemsPaddingX;
  int y = menuManager.menuFormatSettings.headerHeight + menuManager.menuFormatSettings.headerPadding;
  int maxWidth = menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.itemsPaddingX;
  int n = 1;

  for (int  i = 0; i < nMenuItems; ++i)
  {
    menuItemList[i]->calculateSize(menuManager, maxWidth);
  }

  for (int i = 0; i < nMenuItems; ++i)
  {
    int width = menuItemList[i]->getWidth();
    int height = menuItemList[i]->getHeight();
    int remaingingHeight = menuManager.displaySettings.height - y;
    if (height <= remaingingHeight)
    {
      menuItemList[i]->setPosition(x, y);
      y += height + menuManager.menuFormatSettings.itemsPaddingY;
    }
    else if (i == tempPageStartInds[n - 1])
    {
      menuItemList[i]->trim(menuManager, remaingingHeight);
      menuItemList[i]->setPosition(x, y);
      y += remaingingHeight;
    }
    else
    {
      y = menuManager.menuFormatSettings.headerHeight + menuManager.menuFormatSettings.headerPadding;
      tempPageStartInds[n] = i;
      ++n;
      --i;
    }
  }
  tempPageStartInds[n] = nMenuItems;
  ++n;

  delete[] pageStartInds;
  pageStartInds = new int[n];
  for (int i = 0; i < n; ++i)
  {
    pageStartInds[i] = tempPageStartInds[i];
  }
  delete[] tempPageStartInds;

  for (int i = 0; i < n; ++i)
  {
    if (pageStartInds[i] > selectedMenuItemInd)
    {
      pageInd = i - 1;
      break;
    }
  }
}