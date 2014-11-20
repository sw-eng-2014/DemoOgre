#include "Main.h"
 
//-------------------------------------------------------------------------------------
Main::Main(void)
	 : mRoot(0),
    mPluginsCfg(Ogre::StringUtil::BLANK),
	mOgreCfg(Ogre::StringUtil::BLANK),
	mOgreLog(Ogre::StringUtil::BLANK),
	mResourcesCfg(Ogre::StringUtil::BLANK)
{
}
//-------------------------------------------------------------------------------------
Main::~Main(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}
 
bool Main::go(void)
{
	return setup();
}

	bool Main::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{
		//Static variables 
		static bool mMouseDown = false;     // If a mouse button is depressed
		static Ogre::Real mRotate = 0.13;   // The rotate constant
		static Ogre::Real mMove = 250;      // The movement constant
		bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);

		if(mWindow->isClosed())
			return false;
 
		//Need to capture/update each device
		mKeyboard->capture();
		mMouse->capture();
 
		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
			return false;
 
		//Togle light on and of 
		if (currMouse && ! mMouseDown)
		{
			Ogre::Light* light = mSceneMgr->getLight("pointLight");
			light->setVisible(! light->isVisible());
		}
		mMouseDown = currMouse;
		//Move character
		Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
		if (mKeyboard->isKeyDown(OIS::KC_W)) // Forward
		{
			transVector.z -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_S)) // Backward
		{
			transVector.z += mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_A)) // Left - yaw or strafe
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw left
				mSceneMgr->getSceneNode("PenguinNode")->yaw(Ogre::Degree(mRotate * 5));
			} else {
				transVector.x -= mMove; // Strafe left
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_D)) // Right - yaw or strafe
		{
			if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
			{
				// Yaw right
				mSceneMgr->getSceneNode("PenguinNode")->yaw(Ogre::Degree(-mRotate * 5));
			} else {
				transVector.x += mMove; // Strafe right
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_Z)) // Up
		{
			transVector.y += mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_X)) // Down
		{
			transVector.y -= mMove;
		}
		if (mKeyboard->isKeyDown(OIS::KC_1)) // Change speed
		{
			mMove = 125;
			mRotate = 0.07;
		}
		if (mKeyboard->isKeyDown(OIS::KC_2)) // Change speed
		{
			mMove = 250;
			mRotate = 0.13;
		}
		mSceneMgr->getSceneNode("PenguinNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		


		return true;
	}

 //Adjust mouse clipping area
void Main::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
//Unattach OIS before window shutdown (very important under Linux)
void Main::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

bool Main::setup(void)
{
	createRoot();
	loadConfig();
	if (!generateRenderWindow()) return false;
	createScene();
	createCamera();
	createViewports();
	startOIS();
	finalTouch();

	return true;
}
void Main::createRoot(void)
{
	mOgreCfg = "ogre.cfg";
	mOgreLog = "Ogre.log";
	#ifdef _DEBUG
		mPluginsCfg = "plugins_d.cfg";
	#else
		mPluginsCfg = "plugins.cfg";
#endif
	// construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg,mOgreCfg,mOgreLog);
}
void Main::loadConfig(void)
{
	#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	#else
	mResourcesCfg = "resources.cfg";
	#endif 
	//Load configFile
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

bool Main::generateRenderWindow(void)
{
		//Render window
		// Show the configuration dialog and initialise the system
		if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		{
			return false;
		}

		//Create a render window
		mWindow = mRoot->initialise(true, "Base application render window");

		// Set default mipmap level (note: some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		// initialise all resource groups
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		return true;
}

void Main::createScene(void)
{
		// Create the SceneManager, in this case a generic one
		mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
		//Set settings
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		//Create penguin
		Ogre::Entity* entPenguin = mSceneMgr->createEntity("Penguin", "penguin.mesh");
		entPenguin -> setCastShadows(true);
		Ogre::SceneNode* nodPenguin = mSceneMgr->getRootSceneNode()->createChildSceneNode("PenguinNode", Ogre::Vector3( 0, 20, 0 ));
		nodPenguin->scale( 1, 1, 1); 
		nodPenguin->attachObject(entPenguin);
  
		//Create floor 
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
 
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
 
		Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
 
		entGround->setMaterialName("Examples/Rockwall");
		entGround->setCastShadows(false);

		// Add block 1 
		Ogre::Entity* entBlock1 = mSceneMgr->createEntity("Block1", "cube.mesh");
		entBlock1 -> setCastShadows(true);
		Ogre::SceneNode* nodBlock1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("Bock1Node", Ogre::Vector3( 100, 0, 100 ));
		nodBlock1->scale( .1, .5, .1); 
		nodBlock1->attachObject(entBlock1);

		// Add block 2
		Ogre::Entity* entBlock2 = mSceneMgr->createEntity("Block2", "cube.mesh");
		entBlock2 -> setCastShadows(true);
		Ogre::SceneNode* nodBlock2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("Bock2Node", Ogre::Vector3( -100, 0, -100 ));
		nodBlock2->scale( 1, 1, 1); 
		nodBlock2->attachObject(entBlock2);

		// Create a light
		Ogre::Light* light = mSceneMgr->createLight("MainLight");
		light ->setType(Ogre::Light::LT_POINT);
		light->setPosition(100,100,100);

}
void Main::createCamera(void)
{
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");
 
		// Position it at 80 in Z direction
		mCamera->setPosition(Ogre::Vector3(200,200,200));
		// Look back along -Z
		mCamera->lookAt(Ogre::Vector3(0,0,0));
		mCamera->setNearClipDistance(5);
}
void Main::createViewports(void)
{
		// Create one viewport, entire window
		Ogre::Viewport* vp = mWindow->addViewport(mCamera);	
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
void Main::startOIS(void)
{
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
 
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
		mInputManager = OIS::InputManager::createInputSystem( pl );
		
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));

		//
		//Set initial mouse clipping size
		windowResized(mWindow);
 
		//Register as a Window listener
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

}
void Main::finalTouch(void)
{
		mRoot->addFrameListener(this);
		mRoot->startRendering();
}


 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Main app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif