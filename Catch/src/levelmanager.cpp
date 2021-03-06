#ifndef _levelmanager_cpp
#define _levelmanager_cpp

#include "levelmanager.h"
#include "levelscorer.h"
#include "catchapp.h"
#include "throwablegenerator.h"
#include "leveltriggers.h"
#include "player.h"

#include "Scripting/scripting.h"

void LoadFerris()
{
    // Get our major roots
    CatchApp* GameApp = CatchApp::GetCatchAppPointer();
    World* CatchWorld = GameApp->GetTheWorld();
    // Get managers
    Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    //Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( CatchWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( CatchWorld->GetManager(ManagerBase::MT_SceneManager) );
    EntityManager* EntMan = static_cast<EntityManager*>( CatchWorld->GetManager(ManagerBase::MT_EntityManager) );
    // Init Resources
    String CommonGroup("Common");
    String FerrisGroup("Ferris");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"Ferris.lvl", Resource::AT_Zip, FerrisGroup, false);
    ResourceMan->InitAssetGroup(FerrisGroup);

    // Scoring and Shop Setup
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Shop->SetLevelCash(100);

    // Camera Setup
    GameApp->GetPlayer()->InitWorldEntities(CatchWorld);
    GameApp->GetPlayer()->GetControl().GetCamera()->SetLocation(Vector3(0,0,425));

    // Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(150,250,-200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,-120),Vector3(15,-10,-120),Vector3(0,10,-120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("Ferris.bullet",FerrisGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            if(10 > ((CompoundCollisionShape*)ToChange)->GetNumChildren())
            {
                CShapeMan->SetNameForUnnamedShape("Tray",ToChange);
            }else{
                CShapeMan->SetNameForUnnamedShape("Wheel",ToChange);
            }
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("Ferris2.bullet");// */
    //----------

    // Create the Wheel
    RigidDebris* FerrisWheel = EntMan->CreateRigidDebris(100.0);
    FerrisWheel->SetName("FerrisWheel");
    FerrisWheel->GetRigidProxy()->SetDamping(0,0.1);
    FerrisWheel->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Wheel") ); // The names aren't being loaded as we expected
    FerrisWheel->GetItemProxy()->SetMesh("wheel.mesh",FerrisGroup);
    FerrisWheel->SetLocation(100,0,0);
    FerrisWheel->AddToWorld();

    // Create the trayz
    Real TrayMass = 50.0;
    RigidDebris* Tray1 = EntMan->CreateRigidDebris(TrayMass);
    Tray1->SetName("Tray1");
    Tray1->GetRigidProxy()->SetDamping(0,0.1);
    Tray1->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray1->GetItemProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray1->SetLocation(30.4,14.3,0);
    Tray1->AddToWorld();
    RigidDebris* Tray2 = EntMan->CreateRigidDebris(TrayMass);
    Tray2->SetName("Tray2");
    Tray2->GetRigidProxy()->SetDamping(0,0.1);
    Tray2->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray2->GetItemProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray2->SetLocation(169.6,14.3,0);
    Tray2->AddToWorld();
    RigidDebris* Tray3 = EntMan->CreateRigidDebris(TrayMass);
    Tray3->SetName("Tray3");
    Tray3->GetRigidProxy()->SetDamping(0,0.1);
    Tray3->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray3->GetItemProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray3->SetLocation(30.4,-42.7,0);
    Tray3->AddToWorld();
    RigidDebris* Tray4 = EntMan->CreateRigidDebris(TrayMass);
    Tray4->SetName("Tray4");
    Tray4->GetRigidProxy()->SetDamping(0,0.1);
    Tray4->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray4->GetItemProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray4->SetLocation(169.6,-42.7,0);
    Tray4->AddToWorld();
    RigidDebris* Tray5 = EntMan->CreateRigidDebris(TrayMass);
    Tray5->SetName("Tray5");
    Tray5->GetRigidProxy()->SetDamping(0,0.1);
    Tray5->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray5->GetItemProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray5->SetLocation(71.5,55.4,0);
    Tray5->AddToWorld();
    RigidDebris* Tray6 = EntMan->CreateRigidDebris(TrayMass);
    Tray6->SetName("Tray6");
    Tray6->GetRigidProxy()->SetDamping(0,0.1);
    Tray6->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray6->GetItemProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray6->SetLocation(128.5,55.4,0);
    Tray6->AddToWorld();
    RigidDebris* Tray7 = EntMan->CreateRigidDebris(TrayMass);
    Tray7->SetName("Tray7");
    Tray7->GetRigidProxy()->SetDamping(0,0.1);
    Tray7->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray7->GetItemProxy()->SetMesh("bluetray.mesh",FerrisGroup);
    Tray7->SetLocation(71.5,-83.8,0);
    Tray7->AddToWorld();
    RigidDebris* Tray8 = EntMan->CreateRigidDebris(TrayMass);
    Tray8->SetName("Tray8");
    Tray8->GetRigidProxy()->SetDamping(0,0.1);
    Tray8->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray8->GetItemProxy()->SetMesh("purpletray.mesh",FerrisGroup);
    Tray8->SetLocation(128.5,-83.8,0);
    Tray8->AddToWorld();// */

    // Create world anchor for the wheel, which will allow it to spin.
    Physics::HingeConstraint* WheelAnchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Vector3(0,0,0),Vector3(0,0,1));
    WheelAnchor->SetAllowCollisions(false);
    WheelAnchor->EnableConstraint(true);// */

    // Create the series of hinges for connecting the 8 trays to the wheel
    Physics::HingeConstraint* Tray1Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray1->GetRigidProxy(),Vector3(-69.6,28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray1Anchor->SetAllowCollisions(false);
    Tray1Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray2Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray2->GetRigidProxy(),Vector3(69.6,28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray2Anchor->SetAllowCollisions(false);
    Tray2Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray3Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray3->GetRigidProxy(),Vector3(-69.6,-28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray3Anchor->SetAllowCollisions(false);
    Tray3Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray4Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray4->GetRigidProxy(),Vector3(69.6,-28.5,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray4Anchor->SetAllowCollisions(false);
    Tray4Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray5Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray5->GetRigidProxy(),Vector3(-28.5,69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray5Anchor->SetAllowCollisions(false);
    Tray5Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray6Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray6->GetRigidProxy(),Vector3(28.5,69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray6Anchor->SetAllowCollisions(false);
    Tray6Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray7Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray7->GetRigidProxy(),Vector3(-28.5,-69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray7Anchor->SetAllowCollisions(false);
    Tray7Anchor->EnableConstraint(true);
    Physics::HingeConstraint* Tray8Anchor = PhysMan->CreateHingeConstraint(FerrisWheel->GetRigidProxy(),Tray8->GetRigidProxy(),Vector3(28.5,-69.6,0),Vector3(0,14.2,0),Vector3(0,0,1),Vector3(0,0,1));
    Tray8Anchor->SetAllowCollisions(false);
    Tray8Anchor->EnableConstraint(true);// */

    // Create some throwable objects
    ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    Physics::CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    Physics::CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    Physics::CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    Physics::CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);

    RigidDebris* Uranium1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium1->GetRigidProxy()->SetCollisionShape(UraniumCS);
    //Uranium1->GetRigidProxy()->SetStickyData(1);
    Uranium1->SetLocation(-145,60,0);
    Uranium1->AddToWorld();
    GameApp->AddThrowable(Uranium1);
    RigidDebris* Uranium2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium2->GetRigidProxy()->SetCollisionShape(UraniumCS);
    //Uranium2->GetRigidProxy()->SetStickyData(1);
    Uranium2->SetLocation(-195,60,0);
    Uranium2->AddToWorld();
    GameApp->AddThrowable(Uranium2);// */
    RigidDebris* Lead1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead1->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-145,20,0);
    Lead1->AddToWorld();
    GameApp->AddThrowable(Lead1);
    RigidDebris* Lead2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead2->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-195,20,0);
    Lead2->AddToWorld();
    GameApp->AddThrowable(Lead2);// */
    RigidDebris* Clay1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Clay") );
    Clay1->GetRigidProxy()->SetCollisionShape(ClayCS);
    Clay1->SetLocation(-145,-20,0);
    Clay1->AddToWorld();
    GameApp->AddThrowable(Clay1);
    RigidDebris* Clay2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Clay") );
    Clay2->GetRigidProxy()->SetCollisionShape(ClayCS);
    Clay2->SetLocation(-195,-20,0);
    Clay2->AddToWorld();
    GameApp->AddThrowable(Clay2);// */
    RigidDebris* Pyrite1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite1->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-145,-60,0);
    Pyrite1->AddToWorld();
    GameApp->AddThrowable(Pyrite1);
    RigidDebris* Pyrite2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite2->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-195,-60,0);
    Pyrite2->AddToWorld();
    GameApp->AddThrowable(Pyrite2);// */

    // Create the zones
    Vector3 StartSize(50,130,25);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone->SetName("StartArea1");
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(StartSize,4.0).GenerateMesh("StartAreaMesh",FerrisGroup,"Basic/Green",CommonGroup) );
    StartZone->SetParticleMinimumTimeToLive(11.0);
    StartZone->SetParticleMaximumTimeToLive(15.0);
    StartZone->SetLocation(-170,0,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(100,100,30);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZoneParams) );
    ScoreZone->SetName("ScoreArea1");
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(ScoreSize,4.0).GenerateMesh("ScoreAreaMesh",FerrisGroup,"Basic/Blue",CommonGroup) );
    ScoreZone->SetLocation(100,0,0);
    ScoreZone->AddToWorld();// */

    Vector3 BonusScoreSize(18,18,30);
    NameValuePairMap BonusZoneParams;
    BonusZoneParams["SizeX"] = StringTools::ConvertToString(BonusScoreSize.X);
    BonusZoneParams["SizeY"] = StringTools::ConvertToString(BonusScoreSize.Y);
    BonusZoneParams["SizeZ"] = StringTools::ConvertToString(BonusScoreSize.Z);
    Physics::CollisionShape* BonusScoreShape = new Physics::BoxCollisionShape("BonusAreaShape",BonusScoreSize);
    Graphics::Mesh* BonusScoreMesh = Graphics::Procedural::BoxCornerGenerator(BonusScoreSize,4.0).GenerateMesh("BonusAreaMesh",FerrisGroup,"Basic/Purple",CommonGroup);

    ScoreArea* BonusZone1 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",BonusZoneParams) );
    BonusZone1->SetName("BonusArea1");
    BonusZone1->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone1->GetItemProxy()->SetMesh(BonusScoreMesh);
    BonusZone1->SetScoreMultiplier(2.0);
    BonusZone1->SetLocation(169.6,14.3,0);
    BonusZone1->AddToWorld();
    BonusZone1->SetSyncTarget(Tray2);// */
    ScoreArea* BonusZone2 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",BonusZoneParams) );
    BonusZone2->SetName("BonusArea2");
    BonusZone2->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone2->GetItemProxy()->SetMesh(BonusScoreMesh);
    BonusZone2->SetScoreMultiplier(2.0);
    BonusZone2->SetLocation(30.4,-42.7,0);
    BonusZone2->AddToWorld();
    BonusZone2->SetSyncTarget(Tray3);// */
    ScoreArea* BonusZone3 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",BonusZoneParams) );
    BonusZone3->SetName("BonusArea3");
    BonusZone3->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone3->GetItemProxy()->SetMesh(BonusScoreMesh);
    BonusZone3->SetScoreMultiplier(2.0);
    BonusZone3->SetLocation(71.5,55.4,0);
    BonusZone3->AddToWorld();
    BonusZone3->SetSyncTarget(Tray5);// */
    ScoreArea* BonusZone4 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",BonusZoneParams) );
    BonusZone4->SetName("BonusArea4");
    BonusZone4->GetGhostProxy()->SetCollisionShape(BonusScoreShape);
    BonusZone4->GetItemProxy()->SetMesh(BonusScoreMesh);
    BonusZone4->SetScoreMultiplier(2.0);
    BonusZone4->SetLocation(128.5,-83.8,0);
    BonusZone4->AddToWorld();
    BonusZone4->SetSyncTarget(Tray8);// */
}

void LoadBigCurve()
{
    // Get our major roots
    CatchApp* GameApp = CatchApp::GetCatchAppPointer();
    World* CatchWorld = GameApp->GetTheWorld();
    // Get managers
    Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    //Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( CatchWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( CatchWorld->GetManager(ManagerBase::MT_SceneManager) );
    EntityManager* EntMan = static_cast<EntityManager*>( CatchWorld->GetManager(ManagerBase::MT_EntityManager) );

    // Init Resources
    String CommonGroup("Common");
    String BigCurveGroup("BigCurve");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"BigCurve.lvl", Resource::AT_Zip, BigCurveGroup, false);
    ResourceMan->InitAssetGroup(BigCurveGroup);

    // Scoring and Shop Setup
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Shop->SetLevelCash(100);

    // Camera Setup
    GameApp->GetPlayer()->InitWorldEntities(CatchWorld);
    GameApp->GetPlayer()->GetControl().GetCamera()->SetLocation(Vector3(0,0,425));

    // Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,-120),Vector3(15,-10,-120),Vector3(0,10,-120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    // -no files to be loaded for this level-

    // Create the Cave
    RigidDebris* CurveCave = EntMan->CreateRigidDebris(0);
    CurveCave->SetName("CurveCave");
    CurveCave->GetRigidProxy()->SetCollisionShape(CShapeMan->GenerateStaticTriMesh("Cave","cave.mesh",BigCurveGroup));
    CurveCave->GetItemProxy()->SetMesh("cave.mesh",BigCurveGroup);
    CurveCave->SetLocation(130,0,-10);
    CurveCave->SetScale(1.05,1.05,1.05);
    CurveCave->AddToWorld();

    // Create some throwable objects
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    Physics::CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    Physics::CollisionShape* RubberCS = CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);

    RigidDebris* Pyrite1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite1->GetRigidProxy()->SetCollisionShape( PyriteCS );
    Pyrite1->SetLocation(-155,-20,0);
    Pyrite1->AddToWorld();
    GameApp->AddThrowable(Pyrite1);
    RigidDebris* Pyrite2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite2->GetRigidProxy()->SetCollisionShape( PyriteCS );
    Pyrite2->SetLocation(-205,-20,0);
    Pyrite2->AddToWorld();
    GameApp->AddThrowable(Pyrite2);// */
    RigidDebris* Rubber1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber1->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber1->SetLocation(-155,-60,0);
    Rubber1->AddToWorld();
    GameApp->AddThrowable(Rubber1);
    RigidDebris* Rubber2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber2->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber2->SetLocation(-205,-60,0);
    Rubber2->AddToWorld();
    GameApp->AddThrowable(Rubber2);
    RigidDebris* Rubber3 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber3->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber3->SetLocation(-155,-100,0);
    Rubber3->AddToWorld();
    GameApp->AddThrowable(Rubber3);
    RigidDebris* Rubber4 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber4->GetRigidProxy()->SetCollisionShape( RubberCS );
    Rubber4->SetLocation(-205,-100,0);
    Rubber4->AddToWorld();
    GameApp->AddThrowable(Rubber4);// */

    // Create the zones
    Vector3 StartSize(50,80,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone->SetName("StartArea1");
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(StartSize,4.0).GenerateMesh("StartAreaMesh",BigCurveGroup,"Basic/Green",CommonGroup) );
    StartZone->SetParticleMinimumTimeToLive(8.0);
    StartZone->SetParticleMaximumTimeToLive(10.0);
    StartZone->SetLocation(-180,-60,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(50,40,50);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZoneParams) );
    ScoreZone->SetName("ScoreArea1");
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(ScoreSize,4.0).GenerateMesh("ScoreAreaMesh",BigCurveGroup,"Basic/Blue",CommonGroup) );
    ScoreZone->SetLocation(120,-20,-10);
    ScoreZone->AddToWorld();// */
}

void LoadBlowsNotSucks()
{
    // Get our major roots
    CatchApp* GameApp = CatchApp::GetCatchAppPointer();
    World* CatchWorld = GameApp->GetTheWorld();
    // Get managers
    Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    //Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( CatchWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( CatchWorld->GetManager(ManagerBase::MT_SceneManager) );
    EntityManager* EntMan = static_cast<EntityManager*>( CatchWorld->GetManager(ManagerBase::MT_EntityManager) );

    // Init Resources
    String CommonGroup("Common");
    String BlowsNotSucksGroup("BlowsNotSucks");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"BlowsNotSucks.lvl", Resource::AT_Zip, BlowsNotSucksGroup, false);
    ResourceMan->InitAssetGroup(BlowsNotSucksGroup);

    // Scoring and Shop Setup
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Shop->SetLevelCash(100);

    // Camera Setup
    GameApp->GetPlayer()->InitWorldEntities(CatchWorld);
    GameApp->GetPlayer()->GetControl().GetCamera()->SetLocation(Vector3(0,0,425));

    // Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,200,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,-120),Vector3(15,-10,-120),Vector3(0,10,-120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("BlowsNotSucks.bullet",BlowsNotSucksGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            CShapeMan->SetNameForUnnamedShape("Fan",ToChange);
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("BlowsNotSucks.bullet");// */
    //----------

    // Create the fan
    RigidDebris* Fan = EntMan->CreateRigidDebris(25);
    Fan->SetName("Fan");
    Fan->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Fan") );
    Fan->GetRigidProxy()->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
    Fan->GetItemProxy()->SetMesh("fan.mesh",BlowsNotSucksGroup);
    Fan->SetLocation(103.388,-58.888,7.5);
    Fan->SetOrientation(Quaternion(MathTools::GetQuarterPi(),Vector3(0,0,1)));
    Fan->AddToWorld();

    RigidDebris* FanBody = EntMan->CreateRigidDebris(0);
    FanBody->SetName("FanBody");
    FanBody->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("Body","body.mesh",BlowsNotSucksGroup) );
    FanBody->GetItemProxy()->SetMesh("body.mesh",BlowsNotSucksGroup);
    FanBody->SetLocation(132.5,-70,25);
    FanBody->AddToWorld();

    RigidDebris* FanButton = EntMan->CreateRigidDebris(2.5);
    FanButton->SetName("FanButton");
    FanButton->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateConvexHull("Button","button.mesh",BlowsNotSucksGroup) );
    FanButton->GetRigidProxy()->SetIgnoreWorldGravity(true);
    FanButton->GetRigidProxy()->SetAngularMovementFactor(Vector3(0,0,0));
    FanButton->GetItemProxy()->SetMesh("button.mesh",BlowsNotSucksGroup);
    FanButton->SetLocation(-10.5,-107.0,-0.5);
    FanButton->AddToWorld();

    // Create the series of constraints for assembling the fan
    Physics::HingeConstraint* FanToBody = PhysMan->CreateHingeConstraint(FanBody->GetRigidProxy(),Fan->GetRigidProxy(),Vector3(-12,-6,-17.5),Vector3(0,-24.2,0),Vector3(-1,1,0).Normalize(),Vector3(0,1,0));
    FanToBody->SetLimits(1.0,-1.0);
    FanToBody->SetAllowCollisions(false);
    FanToBody->EnableConstraint(true);

    Physics::SixDofSpringConstraint* ButtonToBody = PhysMan->CreateSixDofSpringConstraint(FanBody->GetRigidProxy(),FanButton->GetRigidProxy(),Transform(Vector3(-143,-52,-25.5),Quaternion(0,0,0,1)),Transform(Vector3(0,0,0),Quaternion(0,0,0,1)));
    ButtonToBody->SetLinearLimitUpper(Vector3(0,15,0));
    ButtonToBody->SetLinearLimitLower(Vector3(0,0,0));
    ButtonToBody->SetAngularLimitUpper(Vector3(0,0,0));
    ButtonToBody->SetAngularLimitLower(Vector3(0,0,0));
    ButtonToBody->SetSpringEnabled(1,true);
    ButtonToBody->SetSpringStiffness(1,500.f);
    ButtonToBody->SetSpringDamping(1,0.5);
    ButtonToBody->CalculateSpringEquilibriumPoint(1);
    ButtonToBody->SetAllowCollisions(false);
    ButtonToBody->EnableConstraint(true);

    // Create the field of force for the wind generated by the fan
    Vector3 FanWindSize(60,115,60);
    Quaternion FanWindOrientation(MathTools::GetQuarterPi(),Vector3(0,0,1));
    FieldOfForce* FanWind = EntMan->CreateFieldOfForce();
    FanWind->SetName("FanWind");
    FanWind->GetGhostProxy()->SetCollisionShape( new Physics::CylinderCollisionShape("FanWindShape",60,115,Vector3::Unit_Y()) );
    FanWind->SetOrientation(FanWindOrientation);
    FanWind->SetDirectionOfForce(Vector3(-1,1,0));
    FanWind->SetAttenuation(0.2f,Mezzanine::Att_Linear,Vector3(-125.5,-81,7.5));
    FanWind->SetFieldStrength(1000.f);
    FanWind->SetLocation(5,40,7.5);
    FanWind->AddToWorld();
    Graphics::ParticleSystemProxy* WindParticle = SceneMan->CreateParticleSystemProxy("WindTunnel");
    WindParticle->GetEmitter(0)->SetCustomParam("width",StringTools::ConvertToString(FanWindSize.X * (Real)1.8));
    WindParticle->GetEmitter(0)->SetCustomParam("height",StringTools::ConvertToString(FanWindSize.Y * (Real)0.6));
    WindParticle->GetEmitter(0)->SetCustomParam("depth",StringTools::ConvertToString(FanWindSize.Z * (Real)1.8));
    WindParticle->GetEmitter(0)->SetCustomParam("position","0 -60.0 0");
    WindParticle->SetLocation(5,40,7.5);
    WindParticle->SetOrientation(FanWindOrientation);
    WindParticle->Activate();// */

    // Create the trigger for the fan
    BNS_Fan* FanTrigger = new BNS_Fan("FanTigger",FanToBody,ButtonToBody,Fan,FanWind,WindParticle);
    PhysMan->AddWorldTrigger(FanTrigger);

    // Create the goal tray
    RigidDebris* GoalTray = EntMan->CreateRigidDebris(0);
    GoalTray->SetName("GoalTray");
    GoalTray->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("GoalShape","tray.mesh",BlowsNotSucksGroup) );
    GoalTray->GetItemProxy()->SetMesh("tray.mesh",BlowsNotSucksGroup);
    GoalTray->SetLocation(-170,100,0);
    GoalTray->AddToWorld();

    // Create some throwable objects
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    ThrowableData* StyrofoamData = ThrowableGenerator::GetThrowableData("Styrofoam");
    Physics::CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);
    Physics::CollisionShape* StyrofoamCS = CShapeMan->GenerateConvexHull("StyrofoamCS",StyrofoamData->MeshName,CommonGroup);

    RigidDebris* Uranium1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium1->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,-20,0);
    Uranium1->AddToWorld();
    GameApp->AddThrowable(Uranium1);
    RigidDebris* Uranium2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium2->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,-20,0);
    Uranium2->AddToWorld();
    GameApp->AddThrowable(Uranium2);// */
    RigidDebris* Styrofoam1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam1->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam1->SetLocation(-155,-60,0);
    Styrofoam1->AddToWorld();
    GameApp->AddThrowable(Styrofoam1);
    RigidDebris* Styrofoam2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam2->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam2->SetLocation(-185,-60,0);
    Styrofoam2->AddToWorld();
    GameApp->AddThrowable(Styrofoam2);
    RigidDebris* Styrofoam3 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam3->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam3->SetLocation(-155,-100,0);
    Styrofoam3->AddToWorld();
    GameApp->AddThrowable(Styrofoam3);
    RigidDebris* Styrofoam4 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Styrofoam") );
    Styrofoam4->GetRigidProxy()->SetCollisionShape(StyrofoamCS);
    Styrofoam4->SetLocation(-185,-100,0);
    Styrofoam4->AddToWorld();
    GameApp->AddThrowable(Styrofoam4);// */

    // Create the zones
    Vector3 StartSize(50,70,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone->SetName("StartArea1");
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(StartSize,4.0).GenerateMesh("StartAreaMesh",BlowsNotSucksGroup,"Basic/Green",CommonGroup) );
    StartZone->SetParticleMinimumTimeToLive(6.0);
    StartZone->SetParticleMaximumTimeToLive(9.0);
    StartZone->SetLocation(-170,-70,0);
    StartZone->AddToWorld();// */

    Vector3 Score1Size(25,21,25);
    NameValuePairMap ScoreZone1Params;
    ScoreZone1Params["SizeX"] = StringTools::ConvertToString(Score1Size.X);
    ScoreZone1Params["SizeY"] = StringTools::ConvertToString(Score1Size.Y);
    ScoreZone1Params["SizeZ"] = StringTools::ConvertToString(Score1Size.Z);
    Physics::CollisionShape* ScoreArea1Shape = new Physics::BoxCollisionShape("ScoreArea1Shape",Score1Size);
    Graphics::Mesh* ScoreArea1Mesh = Graphics::Procedural::BoxCornerGenerator(Score1Size,4.0).GenerateMesh("ScoreArea1Mesh",BlowsNotSucksGroup,"Basic/Blue",CommonGroup);

    ScoreArea* ScoreZone1 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZone1Params) );
    ScoreZone1->SetName("ScoreArea1");
    ScoreZone1->GetGhostProxy()->SetCollisionShape(ScoreArea1Shape);
    ScoreZone1->GetItemProxy()->SetMesh(ScoreArea1Mesh);
    ScoreZone1->SetLocation(-10.5,-100.0,0.0);
    ScoreZone1->AddToWorld();

    Vector3 Score2Size(55,48,15);
    NameValuePairMap ScoreZone2Params;
    ScoreZone2Params["SizeX"] = StringTools::ConvertToString(Score2Size.X);
    ScoreZone2Params["SizeY"] = StringTools::ConvertToString(Score2Size.Y);
    ScoreZone2Params["SizeZ"] = StringTools::ConvertToString(Score2Size.Z);
    Physics::CollisionShape* ScoreArea2Shape = new Physics::BoxCollisionShape("ScoreArea2Shape",Score2Size);
    Graphics::Mesh* ScoreArea2Mesh = Graphics::Procedural::BoxCornerGenerator(Score2Size,4.0).GenerateMesh("ScoreArea2Mesh",BlowsNotSucksGroup,"Basic/Blue",CommonGroup);

    ScoreArea* ScoreZone2 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZone2Params) );
    ScoreZone2->SetName("ScoreArea2");
    ScoreZone2->GetGhostProxy()->SetCollisionShape(ScoreArea2Shape);
    ScoreZone2->GetItemProxy()->SetMesh(ScoreArea2Mesh);
    ScoreZone2->SetLocation(-170,100,0);
    ScoreZone2->AddToWorld();// */
}

void LoadJustice()
{
    // Get our major roots
    CatchApp* GameApp = CatchApp::GetCatchAppPointer();
    World* CatchWorld = GameApp->GetTheWorld();
    // Get managers
    Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    //Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( CatchWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( CatchWorld->GetManager(ManagerBase::MT_SceneManager) );
    EntityManager* EntMan = static_cast<EntityManager*>( CatchWorld->GetManager(ManagerBase::MT_EntityManager) );

    // Init Resources
    String CommonGroup("Common");
    String JusticeGroup("Justice");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"Justice.lvl", Resource::AT_Zip, JusticeGroup, false);
    ResourceMan->InitAssetGroup(JusticeGroup);

    // Scoring and Shop Setup
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Shop->SetLevelCash(100);

    // Camera Setup
    GameApp->GetPlayer()->InitWorldEntities(CatchWorld);
    GameApp->GetPlayer()->GetControl().GetCamera()->SetLocation(Vector3(0,0,425));

    // Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,-120),Vector3(15,-10,-120),Vector3(0,10,-120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("Justice.bullet",JusticeGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            if(1 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Union",ToChange);
            else if(8 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Scale",ToChange);
            else if(9 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Tray",ToChange);
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("Justice.bullet");// */
    //----------

    // Create Lady Justice
    RigidDebris* LadyJustice = EntMan->CreateRigidDebris(0);
    LadyJustice->SetName("LadyJustice");
    LadyJustice->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("LadyShape","lady.mesh",JusticeGroup) );
    LadyJustice->GetItemProxy()->SetMesh("lady.mesh",JusticeGroup);
    LadyJustice->SetLocation(12,-60,-103.35);
    LadyJustice->AddToWorld();

    // Create the scale object
    Real ScaleMass = 50.0;
    RigidDebris* JusticeScale = EntMan->CreateRigidDebris(ScaleMass);
    JusticeScale->SetName("JusticeScale");
    JusticeScale->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Scale") );
    JusticeScale->GetRigidProxy()->SetDamping(0.5,0.80);
    JusticeScale->GetItemProxy()->SetMesh("scale.mesh",JusticeGroup);
    JusticeScale->SetLocation(0,37.4,0);
    JusticeScale->AddToWorld();

    // Create the union objects
    Real UnionMass = 5.0;
    RigidDebris* Union1 = EntMan->CreateRigidDebris(UnionMass);
    Union1->SetName("Union1");
    Union1->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union1->GetItemProxy()->SetMesh("union.mesh",JusticeGroup);
    Union1->SetLocation(-112,0.2,-16.4);
    Union1->AddToWorld();
    RigidDebris* Union2 = EntMan->CreateRigidDebris(UnionMass);
    Union2->SetName("Union2");
    Union2->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union2->GetItemProxy()->SetMesh("union.mesh",JusticeGroup);
    Union2->SetLocation(-112,0.2,16.4);
    Union2->AddToWorld();
    RigidDebris* Union3 = EntMan->CreateRigidDebris(UnionMass);
    Union3->SetName("Union3");
    Union3->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union3->GetItemProxy()->SetMesh("union.mesh",JusticeGroup);
    Union3->SetLocation(112,0.2,-16.4);
    Union3->AddToWorld();
    RigidDebris* Union4 = EntMan->CreateRigidDebris(UnionMass);
    Union4->SetName("Union4");
    Union4->GetRigidProxy()->SetCollisionShape(CShapeMan->GetShape("Union"));
    Union4->GetItemProxy()->SetMesh("union.mesh",JusticeGroup);
    Union4->SetLocation(112,0.2,16.4);
    Union4->AddToWorld();

    // Create the trays
    Real TrayMass = 25.0;
    RigidDebris* Tray1 = EntMan->CreateRigidDebris(TrayMass);
    Tray1->SetName("Tray1");
    Tray1->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray1->GetRigidProxy()->SetLinearMovementFactor(Vector3(1,1,0));
    Tray1->GetRigidProxy()->SetAngularMovementFactor(Vector3(1,0,1));
    Tray1->GetRigidProxy()->SetDamping(0.0,0.3);
    Tray1->GetItemProxy()->SetMesh("tray.mesh",JusticeGroup);
    Tray1->SetLocation(-112,-35,0);
    Tray1->AddToWorld();
    RigidDebris* Tray2 = EntMan->CreateRigidDebris(TrayMass);
    Tray2->SetName("Tray2");
    Tray2->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Tray") );
    Tray2->GetRigidProxy()->SetLinearMovementFactor(Vector3(1,1,0));
    Tray2->GetRigidProxy()->SetAngularMovementFactor(Vector3(1,0,1));
    Tray2->GetRigidProxy()->SetDamping(0.0,0.3);
    Tray2->GetItemProxy()->SetMesh("tray.mesh",JusticeGroup);
    Tray2->SetLocation(112,-35,0);
    Tray2->AddToWorld();

    // Create the series of constraints for connecting all the pieces of the scale together
    // Starting with creating the anchor for the scale
    Physics::HingeConstraint* ScaleAnchor = PhysMan->CreateHingeConstraint(LadyJustice->GetRigidProxy(),JusticeScale->GetRigidProxy(),Vector3(-12,127.4,103.35),Vector3(0,30,0),Vector3(0,0,1),Vector3(0,0,1));
    ScaleAnchor->SetLimits( -(MathTools::GetPi() * 0.20),(MathTools::GetPi() * 0.20) );
    ScaleAnchor->SetAllowCollisions(false);
    ScaleAnchor->EnableConstraint(true);

    // Original X distance from pivot on scale is 112.7, but space was needed to prevent collsions.
    // Create the scale-to-union constraints
    Physics::Point2PointConstraint* U1S = PhysMan->CreatePoint2PointConstraint(JusticeScale->GetRigidProxy(),Union1->GetRigidProxy(),Vector3(-112.0,-20,-16.4),Vector3(0,17.2,0));//58,47.4,0 // -54,27.4,-16.4
    U1S->SetAllowCollisions(false);
    U1S->EnableConstraint(true);
    Physics::Point2PointConstraint* U2S = PhysMan->CreatePoint2PointConstraint(JusticeScale->GetRigidProxy(),Union2->GetRigidProxy(),Vector3(-112.0,-20,16.4),Vector3(0,17.2,0));//58,47.4,0 // -54,27.4,16.4
    U2S->SetAllowCollisions(false);
    U2S->EnableConstraint(true);
    Physics::Point2PointConstraint* U3S = PhysMan->CreatePoint2PointConstraint(JusticeScale->GetRigidProxy(),Union3->GetRigidProxy(),Vector3(112.0,-20,-16.4),Vector3(0,17.2,0));//58,47.4,0 // 170,27.4,-16.4
    U3S->SetAllowCollisions(false);
    U3S->EnableConstraint(true);
    Physics::Point2PointConstraint* U4S = PhysMan->CreatePoint2PointConstraint(JusticeScale->GetRigidProxy(),Union4->GetRigidProxy(),Vector3(112.0,-20,16.4),Vector3(0,17.2,0));//58,47.4,0 // 170,27.4,16.4
    U4S->SetAllowCollisions(false);
    U4S->EnableConstraint(true);

    // Create the union-to-tray constraints
    Physics::Point2PointConstraint* U1T = PhysMan->CreatePoint2PointConstraint(Union1->GetRigidProxy(),Tray1->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,-16.4));// -54,10.2,-16.4 // -54,-25,0
    U1T->SetAllowCollisions(false);
    U1T->EnableConstraint(true);
    Physics::Point2PointConstraint* U2T = PhysMan->CreatePoint2PointConstraint(Union2->GetRigidProxy(),Tray1->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,16.4));// -54,10.2,16.4 // -54,-25,0
    U2T->SetAllowCollisions(false);
    U2T->EnableConstraint(true);
    Physics::Point2PointConstraint* U3T = PhysMan->CreatePoint2PointConstraint(Union3->GetRigidProxy(),Tray2->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,-16.4));// 170,10.2,-16.4 // 170,-25,0
    U3T->SetAllowCollisions(false);
    U3T->EnableConstraint(true);
    Physics::Point2PointConstraint* U4T = PhysMan->CreatePoint2PointConstraint(Union4->GetRigidProxy(),Tray2->GetRigidProxy(),Vector3(0,-17.2,0),Vector3(0,18,16.4));// 170,10.2,16.4 // 170,-25,0
    U4T->SetAllowCollisions(false);
    U4T->EnableConstraint(true);

    // Create some throwable objects
    //ThrowableData* ClayData = ThrowableGenerator::GetThrowableData("Clay");
    //ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    ThrowableData* UraniumData = ThrowableGenerator::GetThrowableData("Uranium");
    //Physics::CollisionShape* ClayCS = CShapeMan->GenerateConvexHull("ClayCS",ClayData->MeshName,CommonGroup);
    //Physics::CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LeadCS",LeadData->MeshName,CommonGroup);
    Physics::CollisionShape* UraniumCS = CShapeMan->GenerateConvexHull("UraniumCS",UraniumData->MeshName,CommonGroup);

    RigidDebris* Uranium1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium1->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium1->SetLocation(-155,110,0);
    Uranium1->AddToWorld();
    GameApp->AddThrowable(Uranium1);
    RigidDebris* Uranium2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Uranium") );
    Uranium2->GetRigidProxy()->SetCollisionShape(UraniumCS);
    Uranium2->SetLocation(-185,110,0);
    Uranium2->AddToWorld();
    GameApp->AddThrowable(Uranium2);

    // Create the zones
    Vector3 StartSize(60,30,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);
    Physics::CollisionShape* StartAreaShape = new Physics::BoxCollisionShape("StartArea1Shape",StartSize);
    Graphics::Mesh* StartAreaMesh = Graphics::Procedural::BoxCornerGenerator(StartSize,4.0).GenerateMesh("StartAreaMesh",JusticeGroup,"Basic/Green",CommonGroup);

    StartArea* StartZone1 = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone1->SetName("StartArea1");
    StartZone1->GetGhostProxy()->SetCollisionShape(StartAreaShape);
    StartZone1->GetItemProxy()->SetMesh(StartAreaMesh);
    StartZone1->SetParticleMinimumTimeToLive(3.0);
    StartZone1->SetParticleMaximumTimeToLive(4.0);
    StartZone1->SetLocation(-160,120,0);
    StartZone1->AddToWorld();
    StartArea* StartZone2 = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone2->SetName("StartArea2");
    StartZone2->GetGhostProxy()->SetCollisionShape(StartAreaShape);
    StartZone2->GetItemProxy()->SetMesh(StartAreaMesh);
    StartZone2->SetParticleMinimumTimeToLive(3.0);
    StartZone2->SetParticleMaximumTimeToLive(4.0);
    StartZone2->SetLocation(160,120,0);
    StartZone2->AddToWorld();// */

    Vector3 ScoreSize(52,25,18);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);
    Physics::CollisionShape* ScoreAreaShape = new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize);
    Graphics::Mesh* ScoreAreaMesh = Graphics::Procedural::BoxCornerGenerator(ScoreSize,4.0).GenerateMesh("ScoreAreaMesh",JusticeGroup,"Basic/Blue",CommonGroup);

    ScoreArea* ScoreZone1 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZoneParams) );
    ScoreZone1->SetName("ScoreArea1");
    ScoreZone1->GetGhostProxy()->SetCollisionShape(ScoreAreaShape);
    ScoreZone1->GetItemProxy()->SetMesh(ScoreAreaMesh);
    ScoreZone1->SetLocation(-113,-40,0);
    ScoreZone1->AddToWorld();// */
    ScoreArea* ScoreZone2 = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZoneParams) );
    ScoreZone2->SetName("ScoreArea2");
    ScoreZone2->GetGhostProxy()->SetCollisionShape(ScoreAreaShape);
    ScoreZone2->GetItemProxy()->SetMesh(ScoreAreaMesh);
    ScoreZone2->SetLocation(113,-40,0);
    ScoreZone2->AddToWorld();// */
}

void LoadRollers()
{
    // Get our major roots
    CatchApp* GameApp = CatchApp::GetCatchAppPointer();
    World* CatchWorld = GameApp->GetTheWorld();
    // Get managers
    Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    //Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( CatchWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( CatchWorld->GetManager(ManagerBase::MT_SceneManager) );
    EntityManager* EntMan = static_cast<EntityManager*>( CatchWorld->GetManager(ManagerBase::MT_EntityManager) );

    // Init Resources
    String CommonGroup("Common");
    String RollersGroup("Rollers");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"Rollers.lvl", Resource::AT_Zip, RollersGroup, false);
    ResourceMan->InitAssetGroup(RollersGroup);

    // Scoring and Shop Setup
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Shop->SetLevelCash(100);

    // Camera Setup
    GameApp->GetPlayer()->InitWorldEntities(CatchWorld);
    GameApp->GetPlayer()->GetControl().GetCamera()->SetLocation(Vector3(0,0,425));

    // Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,-120),Vector3(15,-10,-120),Vector3(0,10,-120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.

    // Create the basic terrain that will be used
    RigidDebris* RollersFrame = EntMan->CreateRigidDebris(0);
    RollersFrame->SetName("RollersFrame");
    RollersFrame->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("FrameShape","frame.mesh",RollersGroup) );
    RollersFrame->GetRigidProxy()->SetFriction(1.0);
    RollersFrame->GetItemProxy()->SetMesh("frame.mesh",RollersGroup);
    RollersFrame->SetLocation(0,-25,0);
    RollersFrame->AddToWorld();

    // Create the individual Rollers and their constraints
    std::vector<Physics::HingeConstraint*> TheRollers;
    Real XStride = 14.5;
    Physics::CollisionShape* RollerShape = new Physics::CylinderCollisionShape("RollerShape",6.51,3.36,Vector3::Unit_Z());
    for( Whole X = 0 ; X < 7 ; ++X )
    {
        Vector3 Location(112.4+(XStride*X),-20,0);
        StringStream Namestream;
        Namestream << "Roller" << (X+1);
        RigidDebris* Roller = EntMan->CreateRigidDebris(10);
        Roller->SetName(Namestream.str());
        Roller->GetRigidProxy()->SetCollisionShape(RollerShape);
        Roller->GetRigidProxy()->SetFriction(1.0);
        Roller->GetItemProxy()->SetMesh("rubberroller.mesh",RollersGroup);
        Roller->SetLocation(Location);
        Roller->AddToWorld();

        Physics::HingeConstraint* RollerAnchor = PhysMan->CreateHingeConstraint(Roller->GetRigidProxy(),Vector3(0,0,0),Vector3(0,0,1));
        RollerAnchor->SetLimits(1.0,-1.0);
        RollerAnchor->SetMaxMotorImpulse(1500.0);
        RollerAnchor->SetMotorEnabled(true);
        RollerAnchor->EnableConstraint(true);

        TheRollers.push_back(RollerAnchor);
    }

    Roll_Roll* RollerTrigger = new Roll_Roll("RollerTrigger",TheRollers);
    PhysMan->AddWorldTrigger(RollerTrigger);

    // Create some throwable objects
    ThrowableData* PyriteData = ThrowableGenerator::GetThrowableData("Pyrite");
    ThrowableData* LeadData = ThrowableGenerator::GetThrowableData("Lead");
    Physics::CollisionShape* PyriteCS = CShapeMan->GenerateConvexHull("PyriteCS",PyriteData->MeshName,CommonGroup);
    Physics::CollisionShape* LeadCS = CShapeMan->GenerateConvexHull("LesdCS",LeadData->MeshName,CommonGroup);

    RigidDebris* Pyrite1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite1->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite1->SetLocation(-160,80,0);
    Pyrite1->AddToWorld();
    GameApp->AddThrowable(Pyrite1);
    RigidDebris* Pyrite2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Pyrite") );
    Pyrite2->GetRigidProxy()->SetCollisionShape(PyriteCS);
    Pyrite2->SetLocation(-120,80,0);
    Pyrite2->AddToWorld();
    GameApp->AddThrowable(Pyrite2);// */
    RigidDebris* Lead1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead1->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead1->SetLocation(-160,30,0);
    Lead1->AddToWorld();
    GameApp->AddThrowable(Lead1);
    RigidDebris* Lead2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Lead") );
    Lead2->GetRigidProxy()->SetCollisionShape(LeadCS);
    Lead2->SetLocation(-120,30,0);
    Lead2->AddToWorld();
    GameApp->AddThrowable(Lead2);// */

    // Create the zones
    Vector3 StartSize(50,50,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone->SetName("ScoreArea1");
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(StartSize,4.0).GenerateMesh("StartAreaMesh",RollersGroup,"Basic/Green",CommonGroup) );
    StartZone->SetParticleMinimumTimeToLive(5.0);
    StartZone->SetParticleMaximumTimeToLive(6.0);
    StartZone->SetLocation(-140,60,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(70,40,20);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZoneParams) );
    ScoreZone->SetName("ScoreArea1");
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(ScoreSize,4.0).GenerateMesh("ScoreAreaMesh",RollersGroup,"Basic/Blue",CommonGroup) );
    ScoreZone->SetLocation(140,20,0);
    ScoreZone->AddToWorld();// */
}

void LoadJustBounce()
{
    // Get our major roots
    CatchApp* GameApp = CatchApp::GetCatchAppPointer();
    World* CatchWorld = GameApp->GetTheWorld();
    // Get managers
    Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    //Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( CatchWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( CatchWorld->GetManager(ManagerBase::MT_SceneManager) );
    EntityManager* EntMan = static_cast<EntityManager*>( CatchWorld->GetManager(ManagerBase::MT_EntityManager) );

    // Init Resources
    String CommonGroup("Common");
    String JustBounceGroup("JustBounce");
    String datadir = "Levels/";
    ResourceMan->AddAssetLocation(datadir+"JustBounce.lvl", Resource::AT_Zip, JustBounceGroup, false);
    ResourceMan->InitAssetGroup(JustBounceGroup);

    // Scoring and Shop Setup
    ItemShop* Shop = CatchApp::GetCatchAppPointer()->GetItemShop();
    Shop->SetLevelCash(100);

    // Camera Setup
    GameApp->GetPlayer()->InitWorldEntities(CatchWorld);
    GameApp->GetPlayer()->GetControl().GetCamera()->SetLocation(Vector3(0,0,425));

    // Lights Setup
    //SceneMan->SetAmbientLight(1.0,1.0,1.0,1.0);
    Graphics::LightProxy* DLight = SceneMan->CreateLightProxy(Graphics::LT_Directional);
    Vector3 Loc(-150,100,200);
    DLight->SetLocation(Loc);
    Loc.Normalize();
    DLight->SetDirection(Vector3(-Loc.X,-Loc.Y,-Loc.Z));
    DLight->SetDiffuseColour(ColourValue(0.3,0.3,0.3,1));
    DLight->SetSpecularColour(ColourValue(0.3,0.3,0.3,1));

    // Physics Setup
    PhysMan->SetWorldGravity(Vector3(0,-1000,0));

    // Assuming all mass amounts are in metric kg.
    // Assuming all distances are in metric cm.

    // Create the background
    Plane SkyPlane(Vector3(-15,-10,-120),Vector3(15,-10,-120),Vector3(0,10,-120));
    SceneMan->CreateSkyPlane(SkyPlane, "Backgrounds/Grassy", CommonGroup, 3, 3);

    // Setup and Create the shapes that will be used.
    CShapeMan->LoadAllShapesFromBinaryFile("JustBounce.bullet",JustBounceGroup);

    //----------
    /*std::set<CollisionShape*>& Unnamed = CShapeMan->GetUnnamedShapes();
    for( std::set<CollisionShape*>::iterator CSit = Unnamed.begin() ; CSit != Unnamed.end() ; CSit++ )
    {
        CollisionShape* ToChange = (*CSit);
        if(CollisionShape::ST_Compound==ToChange->GetType())
        {
            if(7 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("Basket",ToChange);
            else if(2 == ((CompoundCollisionShape*)ToChange)->GetNumChildren())
                CShapeMan->SetNameForUnnamedShape("ElasticBed",ToChange);
        }
    }
    Unnamed.clear();
    CShapeMan->SaveAllStoredShapesToFile("JustBounce.bullet");// */
    //----------

    // Create the basic terrain that will be used
    RigidDebris* TopWall = EntMan->CreateRigidDebris(0);
    TopWall->SetName("TopWall");
    TopWall->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("WallShape","wall.mesh",JustBounceGroup) );
    TopWall->GetRigidProxy()->SetRestitution(0.5);
    TopWall->GetItemProxy()->SetMesh("wall.mesh",JustBounceGroup);
    TopWall->SetLocation(0,85,0);
    TopWall->AddToWorld();

    RigidDebris* Basket = EntMan->CreateRigidDebris(0);
    Basket->SetName("Basket");
    Basket->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("Basket") );
    Basket->GetItemProxy()->SetMesh("basket.mesh",JustBounceGroup);
    Basket->SetLocation(165,-25,0);
    Basket->AddToWorld();

    RigidDebris* ElasticFloor = EntMan->CreateRigidDebris(0);
    ElasticFloor->SetName("ElasticFloor");
    ElasticFloor->GetRigidProxy()->SetCollisionShape( CShapeMan->GetShape("ElasticBed") );// ©ShapeMan->GenerateStaticTriMesh("ElasticShape","elastic_bed.mesh",JustBounceGroup));
    ElasticFloor->GetRigidProxy()->SetRestitution(1.0);
    //ElasticFloor->GetRigidProxy()->SetFriction(0.4);
    ElasticFloor->GetItemProxy()->SetMesh("elastic_bed.mesh",JustBounceGroup);
    ElasticFloor->SetLocation(-20,-70,0);
    ElasticFloor->AddToWorld();

    RigidDebris* ElasticCase = EntMan->CreateRigidDebris(0);
    ElasticCase->SetName("ElasticCase");
    ElasticCase->GetRigidProxy()->SetCollisionShape( CShapeMan->GenerateStaticTriMesh("CaseShape","case.mesh",JustBounceGroup) );
    ElasticCase->GetItemProxy()->SetMesh("case.mesh",JustBounceGroup);
    ElasticCase->SetLocation(-20,-71,0);
    ElasticCase->AddToWorld();

    // Create some throwable objects
    //ThrowableData* RubberData = ThrowableGenerator::GetThrowableData("Rubber");
    ThrowableData* WoodData = ThrowableGenerator::GetThrowableData("Wood");
    Physics::CollisionShape* RubberCS = new Physics::SphereCollisionShape("RubberCS",11.4);// CShapeMan->GenerateConvexHull("RubberCS",RubberData->MeshName,CommonGroup);
    Physics::CollisionShape* WoodCS = CShapeMan->GenerateConvexHull("WoodCS",WoodData->MeshName,CommonGroup);// */

    RigidDebris* Rubber1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber1->GetRigidProxy()->SetCollisionShape(RubberCS);
    Rubber1->SetLocation(-185,110,0);
    Rubber1->AddToWorld();
    GameApp->AddThrowable(Rubber1);
    RigidDebris* Rubber2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Rubber") );
    Rubber2->GetRigidProxy()->SetCollisionShape(RubberCS);
    Rubber2->SetLocation(-155,110,0);
    Rubber2->AddToWorld();
    GameApp->AddThrowable(Rubber2);// */
    RigidDebris* Wood1 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Wood") );
    Wood1->GetRigidProxy()->SetCollisionShape(WoodCS);
    Wood1->SetLocation(-185,70,0);
    Wood1->AddToWorld();
    GameApp->AddThrowable(Wood1);
    RigidDebris* Wood2 = static_cast<RigidDebris*>( ThrowableGenerator::CreateThrowable("Wood") );
    Wood2->GetRigidProxy()->SetCollisionShape(WoodCS);
    Wood2->SetLocation(-155,70,0);
    Wood2->AddToWorld();
    GameApp->AddThrowable(Wood2);// */


    // Create the zones
    Vector3 StartSize(50,50,20);
    NameValuePairMap StartZoneParams;
    StartZoneParams["SizeX"] = StringTools::ConvertToString(StartSize.X);
    StartZoneParams["SizeY"] = StringTools::ConvertToString(StartSize.Y);
    StartZoneParams["SizeZ"] = StringTools::ConvertToString(StartSize.Z);

    StartArea* StartZone = static_cast<StartArea*>( EntMan->CreateEntity("StartArea",StartZoneParams) );
    StartZone->SetName("StartArea1");
    StartZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("StartAreaShape",StartSize) );
    StartZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(StartSize,4.0).GenerateMesh("StartAreaMesh",JustBounceGroup,"Basic/Green",CommonGroup) );
    StartZone->SetParticleMinimumTimeToLive(5.0);
    StartZone->SetParticleMaximumTimeToLive(6.0);
    StartZone->SetLocation(-170,90,0);
    StartZone->AddToWorld();// */

    Vector3 ScoreSize(42,50,35);
    NameValuePairMap ScoreZoneParams;
    ScoreZoneParams["SizeX"] = StringTools::ConvertToString(ScoreSize.X);
    ScoreZoneParams["SizeY"] = StringTools::ConvertToString(ScoreSize.Y);
    ScoreZoneParams["SizeZ"] = StringTools::ConvertToString(ScoreSize.Z);

    ScoreArea* ScoreZone = static_cast<ScoreArea*>( EntMan->CreateEntity("ScoreArea",ScoreZoneParams) );
    ScoreZone->SetName("ScoreArea1");
    ScoreZone->GetGhostProxy()->SetCollisionShape( new Physics::BoxCollisionShape("ScoreAreaShape",ScoreSize) );
    ScoreZone->GetItemProxy()->SetMesh( Graphics::Procedural::BoxCornerGenerator(ScoreSize,4.0).GenerateMesh("ScoreAreaMesh",JustBounceGroup,"Basic/Blue",CommonGroup) );
    ScoreZone->SetLocation(158,-25,0);
    ScoreZone->AddToWorld();// */
}

///////////////////////////////////////////////////////////////////////////////
// Anonymous Namespace Utilities

namespace
{
    /// @brief A mutex used by the level loader to set the next level.
    Threading::Mutex NextLevelMutex;
}

///////////////////////////////////////////////////////////////////////////////
// CatchLevel Methods

CatchLevel::CatchLevel(const String& LvlName) :
    LevelName(LvlName),
    LevelTargetTime(0)
    {  }

CatchLevel::CatchLevel(const String& Group, const XML::Document& LevelDoc) :
    GroupName(Group),
    LevelTargetTime(0)
    { this->DeSerializeLevelData(LevelDoc); }

CatchLevel::~CatchLevel()
    {  }

void CatchLevel::DeSerializeLevelData(const XML::Document& LevelDoc)
{
    XML::Attribute CurrAttrib;

    XML::Node RootNode = LevelDoc.GetChild("LevelRoot");
    if( !RootNode.Empty() ) {
        // First get the basic level properties
        XML::Node LevelPropertiesNode = RootNode.GetChild("LevelProperties");
        if( !LevelPropertiesNode.Empty() ) {
            CurrAttrib = LevelPropertiesNode.GetAttribute("Name");
            if( !CurrAttrib.Empty() )
                this->LevelName = CurrAttrib.AsString();

            CurrAttrib = LevelPropertiesNode.GetAttribute("TargetTime");
            if( !CurrAttrib.Empty() )
                this->LevelTargetTime = CurrAttrib.AsWhole();
        }

        // Find and assign the preview data
        XML::Node PreviewNode = RootNode.GetChild("Preview");
        if( !PreviewNode.Empty() ) {
            CurrAttrib = PreviewNode.GetAttribute("Atlas");
            if( !CurrAttrib.Empty() ) {
                this->PreviewAtlasName = CurrAttrib.AsString();
            }

            CurrAttrib = PreviewNode.GetAttribute("Image");
            if( !CurrAttrib.Empty() ) {
                this->PreviewImageName = CurrAttrib.AsString();
            }
        }

        // Get the score tier data
        XML::Node ScoreTiersNode = RootNode.GetChild("ScoreTiers");
        if( !ScoreTiersNode.Empty() ) {
            for( XML::NodeIterator ScoreTierIt = ScoreTiersNode.begin() ; ScoreTierIt != ScoreTiersNode.end() ; ++ScoreTierIt )
            {
                Whole TierValue = 0;
                Whole Threshold = 0;

                CurrAttrib = (*ScoreTierIt).GetAttribute("TierValue");
                if( !CurrAttrib.Empty() ) {
                    TierValue = CurrAttrib.AsWhole();
                }

                CurrAttrib = (*ScoreTierIt).GetAttribute("Threshold");
                if( !CurrAttrib.Empty() ) {
                    Threshold = CurrAttrib.AsWhole();
                }

                if( TierValue > 0 && Threshold > 0 ) {
                    if( TierValue > this->ScoreTiers.size() ) {
                        this->ScoreTiers.resize(TierValue);
                    }
                    this->ScoreTiers.at(TierValue - 1) = Threshold;
                }
            }
        }

        Resource::ResourceManager* ResourceMan = Resource::ResourceManager::GetSingletonPtr();
        //CatchApp* CatchAppPtr =  CatchApp::GetCatchAppPointer();
        Scripting::Lua::Lua51WorkUnit* Work = CatchApp::GetCatchAppPointer()->GetLuaScriptWork();
        // Get the scripts that will be run while this level is loaded
        XML::Node LevelScriptsNode = RootNode.GetChild("LevelScripts");
        if( !LevelScriptsNode.Empty() ) {
            for( XML::NodeIterator ScriptNodeIt = LevelScriptsNode.begin() ; ScriptNodeIt != LevelScriptsNode.end() ; ++ScriptNodeIt )
            {
                String ScriptSource;
                // Prioritize in file script code
                CurrAttrib = (*ScriptNodeIt).GetAttribute("Source");
                if( !CurrAttrib.Empty() ) {
                    ScriptSource = CurrAttrib.AsString();
                }

                if( ScriptSource.empty() ) {
                    // Next attempt to load the specified file, if one is specified
                    CurrAttrib = (*ScriptNodeIt).GetAttribute("FileName");
                    if( !CurrAttrib.Empty() ) {
                        DataStreamPtr ScriptStream = ResourceMan->OpenAssetStream( CurrAttrib.AsString(), this->LevelName );
                        ScriptSource = ScriptStream->GetAsString();
                    }
                }

                Work->AddScript(ScriptSource);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Utility

const String& CatchLevel::GetName() const
    { return this->LevelName; }

const String& CatchLevel::GetGroupName() const
    { return this->GroupName; }

const String& CatchLevel::GetPreviewAtlasName() const
    { return this->PreviewAtlasName; }

const String& CatchLevel::GetPreviewImageName() const
    { return this->PreviewImageName; }

///////////////////////////////////////////////////////////////////////////////
// MetaData Access

Whole CatchLevel::GetLevelTargetTime() const
    { return this->LevelTargetTime; }

Whole CatchLevel::GetScoreThreshold(const Whole Tier) const
    { return this->ScoreTiers.at(Tier); }

Whole CatchLevel::GetNumScoreTiers() const
    { return this->ScoreTiers.size(); }

CatchLevel::ScriptPtr CatchLevel::GetLevelScript(const Whole Index) const
    { return this->LevelScripts.at(Index); }

Whole CatchLevel::GetNumLevelScripts() const
    { return this->LevelScripts.size(); }

///////////////////////////////////////////////////////////////////////////////
// LevelLoader Methods

LevelManager::LevelManager(Entresol* Ent, const String& PathToLevels) :
    LevelPath(PathToLevels),
    TheEntresol(Ent),
    LevelToLoad(NULL),
    CurrentLevel(NULL)
    { this->GameLevels.push_back( new GameLevel("MainMenu") ); }

LevelManager::~LevelManager()
    {  }

Resource::ResourceManager* LevelManager::GetResourceManager() const
    { return static_cast<Resource::ResourceManager*>( this->TheEntresol->GetManager(ManagerBase::MT_ResourceManager) ); }

///////////////////////////////////////////////////////////////////////////////
// Utility

Whole LevelManager::DetectLevels()
{
    Whole Detected = 0;
    XML::Document LevelDoc;
    Resource::ResourceManager* ResourceMan = this->GetResourceManager();
    StringVector LevelSet = Resource::GetDirContents( this->LevelPath );
    for( StringVector::iterator LvlIt = LevelSet.begin() ; LvlIt != LevelSet.end() ; LvlIt++ )
    {
        const String& FileName = (*LvlIt);
        if( String::npos != FileName.find(".lvl") ) {
            ++Detected;

            const String AssetGroupName = FileName.substr(0,FileName.find_last_of('.'));
            const String CompletePath = this->LevelPath + FileName;
            ResourceMan->AddAssetLocation(CompletePath,Resource::AT_Zip,AssetGroupName);

            LevelDoc.Reset();
            DataStreamPtr LevelStream = ResourceMan->OpenAssetStream("Level.xml",AssetGroupName);
            LevelDoc.Load( *LevelStream.Get() );

            GameLevel* NewLevel = new GameLevel(AssetGroupName,LevelDoc);
            this->GameLevels.push_back(NewLevel);
        }
    }
    return Detected;
}

void LevelManager::PopulateLevelSelectUI()
{
    const String LevelNameFont = "Ubuntu-18";
    const Real LevelNameScale = 0.85;

    UI::UIManager* UIMan = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) );
    UI::Screen* MainMenuScreen = UIMan->GetScreen("MainMenuScreen");
    UI::GridContainer* LevelSelect = static_cast<UI::GridContainer*>( MainMenuScreen->GetWidget("MS_LevelSelectGrid") );

    Whole LevelZOrder = 0;
    Whole CurrColumn = 0, CurrRow = 0;
    LevelSelect->DestroyAllChildren();

    for( GameLevelIterator LvlIt = this->GameLevels.begin() ; LvlIt != this->GameLevels.end() ; ++LvlIt )
    {
        // Setup our names
        const String& LevelName = (*LvlIt)->GetName();
        const String& GroupName = (*LvlIt)->GetGroupName();
        const String& PreviewAtlasName = (*LvlIt)->GetPreviewAtlasName();
        const String& PreviewImageName = (*LvlIt)->GetPreviewImageName();
        // Early escape for invalid level
        if( LevelName == "MainMenu" )
            continue;

        // We want a 2 wide coloumn straight down
        if( CurrColumn > 1 ) {
            CurrColumn = 0;
            ++CurrRow;
        }

        // Verify our atlas is loaded, safely
        UI::TextureAtlas* LevelAtlas = UIMan->GetAtlas(PreviewAtlasName);
        if( LevelAtlas == NULL ) {
            UIMan->LoadMTA(PreviewAtlasName,GroupName);
        }

        // Create the root widget that will glue all the other pieces together
        UI::Widget* LevelRoot = MainMenuScreen->CreateWidget( LevelName );
        LevelRoot->CreateSingleImageLayer("MMLevelCellBack",0,0);
        UI::SingleImageLayer* SelectedLevelBack = LevelRoot->CreateSingleImageLayer("MMLevelCellBackSelected");
        // Now we need to setup our RenderLayerGroup bindings for being selected
        UI::RenderLayerGroup* SelectedNormalGroup = LevelRoot->CreateRenderLayerGroup(UI::CheckBox::WG_SelectedNormal);
        UI::RenderLayerGroup* SelectedHoveredGroup = LevelRoot->CreateRenderLayerGroup(UI::CheckBox::WG_SelectedHovered);
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1, SelectedNormalGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Hovered, SelectedHoveredGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Focused, SelectedNormalGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Dragged, SelectedNormalGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Hovered | UI::Widget::WS_Focused, SelectedHoveredGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Focused | UI::Widget::WS_Dragged, SelectedNormalGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Dragged | UI::Widget::WS_Hovered, SelectedHoveredGroup );
        LevelRoot->BindGroupToState( UI::Widget::WS_User_State_1 | UI::Widget::WS_Hovered | UI::Widget::WS_Focused | UI::Widget::WS_Dragged, SelectedHoveredGroup );
        // Now that bindings work is done, add our selected sprite to the configuration
        SelectedNormalGroup->AddLayer(SelectedLevelBack,1);
        SelectedHoveredGroup->AddLayer(SelectedLevelBack,1);

        // Create the preview section for this level widget
        UI::Widget* LevelPortrait = MainMenuScreen->CreateWidget( LevelName + ".Portrait", UI::UnifiedRect(-0.2,0.0,1.3333,1.3) );
        LevelPortrait->SetHorizontalPositioningRules(UI::PF_Anchor_Left | UI::PF_Anchor_SelfSize);
        LevelPortrait->SetVerticalPositioningRules(UI::PF_Anchor_VerticalCenter);
        LevelPortrait->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
        UI::SingleImageLayer* LevelImage = LevelPortrait->CreateSingleImageLayer(0,0);
        LevelImage->SetSprite( PreviewImageName, PreviewAtlasName.substr( 0, PreviewAtlasName.find_last_of('.') ) );
        LevelImage->SetScale(Vector2(0.9,0.9));
        LevelPortrait->CreateSingleImageLayer("MMLevelPreviewBox",1,1);
        LevelRoot->AddChild(LevelPortrait,1);

        // Create the widget that will place and render the level name text
        UI::Widget* LevelTitle = MainMenuScreen->CreateWidget( LevelName + ".Title", UI::UnifiedRect(0.4,0.06,0.50,0.4) );
        UI::SingleLineTextLayer* LevelTitleText = LevelTitle->CreateSingleLineTextLayer(LevelNameFont,0,0);
        LevelTitleText->SetText(LevelName);
        LevelTitleText->HorizontallyAlign(UI::LA_Center);
        LevelTitleText->VerticallyAlign(UI::LA_Center);
        LevelTitleText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,LevelNameScale);
        LevelRoot->AddChild(LevelTitle,2);

        const Whole NumStars = 5;
        // Create the score star display that will indicate the degree of completion for a level
        UI::Widget* LevelScore = MainMenuScreen->CreateWidget( LevelName + ".Score", UI::UnifiedRect(0.4,0.48,0.50,0.38) );
        UI::MultiImageLayer* ScoreStars = LevelScore->CreateMultiImageLayer(0,0);
        ScoreStars->ReserveMultiImageData(NumStars);
        // Set up our multiple stars for displaying the score threshold achieved
        for( Whole X = 0 ; X < NumStars ; ++X )
        {
            UI::UnifiedVec2 UPosition(0.2 * static_cast<Real>(X),0.0);
            UI::UnifiedVec2 USize(0.2,1.0);

            ScoreStars->SetImagePosition(X,UPosition);
            ScoreStars->SetImageSize(X,USize);
            ScoreStars->SetSprite(X,"MMLevelScoreStar");
        }
        LevelRoot->AddChild(LevelScore,3);

        // Insert this level into the grid
        UI::GridRect LevelGridPos(CurrColumn,CurrRow,1,1);
        LevelSelect->AddChild(LevelRoot,LevelZOrder,LevelGridPos);

        // Increment our data for the next one
        ++LevelZOrder;
        ++CurrColumn;
    }
}

void LevelManager::SetNextLevel(const String& LevelName)
{
    GameLevel* Next = this->GetLevel(LevelName);
    if( Next != NULL ) {
        this->SetNextLevel( Next );
    }
}

void LevelManager::SetNextLevel(GameLevel* NextLevel)
{
    Threading::lock_guard<Threading::Mutex> Lock(NextLevelMutex);
    this->LevelToLoad = NextLevel;
}

GameLevel* LevelManager::GetNextLevel() const
    { return this->LevelToLoad; }

GameLevel* LevelManager::GetCurrentLevel() const
    { return this->CurrentLevel; }

Boole LevelManager::HasALevelToLoad() const
    { return ( this->LevelToLoad != NULL ); }

void LevelManager::SetLevelPath(const String& PathToLevels)
    { this->LevelPath = PathToLevels; }

const String& LevelManager::GetLevelPath() const
    { return this->LevelPath; }

///////////////////////////////////////////////////////////////////////////////
// Game Level Access

GameLevel* LevelManager::GetLevel(const Whole Index)
{
    return this->GameLevels.at(Index);
}

GameLevel* LevelManager::GetLevel(const String& LevelName)
{
    for( GameLevelIterator LvlIt = this->GameLevels.begin() ; LvlIt != this->GameLevels.end() ; ++LvlIt )
    {
        if( (*LvlIt)->GetName() == LevelName ) {
            return (*LvlIt);
        }
    }
    return NULL;
}

Whole LevelManager::GetNumLevels() const
{
    return this->GameLevels.size();
}

LevelManager::GameLevelIterator LevelManager::BeginGameLevel()
    { return this->GameLevels.begin(); }

LevelManager::GameLevelIterator LevelManager::EndGameLevel()
    { return this->GameLevels.end(); }

LevelManager::ConstGameLevelIterator LevelManager::BeginGameLevel() const
    { return this->GameLevels.begin(); }

LevelManager::ConstGameLevelIterator LevelManager::EndGameLevel() const
    { return this->GameLevels.end(); }


///////////////////////////////////////////////////////////////////////////////
// Loading and Unloading

void LevelManager::LoadNextLevel()
{
    /// @todo The if(Level) statements need to removed in favor of something that will parse an XML file of the level.
    if( "Ferris" == this->LevelToLoad->GetName() )
        LoadFerris();
    else if( "BigCurve" == this->LevelToLoad->GetName() )
        LoadBigCurve();
    else if( "BlowsNotSucks" == this->LevelToLoad->GetName() )
        LoadBlowsNotSucks();
    else if( "Justice" == this->LevelToLoad->GetName() )
        LoadJustice();
    else if( "Rollers" == this->LevelToLoad->GetName() )
        LoadRollers();
    else if( "JustBounce" == this->LevelToLoad->GetName() )
        LoadJustBounce();

    this->CurrentLevel = this->LevelToLoad;
    this->LevelToLoad = NULL;
}

void LevelManager::UnloadLevel()
{
    // World Cleanup
    this->TheEntresol->GetWorld(0)->Clear();

    // Entresol Cleanup
    /// @todo This should be populated with the appropriate logic after the engine state refactors are done.
    Physics::CollisionShapeManager* CShapeMan = static_cast<Physics::CollisionShapeManager*>( this->TheEntresol->GetManager(ManagerBase::MT_CollisionShapeManager) );
    Graphics::MeshManager* MeshMan = static_cast<Graphics::MeshManager*>( this->TheEntresol->GetManager(ManagerBase::MT_MeshManager) );

    CShapeMan->DestroyAllShapes();
    MeshMan->RemoveAllMeshes();
}

#endif
