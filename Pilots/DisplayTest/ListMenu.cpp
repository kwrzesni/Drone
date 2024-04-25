#include "MenuManager.h"
#include "DrawingUtils.h"

ListMenu::ListMenu(MenuManager& menuManager, String title)
  : IMenu{menuManager}, title{title}
{}

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

void ListMenu::draw()
{
  if (menuVersion < menuManager.menuFormatSettings.menuVersion)
  {    
    int* tempPageStartInds = new int[nMenuItems + 1];
    tempPageStartInds[0] = 0;

    int x = menuManager.menuFormatSettings.itemsPaddingX;
    int y = menuManager.menuFormatSettings.headerHeight + menuManager.menuFormatSettings.headerPadding;
    int maxWidth = menuManager.displaySettings.width - 2 * menuManager.menuFormatSettings.itemsPaddingX;
    int n = 1;

    for (int i = 0; i < nMenuItems; ++i)
    {
      String text = menuItemList[i]->getText();
      int width = calculateWidth(menuManager, text, maxWidth);
      int maxHeight = menuManager.displaySettings.height - y;
      int wholeTextHeight = calculateTextHeight(menuManager, text, width);
      if (y + wholeTextHeight <= menuManager.displaySettings.height)
      {
        menuItemList[i]->setPosition(x, y, width, wholeTextHeight);
        y += wholeTextHeight + menuManager.menuFormatSettings.itemsPaddingY;
      }
      else if (i == tempPageStartInds[n - 1])
      {
        menuItemList[i]->setPosition(x, y, width, maxHeight);
        y = menuManager.displaySettings.height;
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
    menuVersion = menuManager.menuFormatSettings.menuVersion;
  }
  clearScreen(menuManager);
  drawHeader(menuManager, title);
  drawPage(false);
}

void ListMenu::update(MenuControlCommand menuControlCommand)
{
  switch (menuControlCommand)
  {
    case MenuControlCommand::up:
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
      break;
    }
    case MenuControlCommand::down:
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
      break;
    }
    case MenuControlCommand::ok:
    {
      menuItemList[selectedMenuItemInd]->runAction();
      break;
    }
    case MenuControlCommand::back:
    {
      if (parent)
      {
        menuManager.changeMenu(*parent, false);
      }
      break;
    }
  }
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

void ListMenu::resetSelection()
{
  selectedMenuItemInd = 0;
  pageInd = 0;
}

String ListMenu::getValue() const
{
  return menuItemList[selectedMenuItemInd]->getValue();
}

String ListMenu::getValue(int ind) const
{
  return menuItemList[ind]->getValue();
}

void ListMenu::setValue(const String& value)
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
}

void ListMenu::setSelectedItemValue(const String& value)
{
  menuItemList[selectedMenuItemInd]->setValue(value);
}

void ListMenu::updateValue(const String& value)
{
  menuItemList[selectedMenuItemInd]->update(menuManager, value);
}