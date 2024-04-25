#ifndef CHANGE_COLOR_ACTION
#define CHANGE_COLOR_ACTION

#include "IAction.h"
#include "Colors.h"

class IMenu;
class MenuManager;

class ChangeColorAction : public IAction
{
public:
  ChangeColorAction(MenuManager& menuManager);
  void run() const override;
private:
  MenuManager& menuManager;
};

#endif
