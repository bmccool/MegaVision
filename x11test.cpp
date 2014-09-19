#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>


// The Keycode to be sent.
// A full list of available codes can be found in /usr/inclucde/X11/keysymdef.h
#define KEYCODE XK_Left

XKeyEvent createKeyEvent(Display *display,
                        Window &win,
                        Window &winRoot,
                        bool press,
                        int keycode,
                        int modifiers)
{
    XKeyEvent event;

    event.display     = display;
    event.window      = win;
    event.root        = winRoot;
    event.subwindow   = None;
    event.time        = CurrentTime;
    event.x           = 1;
    event.y           = 1;
    event.x_root      = 1;
    event.y_root      = 1;
    event.same_screen = true;
    event.state       = modifiers;

    if (press)
    {
        event.type = KeyPress;
    }
    else
    {
        event.type = KeyRelease;
    }
    return event;
}
int catcher( Display *disp, XErrorEvent *xe )
{
        printf( "Something had happened, bruh.\n" );
        return 0;
}

Window *list(Display *disp, unsigned long *len)
{
    Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False), type;
    int form;
    unsigned long remain;
    unsigned char *list;

    XGetWindowProperty(disp, XDefaultRootWindow(disp), prop, 0, 1024, False, XA_WINDOW,
                            &type, &form, len, &remain, &list);
    return (Window *)list;
}
char *name(Display *disp, Window window)
{
    Atom prop = XInternAtom(disp, "WM_NAME", False), type;
    int form;
    unsigned long remain, len;
    unsigned char *list;

    XGetWindowProperty(disp, window, prop, 0, 1024, False, AnyPropertyType,
                            &type, &form, &len, &remain, &list);
    return (char*)list;
}
int main(int argc, char *argv[])
{
    Display *disp;
    Window *wlist;
    unsigned long len;
    char *wname;
    Window winFocus;
    int revert;
    
    disp = XOpenDisplay(NULL);
    Window winRoot = XDefaultRootWindow(disp);


    wlist = (Window*)list(disp, &len);

    //sleep( 3 ); // <-- inserted to give me time to close an open window

    XSetErrorHandler( catcher ); // <-- inserted to set error handler

    int i;
    int nes_index;
    for(i = 0; i < (int)len; i++){
    //        if(wlist[i] != 0){    // <-- apparently futile?
                    wname = name(disp, wlist[i]);
                    if (wname[0] == 'F')
                    {
			printf("here!\n");
                        nes_index = i;
                    }
                    printf("%d: %s %x\n", i, wname, (int)wlist[i]);
                    free(wname);
    //        }
    }
    printf("Nes index is %d, and window is %x\n", nes_index, (int)wlist[nes_index]);
  //XSetInputFocus(Display *display, Window focus, int revert_to, Time time);
    XSetInputFocus(disp, wlist[nes_index], RevertToNone, CurrentTime);
    XGetInputFocus(disp, &winFocus, &revert);
    printf("I set the focus to be %x, and the focus is %x\n", (int)wlist[nes_index], (int)winFocus);
    XKeyEvent event = createKeyEvent(disp, wlist[nes_index], winRoot, true, KEYCODE, 0);
    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
    ////
    XSetInputFocus(disp, wlist[nes_index], RevertToNone, CurrentTime);
    XGetInputFocus(disp, &winFocus, &revert);
    event = createKeyEvent(disp, wlist[nes_index], winRoot, false, KEYCODE, 0);
//    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

    XSetInputFocus(disp, wlist[nes_index], RevertToNone, CurrentTime);
    XGetInputFocus(disp, &winFocus, &revert);
    printf("I set the focus to be %x, and the focus is %x\n", (int)wlist[nes_index], (int)winFocus);
    event = createKeyEvent(disp, wlist[nes_index], winRoot, true, KEYCODE, 0);
    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
    ////
    XSetInputFocus(disp, wlist[nes_index], RevertToNone, CurrentTime);
    XGetInputFocus(disp, &winFocus, &revert);
    event = createKeyEvent(disp, wlist[nes_index], winRoot, false, KEYCODE, 0);
//    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);


    XSetErrorHandler( NULL ); // <-- restore the default error handler
    return 0;
}
