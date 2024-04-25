#ifndef UPDATE_BOOL_ACTION
#define UPDATE_BOOL_ACTION

#include "IAction.h"

class IMenu;
class MenuManager;
class IMenuItem;

class UpdateBoolAction : public IAction
{
public:
  UpdateBoolAction(MenuManager& menuManager);
  void run() const override;
private:
  MenuManager& menuManager;
  int ind;
};

#endif
