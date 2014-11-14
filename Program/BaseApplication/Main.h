#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

#include <OgreMeshManager.h>

class Main : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
    Main(void);
    virtual ~Main(void);
    bool go(void);

private:
    Ogre::Root* mRoot;
    Ogre::String mPluginsCfg;
	Ogre::String mOgreCfg;
	Ogre::String mOgreLog;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;

	Ogre::SceneManager* mSceneMgr;

	Ogre::Camera* mCamera;

	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;

protected: 
	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	//
	virtual bool setup(void);
	virtual void createRoot(void);
	virtual void loadConfig(void);
	virtual bool generateRenderWindow(void);
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void startOIS(void);
	virtual void finalTouch(void);
};