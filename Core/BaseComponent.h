#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

class BaseComponent
{
public:
    BaseComponent();

    void tick(long long tickCounter, int ticksPerSecond);
};

#endif // BASECOMPONENT_H
