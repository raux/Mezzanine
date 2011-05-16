//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _scenemanagertests_h
#define _scenemanagertests_h

#include "main.h"

class SceneManagerTests : public UnitTest
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            SceneManager* SM = 0;

            if (RunInteractiveTests)
            {

                AddTestResult("SceneManager::DisplaySkyPlane", Unknown);
                try
                {
                    StartEngine();
                    TheMessage="Does It Look Like a SkyPlane?";
                    SM = TheWorld->GetSceneManager();
                    SM->SetAmbientLight(1,1,1,1);
                    //SM->CreateSkyPlane(Plane(Vector3(0,1,0),5),"Examples/CloudPlane","files");
                    SM->CreateSkyPlane(Plane(Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0)),"Examples/CloudPlane","files");
                    GetAnswer();
                    TheWorld->MainLoop();
                    StopEngine();
                    AddTestResult("SceneManager::DisplaySkyPlane", AnswerToQuestion, UnitTest::OverWrite);
                } catch (std::exception e) {
                    cout << e.what();
                    AddTestResult("SceneManager::DisplaySkyPlane", Failed, UnitTest::OverWrite);
                }

                try
                {
                    StartEngine();
                    AddTestResult("SceneManager::DisplaySkyBox", Unknown);
                    TheMessage="Does It Look Like a SkyBox?";
                    SM = TheWorld->GetSceneManager();
                    SM->SetAmbientLight(1,1,1,1);
                    SM->CreateSkyBox("Examples/CloudBox","files",50);
                    GetAnswer();
                    TheWorld->MainLoop();
                    StopEngine();
                    AddTestResult("SceneManager::DisplaySkyBox", AnswerToQuestion, UnitTest::OverWrite);
                } catch (std::exception e) {
                    cout << e.what();
                    AddTestResult("SceneManager::DisplaySkyBox", Failed, UnitTest::OverWrite);
                }

                try
                {
                    StartEngine();
                    AddTestResult("SceneManager::DisplaySkyDome", Unknown);
                    TheMessage="Does It Look Like a SkyDome?";
                    SM = TheWorld->GetSceneManager();
                    SM->SetAmbientLight(1,1,1,1);
                    SM->CreateSkyDome("Examples/CloudPlane","files",50);
                    GetAnswer();
                    TheWorld->MainLoop();
                    StopEngine();
                    AddTestResult("SceneManager::DisplaySkyDome", AnswerToQuestion, UnitTest::OverWrite);
                } catch (std::exception e) {
                    cout << e.what();
                    AddTestResult("SceneManager::DisplaySkyDome", Failed, UnitTest::OverWrite);
                }


            }else{
                AddTestResult("SceneManager::DisplaySkyPlane", Skipped);
                AddTestResult("SceneManager::DisplaySkyBox", Skipped);
                AddTestResult("SceneManager::DisplaySkyDome", Skipped);
            }

            if (RunAutomaticTests)
            {
                StartEngine();
                AddTestResult("SceneManager::operator<< (No Sky)", Unknown);
                SceneManager* SM = TheWorld->GetSceneManager();
                stringstream SerializeTest1("");
                SM->SetAmbientLight(1,1,1,1);
                SM->CreateLight("ADefaultLightWithNoChangesAtAll");
                SM->CreateParticleEffect("SomeGreenParticles", "Examples/GreenyNimbus");
                SM->CreateFreeNode("ASimpleFreeNode",phys::Vector3(1,1,1),phys::Vector3(2,2,2));
                SerializeTest1 << *SM;
                if ("<SceneManager Version=\"1\" Name=\"SceneManagerInstance1\" SceneShadowTechnique=\"0\" ShadowTextureCount=\"1\" ShadowTextureSize=\"512\" ShadowFarDistance=\"0\" SkyMethod=\"0\"><ShadowColour><ColourValue Version=\"1\" Red=\"0.25\" Green=\"0.25\" Blue=\"0.25\" Alpha=\"1\" /></ShadowColour><AmbientLight><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></AmbientLight><Light Version=\"1\" Name=\"ADefaultLightWithNoChangesAtAll\" AttachedTo=\"\" Type=\"1\" PowerScale=\"1\" AttenuationRange=\"100000\" AttenuationConstant=\"1\" AttenuationQuadric=\"0\" AttenuationLinear=\"0\" SpotlightInnerAngle=\"0.523599\" SpotlightOuterAngle=\"0.698132\" SpotlightFalloff=\"1\"><Direction><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\"/></Direction><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location><SpecularColour><ColourValue Version=\"1\" Red=\"0\" Green=\"0\" Blue=\"0\" Alpha=\"1\" /></SpecularColour><fDiffuseColour><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></fDiffuseColour></Light><ParticleEffect Version=\"1\" Name=\"SomeGreenParticles\" AttachedTo=\"\" Enabled=\"0\" Template=\"\"><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" W=\"1\" /></Orientation><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location></ParticleEffect><WorldNode Version=\"1\" Name=\"ASimpleFreeNode\" AttachedTo=\"\" StandingType=\"0\"><Orientation><Quaternion Version=\"1\" X=\"-0.325058\" Y=\"0.325058\" Z=\"0\" W=\"0.888074\" /></Orientation><Location><Vector3 Version=\"1\" X=\"2\" Y=\"2\" Z=\"2\"/></Location></WorldNode></SceneManager>" == SerializeTest1.str())
                {
                    AddTestResult("SceneManager::operator<< (No Sky)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator<< (No Sky)", Failed, UnitTest::OverWrite);
                }
                StopEngine();
                StartEngine();
                AddTestResult("SceneManager::operator>> (No Sky)", Unknown);
                SM = TheWorld->GetSceneManager();
                try
                {
                    SerializeTest1 >> *SM;
                    stringstream SerializeTest1a("");
                    SerializeTest1a << *SM;
                    if ( SerializeTest1a.str() == SerializeTest1.str())
                    {
                        AddTestResult("SceneManager::operator>> (No Sky)", Success, UnitTest::OverWrite);
                    }else{
                        AddTestResult("SceneManager::operator>> (No Sky)", Failed, UnitTest::OverWrite);
                    }
                }catch(Exception E){
                    AddTestResult("SceneManager::operator>> (No Sky)", Failed, UnitTest::OverWrite);
                    TheWorld->LogStream << endl << "SceneManager::operator>> (No Sky) - Failed With: " << E.what();
                    TheWorld->CommitLog();
                }

                StopEngine();


                StartEngine();
                AddTestResult("SceneManager::operator<< (SkyDome)", Unknown);
                SM = TheWorld->GetSceneManager();
                stringstream SerializeTest2("");
                SM->SetAmbientLight(1,1,1,1);
                SM->CreateSkyDome("Examples/CloudPlane","files",50);
                SM->CreateLight("ADefaultLightWithNoChangesAtAll");
                SM->CreateParticleEffect("SomeGreenParticles", "Examples/GreenyNimbus");
                SM->CreateFreeNode("ASimpleFreeNode",phys::Vector3(1,1,1),phys::Vector3(2,2,2));
                SerializeTest2 << *SM;
                if ("<SceneManager Version=\"1\" Name=\"SceneManagerInstance1\" SceneShadowTechnique=\"0\" ShadowTextureCount=\"1\" ShadowTextureSize=\"512\" ShadowFarDistance=\"0\" SkyMethod=\"3\"><ShadowColour><ColourValue Version=\"1\" Red=\"0.25\" Green=\"0.25\" Blue=\"0.25\" Alpha=\"1\" /></ShadowColour><AmbientLight><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></AmbientLight><SkyDome Version=\"1\" MaterialName=\"Examples/CloudPlane\" MaterialGroupName=\"files\" DrawFirst=\"1\" Distance=\"50\" Curvature=\"10\" Tiling=\"8\" XSegments=\"16\" YSegments=\"16\" YSegments_keep=\"-1\" ><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" W=\"1\" /></Orientation></SkyDome><Light Version=\"1\" Name=\"ADefaultLightWithNoChangesAtAll\" AttachedTo=\"\" Type=\"1\" PowerScale=\"1\" AttenuationRange=\"100000\" AttenuationConstant=\"1\" AttenuationQuadric=\"0\" AttenuationLinear=\"0\" SpotlightInnerAngle=\"0.523599\" SpotlightOuterAngle=\"0.698132\" SpotlightFalloff=\"1\"><Direction><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\"/></Direction><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location><SpecularColour><ColourValue Version=\"1\" Red=\"0\" Green=\"0\" Blue=\"0\" Alpha=\"1\" /></SpecularColour><fDiffuseColour><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></fDiffuseColour></Light><ParticleEffect Version=\"1\" Name=\"SomeGreenParticles\" AttachedTo=\"\" Enabled=\"0\" Template=\"\"><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" W=\"1\" /></Orientation><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location></ParticleEffect><WorldNode Version=\"1\" Name=\"ASimpleFreeNode\" AttachedTo=\"\" StandingType=\"0\"><Orientation><Quaternion Version=\"1\" X=\"-0.325058\" Y=\"0.325058\" Z=\"0\" W=\"0.888074\" /></Orientation><Location><Vector3 Version=\"1\" X=\"2\" Y=\"2\" Z=\"2\"/></Location></WorldNode></SceneManager>" == SerializeTest2.str())
                {
                    AddTestResult("SceneManager::operator<< (SkyDome)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator<< (SkyDome)", Failed, UnitTest::OverWrite);
                }
                StopEngine();

                StartEngine();
                AddTestResult("SceneManager::operator<< (SkyPlane)", Unknown);
                SM = TheWorld->GetSceneManager();
                stringstream SerializeTest3("");
                SM->SetAmbientLight(1,1,1,1);
                SM->CreateSkyPlane(Plane(Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0)),"Examples/CloudPlane","files");
                SM->CreateLight("ADefaultLightWithNoChangesAtAll");
                SM->CreateParticleEffect("SomeGreenParticles", "Examples/GreenyNimbus");
                SM->CreateFreeNode("ASimpleFreeNode",phys::Vector3(1,1,1),phys::Vector3(2,2,2));
                SerializeTest3 << *SM;
                if ("<SceneManager Version=\"1\" Name=\"SceneManagerInstance1\" SceneShadowTechnique=\"0\" ShadowTextureCount=\"1\" ShadowTextureSize=\"512\" ShadowFarDistance=\"0\" SkyMethod=\"1\"><ShadowColour><ColourValue Version=\"1\" Red=\"0.25\" Green=\"0.25\" Blue=\"0.25\" Alpha=\"1\" /></ShadowColour><AmbientLight><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></AmbientLight><SkyPlane Version=\"1\" MaterialName=\"Examples/CloudPlane\" MaterialGroupName=\"files\" DrawFirst=\"1\" Scale=\"1000\" Tiling=\"10\" Bow=\"0\" XSegments=\"1\" YSegments=\"1\" ><Plane Version=\"1\" Distance=\"5\" ><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\"/></Plane></SkyPlane><Light Version=\"1\" Name=\"ADefaultLightWithNoChangesAtAll\" AttachedTo=\"\" Type=\"1\" PowerScale=\"1\" AttenuationRange=\"100000\" AttenuationConstant=\"1\" AttenuationQuadric=\"0\" AttenuationLinear=\"0\" SpotlightInnerAngle=\"0.523599\" SpotlightOuterAngle=\"0.698132\" SpotlightFalloff=\"1\"><Direction><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\"/></Direction><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location><SpecularColour><ColourValue Version=\"1\" Red=\"0\" Green=\"0\" Blue=\"0\" Alpha=\"1\" /></SpecularColour><fDiffuseColour><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></fDiffuseColour></Light><ParticleEffect Version=\"1\" Name=\"SomeGreenParticles\" AttachedTo=\"\" Enabled=\"0\" Template=\"\"><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" W=\"1\" /></Orientation><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location></ParticleEffect><WorldNode Version=\"1\" Name=\"ASimpleFreeNode\" AttachedTo=\"\" StandingType=\"0\"><Orientation><Quaternion Version=\"1\" X=\"-0.325058\" Y=\"0.325058\" Z=\"0\" W=\"0.888074\" /></Orientation><Location><Vector3 Version=\"1\" X=\"2\" Y=\"2\" Z=\"2\"/></Location></WorldNode></SceneManager>" == SerializeTest3.str())
                {
                    AddTestResult("SceneManager::operator<< (SkyPlane)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator<< (SkyPlane)", Failed, UnitTest::OverWrite);
                }
                StopEngine();

                StartEngine();
                AddTestResult("SceneManager::operator<< (SkyBox)", Unknown);
                SM = TheWorld->GetSceneManager();
                stringstream SerializeTest4("");
                SM->SetAmbientLight(1,1,1,1);
                SM->CreateSkyBox("Examples/CloudBox","files",50);
                SM->CreateLight("ADefaultLightWithNoChangesAtAll");
                SM->CreateParticleEffect("SomeGreenParticles", "Examples/GreenyNimbus");
                SM->CreateFreeNode("ASimpleFreeNode",phys::Vector3(1,1,1),phys::Vector3(2,2,2));
                SerializeTest4 << *SM;
                TheWorld->LogStream << *SM;
                TheWorld->CommitLog();
                if ("<SceneManager Version=\"1\" Name=\"SceneManagerInstance1\" SceneShadowTechnique=\"0\" ShadowTextureCount=\"1\" ShadowTextureSize=\"512\" ShadowFarDistance=\"0\" SkyMethod=\"2\"><ShadowColour><ColourValue Version=\"1\" Red=\"0.25\" Green=\"0.25\" Blue=\"0.25\" Alpha=\"1\" /></ShadowColour><AmbientLight><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></AmbientLight><SkyBox Version=\"1\" MaterialName=\"Examples/CloudBox\" MaterialGroupName=\"files\" DrawFirst=\"1\" Distance=\"50\" ><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" W=\"1\" /></Orientation></SkyBox><Light Version=\"1\" Name=\"ADefaultLightWithNoChangesAtAll\" AttachedTo=\"\" Type=\"1\" PowerScale=\"1\" AttenuationRange=\"100000\" AttenuationConstant=\"1\" AttenuationQuadric=\"0\" AttenuationLinear=\"0\" SpotlightInnerAngle=\"0.523599\" SpotlightOuterAngle=\"0.698132\" SpotlightFalloff=\"1\"><Direction><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\"/></Direction><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location><SpecularColour><ColourValue Version=\"1\" Red=\"0\" Green=\"0\" Blue=\"0\" Alpha=\"1\" /></SpecularColour><fDiffuseColour><ColourValue Version=\"1\" Red=\"1\" Green=\"1\" Blue=\"1\" Alpha=\"1\" /></fDiffuseColour></Light><ParticleEffect Version=\"1\" Name=\"SomeGreenParticles\" AttachedTo=\"\" Enabled=\"0\" Template=\"\"><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" W=\"1\" /></Orientation><Location><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\"/></Location></ParticleEffect><WorldNode Version=\"1\" Name=\"ASimpleFreeNode\" AttachedTo=\"\" StandingType=\"0\"><Orientation><Quaternion Version=\"1\" X=\"-0.325058\" Y=\"0.325058\" Z=\"0\" W=\"0.888074\" /></Orientation><Location><Vector3 Version=\"1\" X=\"2\" Y=\"2\" Z=\"2\"/></Location></WorldNode></SceneManager>" == SerializeTest4.str())
                {
                    AddTestResult("SceneManager::operator<< (SkyBox)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator<< (SkyBox)", Failed, UnitTest::OverWrite);
                }
                StopEngine();

            }else{
                AddTestResult("SceneManager::operator<< (SkyDome)", Skipped);
                AddTestResult("SceneManager::operator<< (SkyPlane)", Skipped);
                AddTestResult("SceneManager::operator<< (SkyBox)", Skipped);
                AddTestResult("SceneManager::operator<< (No Sky)", Skipped);
                AddTestResult("SceneManager::operator>> (SkyDome)", Skipped);
                AddTestResult("SceneManager::operator>> (SkyPlane)", Skipped);
                AddTestResult("SceneManager::operator>> (SkyBox)", Skipped);
                AddTestResult("SceneManager::operator>> (No Sky)", Skipped);
            }

        }
};
#endif
