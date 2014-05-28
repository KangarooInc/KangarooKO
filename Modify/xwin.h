#ifndef _XWIN_H_
#define _XWIN_H_

extern void initXWindows(void);
extern void cleanupXWindows(void);
extern void set_title(void);
extern void check_resize(XEvent *);
extern void reshape_window(int, int);
extern void setup_screen_res(const int, const int);

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

extern int xres, yres;

#endif
