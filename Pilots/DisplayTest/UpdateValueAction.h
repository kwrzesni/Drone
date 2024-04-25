#ifndef UPDATE_VALUE_ACTION
#define UPDATE_VALUE_ACTION

#include "IAction.h"

class IMenu;
class MenuManager;
class IMenuItem;

class UpdateValueAction : public IAction
{
public:
  UpdateValueAction(MenuManager& menuManager, int ind);
  void run() const override;
private:
  MenuManager& menuManager;
  int ind;
};

#endif
