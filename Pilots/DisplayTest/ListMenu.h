#ifndef LIST_MENU
#define LIST_MENU

#include "IMenu.h"
#include "IMenuItem.h"
#include <initializer_list>

enum class MenuControlCommand;

class ListMenu : public IMenu
{
public:
  ListMenu(MenuManager& menuManager, String title);
  void assignMenuItems(std::initializer_list<IMenuItem*> menuItems);
  void clear();
  void draw();
  void update(MenuControlCommand menuControlCommand) override;
  void resetSelection() override;
  String getValue() const override;
  String getValue(int ind) const override;
  void setValue(const String& value) override;
  void setSelectedItemValue(const String& value) override;
  void updateValue(const String& value) override;
  ~ListMenu();
private:
  void drawPage(bool needToClearThePage);
private:
  String title;
  IMenuItem** menuItemList = nullptr;
  int* pageStartInds = nullptr;
  int nMenuItems = 0;
  int selectedMenuItemInd = 0;
  int pageInd = 0;
};

#endif
