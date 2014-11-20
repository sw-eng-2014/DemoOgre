/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);
	Real mRotate;          // The rotate constant
	Real mMove;            // The movement constant
 
	SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
 
	Vector3 mDirection;     // Value to move in the correct direction

protected:
    virtual void createScene(void);
  virtual void createScene(void);
    virtual void createFrameListener(void);


    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt );
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent& evt );
    virtual bool keyReleased( const OIS::KeyEvent& evt );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent& evt );
    virtual bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
 
    Ogre::Real mRotate;          // The rotate constant
    Ogre::Real mMove;            // The movement constant
    Ogre::SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
    Ogre::Vector3 mDirection;    // Value to move in the correct direction

};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
