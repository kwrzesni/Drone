#ifndef CHANGE_MENU_ACTION
#define CHANGE_MENU_ACTION

#include "IAction.h"

class IMenu;
class MenuManager;

class ChangeMenuAction : public IAction
{
public:
  ChangeMenuAction(IMenu& targetMenu, MenuManager& menuManager, int ind = -1);
  void run() const override;
private:
  IMenu& targetMenu;
  MenuManager& menuManager;
  int ind;
};

#endif
