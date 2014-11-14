/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"
//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{

}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
        // add the ninja
	Ogre::Entity *ent = mSceneMgr->createEntity("Ninja", "ninja.mesh");
        Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
        node->attachObject(ent);
 
        // create the light
        Ogre::Light *light = mSceneMgr->createLight("Light1");
        light->setType(Ogre::Light::LT_POINT);
        light->setPosition(Ogre::Vector3(250, 150, 250));
        light->setDiffuseColour(Ogre::ColourValue::White);
        light->setSpecularColour(Ogre::ColourValue::White);
 
       // Create the scene node
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Ogre::Vector3(-400, 200, 400));
 
        // Make it look towards the ninja
        node->yaw(Ogre::Degree(-45));
 
        // Create the pitch node
        node = node->createChildSceneNode("PitchNode1");
        node->attachObject(mCamera);
 
        // create the second camera node/pitch node
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Ogre::Vector3(0, 200, 400));
        node = node->createChildSceneNode("PitchNode2");
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createFrameListener(void)
{
	BaseApplication::createFrameListener();
	// Populate the camera container
	mCamNode = mCamera->getParentSceneNode();
 
	// set the rotation and move speed
	mRotate = 0.13;
	mMove = 250;
	mDirection = Ogre::Vector3::ZERO;
}
//-------------------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed()) return false;
    if (mShutDown) return false;
    mKeyboard->capture();
    mMouse->capture();
    mTrayMgr->frameRenderingQueued(evt);
        return true;
}
//-------------------------------------------------------------------------------------
// OIS::KeyListener
bool TutorialApplication::keyPressed( const OIS::KeyEvent& evt ){
	switch (evt.key)
    {
    case OIS::KC_ESCAPE: 
        mShutDown = true;
        break;
    default:
        break;
    }
	return true;
}
bool TutorialApplication::keyReleased( const OIS::KeyEvent& evt ){return true;}
// OIS::MouseListener
bool TutorialApplication::mouseMoved( const OIS::MouseEvent& evt ){return true;}
bool TutorialApplication::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){return true;}
bool TutorialApplication::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){return true;}
//-------------------------------------------------------------------------------------
 

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
