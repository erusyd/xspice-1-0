
/*
	$Header: //pepper/atesse_spice/spice3/FTE/RCS/x11.c,v 1.3 92/06/26 15:38:04 bill Exp $
	Copyright 1988 Jeffrey M. Hsu

	X11 drivers.

*/

#include "prefix.h"

#ifdef GI_X11

#include <sys/time.h>

#include "FTEgraph.h"
#ifndef CMS
#include "FTEdbgraph.h"
#else  /* CMS */
#include "FTEdbgra.h"
#endif /* CMS */
#ifndef CMS
#include "FTEdevice.h"
#else  /* CMS */
#include "FTEdevic.h"
#endif /* CMS */
#include "FTEinput.h"

#include "CPdefs.h"
#include "FTEdefs.h"

/* gtri - begin - Update include files for X11 R4+ */
#define XAW_BC
#include <IntrinsicP.h>
#include <Xatom.h>
#include <StringDefs.h>
#include <Xutil.h>
#include <cursorfont.h>
#include <Box.h>
#include <Command.h>
#include <Form.h>
#include <Shell.h>
/* gtri - end - Update include files for X11 R4+ */

#ifdef DEBUG
extern int _Xdebug;
#endif

/* forward declarations */
extern void handlebuttonev(), handlekeypressed(), killwin(), hardcopy(),
	    redraw(), resize();
int errorhandler();

/* X dependent default parameters */
#define DEF_FONT "6x10"
#define NUMLINESTYLES 8
#define NXPLANES 5      /* note: What is this used for? */
#define BOXSIZE 30      /* initial size of bounding box for zoomin */

typedef struct {
	Window window;
	Widget shell, form, view, buttonbox, buttons[2];
	XFontStruct *font;
	GC gc;
	int lastx, lasty;   /* used in X_DrawLine */
	int lastlinestyle;  /* used in X_DrawLine */
} X11devdep;

#define DEVDEP(g) (*((X11devdep *) (g)->devdep))

static Display *display;
static GC xorgc;
static char *xlinestyles[NUMLINESTYLES] = {	/* test patterns XXX */
	"\001\001\001\001",	/* solid */
	"\001\003\001\003",	/* spaced dots */
	"\001\002\001\002",	/* dots */
	"\003\003\003\003",	/* shortdash */
	"\001\004\006\004",	/* dots longdash */
	"\007\007\007\007",	/* longdash */
	"\001\004\004\004",	/* dots shortdash */
	"\003\003\007\003",	/* short/longdash */
};
static Widget toplevel;
static Bool noclear = False;

static GRAPH *lasthardcopy; /* graph user selected */
#ifdef notdef
static Bool KEYwaiting;     /* if user typed something */
#endif

X11_Init()
{

	char buf[512];
	char *displayname;

	XGCValues gcvalues;

	/* grrr, Xtk forced contortions */
	char *argv[2];
	int argc = 2;

	if (cp_getvar("display", VT_STRING, buf)) {
	  displayname = buf;
	} else if (!(displayname = getenv("DISPLAY"))) {
	  internalerror("Can't open X display.");
	  return (1);
	}

#ifdef DEBUG
	_Xdebug = 1;
#endif

	argv[0] = "nutmeg";
	argv[1] = displayname;
/*
	argv[2] = "-geometry";
	argv[3] = "=1x1+2+2";
*/

	/* initialize X toolkit */
	toplevel = XtInitialize("nutmeg", "Nutmeg", NULL, 0, &argc, argv);

	display = XtDisplay(toplevel);

#ifdef notdef
	/* note: set up input sources and handlers for Xt */
	XtAddInput(fileno(stdin), XtInputReadMask,
	        keyhandler, NULL);
#endif

/* note: don't realize the top level widget */

	gcvalues.function = GXxor;
	gcvalues.line_width = 1;
	/* we intend to xor */
	gcvalues.foreground = WhitePixel(display, DefaultScreen(display));
	gcvalues.background = BlackPixel(display, DefaultScreen(display));
	xorgc = XCreateGC(display, DefaultRootWindow(display),
	        GCLineWidth | GCFunction | GCForeground | GCBackground,
	        &gcvalues);

	/* set correct information */
	dispdev->numlinestyles = NUMLINESTYLES;
	dispdev->numcolors = NUMCOLORS;

	dispdev->width = DisplayWidth(display, DefaultScreen(display));
	dispdev->height = DisplayHeight(display, DefaultScreen(display));

	/* we don't want non-fatal X errors to call exit */
	XSetErrorHandler(errorhandler);


	return (0);

}

int
errorhandler(display, errorev)
Display *display;
XErrorEvent *errorev;
{
	XGetErrorText(display, errorev->error_code, ErrorMessage, 1024);
	externalerror(ErrorMessage);
	return;
}

/* Recover from bad NewViewPort call. */
#define RECOVERNEWVIEWPORT()    free((char *) graph);\
	            graph = (GRAPH *) NULL; 
	    /* need to do this or else DestroyGraph will free it again */

/* NewViewport is responsible for filling in graph->viewport */
X11_NewViewport(graph)
GRAPH *graph;
{

	char geometry[32], defaultgeometry[32];
	char fontname[64];
	Cursor cursor;
	XGCValues gcvalues;

	static Arg viewargs[] = {
	    { XtNresizable, (XtArgVal) TRUE },
	    { XtNwidth, (XtArgVal) 300 },
	    { XtNheight, (XtArgVal) 300 },
	};

/*
	static Arg formargs[] = {
	    { NULL, (XtArgVal) NULL },
	};
*/

	Arg bboxargs[1];

	Arg buttonargs[1];

	graph->devdep = calloc(1, sizeof(X11devdep));

	/* set up new shell */
	DEVDEP(graph).shell = XtCreateApplicationShell("shell",
	        topLevelShellWidgetClass, NULL, 0);

	/* set up form widget */
	DEVDEP(graph).form = XtCreateManagedWidget("some name",
	    formWidgetClass, DEVDEP(graph).shell, NULL, 0);

	/* set up viewport */
/* note: allow user to specify geometry XXX */
	DEVDEP(graph).view = XtCreateManagedWidget("viewport", widgetClass,
	    DEVDEP(graph).form, viewargs, XtNumber(viewargs));

/* note: what about error handling?  What if XtCreateWidget fails? XXX */

	/* set up button box */
	XtSetArg(bboxargs[0], XtNfromHoriz, DEVDEP(graph).view);
	DEVDEP(graph).buttonbox = XtCreateManagedWidget("buttonbox",
	    boxWidgetClass, DEVDEP(graph).form, bboxargs, 1);

	/* set up buttons */

	XtSetArg(buttonargs[0], XtNlabel, "quit");
	DEVDEP(graph).buttons[0] = XtCreateManagedWidget("quit",
	    commandWidgetClass, DEVDEP(graph).buttonbox,
	    buttonargs, 1);
	XtAddCallback(DEVDEP(graph).buttons[0], XtNcallback, killwin, graph);

	buttonargs[0].value = (XtArgVal) "hardcopy";
	DEVDEP(graph).buttons[1] = XtCreateManagedWidget("quit",
	    commandWidgetClass, DEVDEP(graph).buttonbox,
	    buttonargs, 1);
	XtAddCallback(DEVDEP(graph).buttons[1], XtNcallback, hardcopy, graph);

	/* set up fonts */
	if (!cp_getvar("font", VT_STRING, fontname)) {
	  (void) strcpy(fontname, DEF_FONT);
	}

	if (!(DEVDEP(graph).font = XLoadQueryFont(display, fontname))) {
	  sprintf(ErrorMessage, "can't open font %s", fontname);
	  internalerror(ErrorMessage);
	  RECOVERNEWVIEWPORT();
	  return(1);
	}
	graph->fontwidth = DEVDEP(graph).font->max_bounds.rbearing -
	        DEVDEP(graph).font->min_bounds.lbearing;
	graph->fontheight = DEVDEP(graph).font->max_bounds.ascent +
	        DEVDEP(graph).font->max_bounds.descent;

	XtRealizeWidget(DEVDEP(graph).shell);

	DEVDEP(graph).window = XtWindow(DEVDEP(graph).view);
	gcvalues.line_width = 1;
	gcvalues.cap_style = CapNotLast;
	DEVDEP(graph).gc = XCreateGC(display, DEVDEP(graph).window,
	        GCLineWidth | GCCapStyle, &gcvalues);

	/* should absolute.positions really be shell.pos? */
	graph->absolute.xpos = DEVDEP(graph).view->core.x;
	graph->absolute.ypos = DEVDEP(graph).view->core.y;
	graph->absolute.width = DEVDEP(graph).view->core.width;
	graph->absolute.height = DEVDEP(graph).view->core.height;

	initlinestyles();
	initcolors(graph);

	/* set up cursor */
	cursor = XCreateFontCursor(display, XC_left_ptr);
	XDefineCursor(display, DEVDEP(graph).window, cursor);

#ifdef notdef
	static String table =
	    "<Configure>:resize()\n\
	    <";
	translations = XtParseTranslationTable(table);
	XtAugmentTranslations(DEVDEP(graph).view, translations);
#endif

	/* set up input events
	    Why bother with event translations? */
	XtAddEventHandler(DEVDEP(graph).view, ButtonPressMask, FALSE,
	        handlebuttonev, graph);
/*  XtAddEventHandler(DEVDEP(graph).view, ButtonMotionMask, FALSE,
	        handlemotionev, graph); */
	XtAddEventHandler(DEVDEP(graph).view, KeyPressMask, FALSE,
	        handlekeypressed, graph);
	XtAddEventHandler(DEVDEP(graph).view, StructureNotifyMask, FALSE,
	        resize, graph);
	XtAddEventHandler(DEVDEP(graph).view, ExposureMask, FALSE,
	        redraw, graph);

	return (0);
}

static
initlinestyles()
{

	int i;

	if (XDisplayPlanes(display, DefaultScreen(display)) > 1) {
	  /* Dotted lines are a distraction when we have colors. */
	  for (i = 2; i < NUMLINESTYLES; i++) {
	    xlinestyles[i] = xlinestyles[0];
	  }
	}

	return;
}

static
initcolors(graph)
GRAPH *graph;
{

	int numdispplanes = DisplayPlanes(display, DefaultScreen(display));
	int i;
	static char *colornames[] = {   "white",    /* white */
	                "black",
	                "red",
	                "blue",
	                "orange",
	                "green",
	                "pink",
	                "brown",
	                "khaki",
	                "plum",
	                "orchid",
	                "violet",
	                "maroon",
	                "turquoise",
	                "sienna",
	                "coral",
	                "cyan",
	                "magenta",
	                "gold",
	                "yellow",
	                ""
	            } ;
	XColor visualcolor, exactcolor;
	char buf[BSIZE], colorstring[BSIZE];
	int xmaxcolors = NUMCOLORS; /* note: can we get rid of this? */

	if (numdispplanes == 1) {
	  /* black and white */
	  graph->colors[0] = WhitePixel(display, DefaultScreen(display));
	  graph->colors[1] = BlackPixel(display, DefaultScreen(display));
	  xmaxcolors = 2;
	  return;
	} else if (numdispplanes < NXPLANES) {
	  xmaxcolors = 1;
	  while (numdispplanes-- > 1)
	    xmaxcolors *= 2;
	}
	for (i = 0; i < xmaxcolors; i++) {
	  (void) sprintf(buf, "color%d", i);
	  if (!cp_getvar(buf, VT_STRING, colorstring))
	    (void) strcpy(colorstring, colornames[i]);
	  if (!XAllocNamedColor(display,
	        DefaultColormap(display, DefaultScreen(display)),
	        colorstring, &visualcolor, &exactcolor)) {
	    (void) sprintf(ErrorMessage,
	      "can't get color %s\n", colorstring);
	    externalerror(ErrorMessage);
	    graph->colors[i] = i ? BlackPixel(display, DefaultScreen(display))
	                : WhitePixel(display, DefaultScreen(display));
	    continue;
	  }
	  graph->colors[i] = visualcolor.pixel;
	}

	return;

}

/* This routine closes the X connection.
	It is not to be called for finishing a graph. */
X11_Close()
{

	XCloseDisplay(display);

}

X11_DrawLine(x1, y1, x2, y2)
int x1, y1, x2, y2;
{

	XDrawLine(display, DEVDEP(currentgraph).window,
	        DEVDEP(currentgraph).gc,
	        x1, currentgraph->absolute.height - y1,
	        x2, currentgraph->absolute.height - y2);


}

/*ARGSUSED*/
X11_Arc(x0, y0, radius, theta1, theta2)
int x0, y0, radius;
double theta1, theta2;
{

	/* never used and not in previous code and XArc doesn't work,
	    so why bother? */

	internalerror("X11_Arc not implemented");

}

/* note: x and y are the LOWER left corner of text */
X11_Text(text, x, y)
char *text;
int x, y;
{

/* We specify text position by lower left corner, so have to adjust for
	X11's font nonsense. */

	XDrawString(display, DEVDEP(currentgraph).window,
	    DEVDEP(currentgraph).gc, x,
	    currentgraph->absolute.height
	        - (y + DEVDEP(currentgraph).font->max_bounds.descent),
	    text, strlen(text));

	/* note: unlike before, we do not save any text here */

}

/*ARGSUSED*/
X11_DefineColor(colorid, red, green, blue)
int colorid;
double red, green, blue;
{

	internalerror("X11_DefineColor not implemented.");

	return(0);

}


/*ARGSUSED*/
X11_DefineLinestyle(linestyleid, mask)
int linestyleid;
int mask;
{

	internalerror("X11_DefineLinestyle not implemented.");

}

X11_SetLinestyle(linestyleid)
int linestyleid;
{

	XGCValues values;

	if (currentgraph->linestyle != linestyleid) {

	  if ((linestyleid == 0) ||
		(DisplayPlanes(display, DefaultScreen(display)) > 1 &&
		linestyleid != 1)) {
	    /* solid if linestyle 0 or if has color,
				allow only one dashed linestyle */
	    values.line_style = LineSolid;
	  } else {
	    values.line_style = LineOnOffDash;
	  }
	  XChangeGC(display, DEVDEP(currentgraph).gc, GCLineStyle, &values);

	  currentgraph->linestyle = linestyleid;
	  XSetDashes(display, DEVDEP(currentgraph).gc, 0,
		xlinestyles[linestyleid], 4);

	}

}

X11_SetColor(colorid)
int colorid;
{

	currentgraph->currentcolor = colorid;
	XSetForeground(display, DEVDEP(currentgraph).gc,
	        currentgraph->colors[colorid]);

}

X11_Update()
{

	XSync(display, 0);

}

X11_Clear()
{

	if (!noclear) /* hack so exposures look like they're handled nicely */
	  XClearWindow(display, DEVDEP(currentgraph).window);

}

void
handlekeypressed(w, clientdata, calldata)
Widget w;
caddr_t clientdata, calldata;
{

	XKeyEvent *keyev = (XKeyPressedEvent *) calldata;
	GRAPH *graph = (GRAPH *) clientdata;
	char text[4];
	int nbytes;

	nbytes = XLookupString(keyev, text, 4, NULL, NULL);
	if (!nbytes) return;
	/* write it */
	PushGraphContext(graph);
	text[nbytes] = '\0';
	SetColor(1);
	Text(text, keyev->x, graph->absolute.height - keyev->y);
	/* save it */
	SaveText(graph, text, keyev->x, graph->absolute.height - keyev->y);
	/* warp mouse so user can type in sequence */
	XWarpPointer(display, None, DEVDEP(graph).window, 0, 0, 0, 0,
	    keyev->x + XTextWidth(DEVDEP(graph).font, text, nbytes),
	    keyev->y);
	PopGraphContext();

}

#ifdef notdef
void
keyhandler(clientdata, source, id)
caddr_t clientdata;
int *source;
XtInputId id;
{

#ifdef notdef
	KEYwaiting = TRUE;
#endif

}
#endif

void
handlebuttonev(w, clientdata, calldata)
Widget w;
caddr_t clientdata, calldata;
{

	XButtonEvent *buttonev = (XButtonEvent *) calldata;

	switch (buttonev->button) {
	  case Button1:
	    slopelocation((GRAPH *) clientdata, buttonev->x, buttonev->y);
	    break;
	  case Button3:
	    zoomin((GRAPH *) clientdata);
	    break;
	}

}

#ifdef notdef
handlemotionev(w, clientdata, calldata)
Widget w;
caddr_t clientdata, calldata;
{

	XMotionEvent *motionev = (XMotionEvent *) calldata;

	switch
}
#endif

slopelocation(graph, x0, y0)
GRAPH *graph;
int x0, y0;         /* initial position of mouse */
{

	int x1, y1;
	int x, y;
	Window rootwindow, childwindow;
	int rootx, rooty;
	unsigned int state;
	double fx0, fx1, fy0, fy1;

	x1 = x0;
	y1 = y0;
	XQueryPointer(display, DEVDEP(graph).window, &rootwindow, &childwindow,
	        &rootx, &rooty, &x, &y, &state);
	XDrawLine(display, DEVDEP(graph).window, xorgc, x0, y0, x0, y1-1);
	XDrawLine(display, DEVDEP(graph).window, xorgc, x0, y1, x1, y1);
	while (state & Button1Mask) {
	  if (x != x1 || y != y1) {
	    XDrawLine(display, DEVDEP(graph).window, xorgc,
	        x0, y0, x0, y1-1);
	    XDrawLine(display, DEVDEP(graph).window, xorgc,
	        x0, y1, x1, y1);
	    x1 = x;
	    y1 = y;
	    XDrawLine(display, DEVDEP(graph).window, xorgc, x0, y0, x0, y1-1);
	    XDrawLine(display, DEVDEP(graph).window, xorgc, x0, y1, x1, y1);
	  }
	  XQueryPointer(display, DEVDEP(graph).window, &rootwindow,
	        &childwindow, &rootx, &rooty, &x, &y, &state);
	}
	XDrawLine(display, DEVDEP(graph).window, xorgc, x0, y0, x0, y1-1);
	XDrawLine(display, DEVDEP(graph).window, xorgc, x0, y1, x1, y1);

	X_ScreentoData(graph, x0, y0, &fx0, &fy0);
	X_ScreentoData(graph, x1, y1, &fx1, &fy1);

	/* print it out */
	if (x1 == x0 && y1 == y0) {     /* only one location */
	  fprintf(stdout, "\nx0 = %g, y0 = %g\n", fx0, fy0);
	} else {    /* need to print info about two points */
	  fprintf(stdout, "\nx0 = %g, y0 = %g    x1 = %g, y1 = %g\n",
	      fx0, fy0, fx1, fy1);
	  fprintf(stdout, "dx = %g, dy = %g\n", fx1-fx0, fy1 - fy0);
	  if (x1 != x0 && y1 != y0) {
	/* add slope info if both dx and dy are zero,
	     because otherwise either dy/dx or dx/dy is zero,
	     which is uninteresting
	*/
	    fprintf(stdout, "dy/dx = %g    dx/dy = %g\n",
	    (fy1-fy0)/(fx1-fx0), (fx1-fx0)/(fy1-fy0));
	  }
	}

	return;

}

/* should be able to do this by sleight of hand on graph parameters */
zoomin(graph)
GRAPH *graph;
{
/* note: need to add circular boxes XXX */

	int x0, y0, x1, y1;
	double fx0, fx1, fy0, fy1, ftemp;
	char buf[BSIZE];
	wordlist *wl;
	int dummy;

	Window rootwindow, childwindow;
	int rootx, rooty;
	unsigned int state;
	int x, y, upperx, uppery, lowerx, lowery;

	/* open box and get area to zoom in on */

	XQueryPointer(display, DEVDEP(graph).window, &rootwindow,
	        &childwindow, &rootx, &rooty, &x0, &y0, &state);

	x = lowerx = x1 = x0 + BOXSIZE;
	y = lowery = y1 = y0 + BOXSIZE;
	upperx = x0;
	uppery = y0;

	XDrawRectangle(display, DEVDEP(graph).window, xorgc,
	        upperx, uppery, lowerx - upperx, lowery - uppery);

/* note: what are src_x, src_y, src_width, and src_height for? XXX */
	XWarpPointer(display, None, DEVDEP(graph).window, 0, 0, 0, 0, x1, y1);

	while (state & Button3Mask) {
	  if (x != x1 || y != y1) {
	    XDrawRectangle(display, DEVDEP(graph).window, xorgc,
	        upperx, uppery, lowerx - upperx, lowery - uppery);
	    x1 = x;
	    y1 = y;
	    /* figure out upper left corner */
	    /* remember X11's (and X10's) demented coordinate system */
	    if (y0 < y1) {
	      uppery = y0;
	      upperx = x0;
	      lowery = y1;
	      lowerx = x1;
	    } else {
	      uppery = y1;
	      upperx = x1;
	      lowery = y0;
	      lowerx = x0;
	    }
	    XDrawRectangle(display, DEVDEP(graph).window, xorgc,
	        upperx, uppery, lowerx - upperx, lowery - uppery);
	  }
	  XQueryPointer(display, DEVDEP(graph).window, &rootwindow,
	           &childwindow, &rootx, &rooty, &x, &y, &state);
	}
	XDrawRectangle(display, DEVDEP(graph).window, xorgc,
	        upperx, uppery, lowerx - upperx, lowery - uppery);

	X_ScreentoData(graph, x0, y0, &fx0, &fy0);
	X_ScreentoData(graph, x1, y1, &fx1, &fy1);

	if (fx0 > fx1) {
	  ftemp = fx0;
	  fx0 = fx1;
	  fx1 = ftemp;
	}
	if (fy0 > fy1) {
	  ftemp = fy0;
	  fy0 = fy1;
	  fy1 = ftemp;
	}

	if (!eq(plot_cur->pl_typename, graph->plotname)) {
	  (void) sprintf(buf,
	   "setplot %s ; %s xlimit %lg %lg ylimit %lg %lg ; setplot $curplot\n",
	   graph->plotname, graph->commandline,
	   fx0, fx1, fy0, fy1);
	} else {
	  (void) sprintf(buf, "%s xlimit %lg %lg ylimit %lg %lg\n",
	        graph->commandline, fx0, fx1, fy0, fy1);
	}

	/* hack for Gordon Jacobs */
	/* add to history list if plothistory is set */
	if (cp_getvar("plothistory", VT_BOOL, (char *) &dummy)) {
	  wl = cp_parse(buf);
	  (void) cp_addhistent(cp_event++, wl);
	}

	(void) cp_evloop(buf);

}

void
hardcopy(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{

	lasthardcopy = (GRAPH *) client_data;
	com_hardcopy(NULL);

}

void
killwin(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{

	GRAPH *graph = (GRAPH *) client_data;

	XtDestroyWidget(DEVDEP(graph).shell);
	DestroyGraph(graph->graphid);

}

/* call higher gr_redraw routine */
void
redraw(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{

	GRAPH *graph = (GRAPH *) client_data;
	XExposeEvent *pev = (XExposeEvent *) call_data;
	XEvent ev;
	XRectangle rects[30];
	int n = 1;

	/* if there is a resize, let the resize routine handle the exposures */
	if (XCheckWindowEvent(display, DEVDEP(graph).window,
	 (long) StructureNotifyMask, &ev)) {
	  resize(w, client_data, &ev);
	  return;
	}

	rects[0].x = pev->x;
	rects[0].y = pev->y;
	rects[0].width = pev->width;
	rects[0].height = pev->height;

	/* pull out all other expose regions that need to be redrawn */
	while (n < 30 && XCheckWindowEvent(display, DEVDEP(graph).window,
	        (long) ExposureMask, &ev)) {
	  pev = (XExposeEvent *) &ev;
	  rects[n].x = pev->x;
	  rects[n].y = pev->y;
	  rects[n].width = pev->width;
	  rects[n].height = pev->height;
	  n++;
	}
	XSetClipRectangles(display, DEVDEP(graph).gc, 0, 0,
	        rects, n, Unsorted);

	noclear = True;
	gr_redraw(graph);
	noclear = False;

	XSetClipMask(display, DEVDEP(graph).gc, None);

}

void
resize(w, client_data, call_data)
Widget w;
caddr_t client_data, call_data;
{

	GRAPH *graph = (GRAPH *) client_data;
	XEvent ev;

	/* pull out all other exposure events
	   Also, get rid of other StructureNotify events on this window.
	    X10 has resize events.  X11 blew it. */
	while (XCheckWindowEvent(display, DEVDEP(graph).window,
	(long) ExposureMask | StructureNotifyMask, &ev)) ;

	graph->absolute.width = w->core.width;
	graph->absolute.height = w->core.height;
	gr_resize(graph);

}

/* stolen from CP/lexical.c */

/* A special 'getc' so that we can deal with ^D properly. There is no way for
 * stdio to know if we have typed a ^D after some other characters, so
 * don't use buffering at all...
 */
static int inchar(fp)
FILE *fp;
{

	char c;
	int i;
	extern int errno;

#ifdef BSD
	if (cp_interactive && !cp_nocc) {
	  i = read((int) fileno(fp), &c, 1);
	  if (i == 0)
	    return (EOF);
	  else if (i == -1) {
	    perror("read");
	    return (EOF);
	  } else
	    return ((int) c);
	}
#endif
#ifdef HPUX
yoyo:
	errno = 0;
#endif
	c = getc(fp);
#ifdef HPUX
	if ((c == EOF) && (errno = EINTR))
	    goto yoyo;
#endif
	return ((int) c);
}

X11_Input(request, response)
REQUEST *request;
RESPONSE *response;
{

	XEvent ev;
	int nfds, readfds;

	switch (request->option) {
	  case char_option:

#ifdef notdef
/* note: check that request->fp has been registered and is
	the event we get XXX */

	    /* handle X events first */
	    do {
	      XtNextEvent(&ev);
	      XtDispatchEvent(&ev);
	    } while (!KEYwaiting);

	    /* dummy typed something */
	    response->reply.ch = inchar(request->fp);
	    KEYwaiting = FALSE;
#endif

	    nfds = ConnectionNumber(display) > fileno(request->fp) ?
	        ConnectionNumber(display) :
	        fileno(request->fp);

	    while (1) {

/*        X11_Update(); */  /* XtPending should flush the queue */

	      /* first read off the queue before doing the select */
	      while (XtPending()) {
	        XtNextEvent(&ev);
	        XtDispatchEvent(&ev);
	      }

	      readfds = 1 << fileno(request->fp) |
	            1 << ConnectionNumber(display);

	      /* block on ConnectionNumber and request->fp */
	      select(nfds + 1, &readfds, (int *) NULL, (int *) NULL, NULL);

	      /* handle X events first */
	      if (readfds & (1 << ConnectionNumber(display))) {
	    /* handle ALL X events */
	    do {
	          XtNextEvent(&ev);
	          XtDispatchEvent(&ev);
	    } while (XtPending());
	      }

	      if (readfds & (1 << fileno(request->fp))) {
	    response->reply.ch = inchar(request->fp);
	    goto out;
	      }

	    }

	    break;
	  case click_option:
	    /* let's fake this */
	    response->reply.graph = lasthardcopy;
	    break;
	  case button_option:
	    /* sit and handle events until get a button selection */
	    internalerror("button_option not implemented");
	    response->option = error_option;
	    return;
	    break;
	  default:
	    internalerror("unrecognized input type");
	    response->option = error_option;
	    return;
	    break;
	}

out:
	response->option = request->option;
	return;

}

static X_ScreentoData(graph, x, y, fx, fy)
GRAPH *graph;
int x,y;
double *fx, *fy;
{

	/* note: figure something out for log grids XXX */

	*fx = (x - graph->viewportxoff) * graph->aspectratiox
	        + graph->datawindow.xmin;
	*fy = ((graph->absolute.height - y) - graph->viewportyoff)
	        * graph->aspectratioy
	        + graph->datawindow.ymin;

}

#else /* not GI_X11 */
int x11_dummy_symbol;
/* otherwise, some linkers get upset */
#endif /* GI_X11 */
