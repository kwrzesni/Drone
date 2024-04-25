#ifndef CHANGE_INT_ACTION
#define CHANGE_INT_ACTION

#include "IAction.h"

class IMenu;
class MenuManager;

class ChangeIntAction : public IAction
{
public:
  ChangeIntAction(MenuManager& menuManager);
  void run() const override;
private:
  MenuManager& menuManager;
};

#endif
