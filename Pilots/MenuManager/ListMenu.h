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
  const Action& getAction() const override;
  void onUp() override;
  void onDown() override;
  void draw() override;
  void resetSelection() override;
  String getValue() const override;
  String getItemValue() const override;
  bool setValue(const String& value) override;
  bool setItemValue(const String& value) override;
  bool updateValue(const String& value) override;
  bool updateItemValue(const String& value) override;
  bool isValidValue(const String& value) override;
  bool isValidItemValue(const String& value) override;
  void updateLayout();
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
