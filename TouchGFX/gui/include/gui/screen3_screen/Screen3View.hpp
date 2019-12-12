#ifndef SCREEN3_VIEW_HPP
#define SCREEN3_VIEW_HPP

#include <gui_generated/screen3_screen/Screen3ViewBase.hpp>
#include <gui/screen3_screen/Screen3Presenter.hpp>

class Screen3View : public Screen3ViewBase
{
public:
    Screen3View();
    virtual ~Screen3View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void playPrevSong();
    virtual void playNextSong();
    virtual void playPause();
    virtual void moveSongPosition(int value);
protected:
};

#endif // SCREEN3_VIEW_HPP
