
#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "axmol.h"

/**
@brief    The axmol Application.

Private inheritance here hides part of interface from Director.
*/
class AppDelegate : private ax::Application
{
public:
    AppDelegate();
    ~AppDelegate() override;

    void initGLContextAttrs() override;
    // cấu hình OpenGL tại đây
    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    bool applicationDidFinishLaunching() override;
    // khởi động game 
    /**
    @brief  Called when the application moves to the background
    */
    void applicationDidEnterBackground() override;
    // khi game bị ẩn đi
    /**
    @brief  Called when the application reenters the foreground
    */
    void applicationWillEnterForeground() override;
    // khi game hiện lại
};

#endif  // _APP_DELEGATE_H_
