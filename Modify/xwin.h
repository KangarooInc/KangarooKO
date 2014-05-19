#ifndef _XWIN_H_
#define _XWIN_H_

extern void initXWindows(void);
extern void cleanupXWindows(void);

extern void set_title(void);
extern void setup_screen_res(const int w, const int h);

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

extern int xres, yres;

#endif
