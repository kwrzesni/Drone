#ifndef IACTION
#define IACTION

class IAction
{
public:
  IAction() = default;
  virtual void run() const = 0;
  virtual ~IAction() = default;
};

#endif
