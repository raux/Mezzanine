#ifndef _workunits_h
#define _workunits_h

#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

class CatchPreInputWorkUnit : public Threading::DefaultWorkUnit
{
    protected:
        CatchApp* CatchApplication;
    public:
        CatchPreInputWorkUnit(CatchApp* Target);
        virtual ~CatchPreInputWorkUnit();

        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// ©atchPreInputWorkUnit

class CatchPostInputWorkUnit : public Threading::DefaultWorkUnit
{
    protected:
        CatchApp* CatchApplication;
    public:
        CatchPostInputWorkUnit(CatchApp* Target);
        virtual ~CatchPostInputWorkUnit();

        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// ©atchPostInputWorkUnit

class CatchPostUIWorkUnit : public Threading::DefaultWorkUnit
{
    protected:
        CatchApp* CatchApplication;
        RayQueryTool RayCaster;
    public:
        CatchPostUIWorkUnit(CatchApp* Target);
        virtual ~CatchPostUIWorkUnit();

        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// ©atchPostUIWorkUnit

class CatchPostGraphicsWorkUnit : public Threading::DefaultWorkUnit
{
    protected:
        CatchApp* CatchApplication;
    public:
        CatchPostGraphicsWorkUnit(CatchApp* Target);
        virtual ~CatchPostGraphicsWorkUnit();

        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
};// ©atchPostGraphicsWorkUnit

#endif
