// Compile command:
// gcc -Wall -O2 xmappingnotify.c -o xmappingnotify -lX11

#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

int main(int argc, char **argv)
{
	XEvent e;
	Display *d;

	if (!(d = XOpenDisplay(NULL))) {
		fprintf(stderr, "cannot open display\n");
		return 1;
	}

	XKeysymToKeycode(d, XK_F1);

	int xkbEventType;
	XkbQueryExtension(d, 0, &xkbEventType, 0, 0, 0);
//	XkbSelectEvents(d, XkbUseCoreKbd, XkbAllEventsMask, XkbAllEventsMask); // arxiko
//	XkbSelectEventDetails(d, XkbUseCoreKbd, XkbMapNotifyMask, XkbAllEventsMask, XkbAllEventsMask); // δικό μου, παίζει καλύτερα, δεν σκανάρει και το ποντίκι, βλέπει caps lock
	XkbSelectEventDetails(d, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask); // του ρώσου, δεν σκανάρει πλήκτρα, μόνο την αλλαγή γλώσσας. Δεν βλέπει caps

	XSync(d, False);

	while (1) {
		XNextEvent(d, &e);
		if (e.type == xkbEventType) {
			XkbEvent* xkbEvent = (XkbEvent*) &e;
			if (xkbEvent->any.xkb_type == XkbStateNotify) {
				int lang = xkbEvent->state.group;
				if (lang == 1) {
					fprintf(stdout, "1\n");
					fflush(stdout);
				} else {
					fprintf(stdout, "0\n");
					fflush(stdout);
				}
			}
		}
	}

	return(0);
}
