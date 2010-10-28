#ifndef _areaofplay_h
#define _areaofplay_h

#include <areaeffect.h>

namespace phys
{
    class ActorBase;
}

using namespace phys;

class AreaOfPlay : public AreaEffect
{
    protected:
    public:
        AreaOfPlay(const String &name, Vector3 Location, World* world);
        ~AreaOfPlay();
        void ApplyEffect();
        bool IsInside(ActorBase* Actor);
};

#endif
