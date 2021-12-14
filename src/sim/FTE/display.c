
#include "prefix.h"
#include "FTEgraph.h"
#ifndef CMS
#include "FTEdevice.h"
#else  /* CMS */
#include "FTEdevic.h"
#endif /* CMS */
#include "FTEinput.h"
#include "CPdefs.h"     /* for VT_STRING */
#include "FTEdefs.h"        /* for mylog() */

/* forward declarations */
static int nodev(), nop(), gen_DatatoScreen(), gen_Input();

#ifdef GI_X
extern int  X_Init(), X_NewViewport(), X_Close(), X_Clear(),
        X_DrawLine(), X_Arc(), X_Text(), X_DefineColor(),
        X_DefineLinestyle(), X_SetLinestyle(), X_SetColor(),
        X_Update(),
        X_Track(), X_MakeMenu(), X_MakeDialog(), X_Input();
#endif

#ifdef GI_X11
extern int  X11_Init(), X11_NewViewport(), X11_Close(), X11_Clear(),
        X11_DrawLine(), X11_Arc(), X11_Text(), X11_DefineColor(),
        X11_DefineLinestyle(), X11_SetLinestyle(), X11_SetColor(),
        X11_Update(),
        X11_Input();
#endif

#ifdef GI_MFB
extern int  Mfb_Init(), Mfb_NewViewport(), Mfb_Close(), Mfb_Clear(),
        Mfb_DrawLine(), Mfb_Arc(), Mfb_Text(), Mfb_DefineColor(),
        Mfb_DefineLinestyle(), Mfb_SetLinestyle(), Mfb_SetColor(),
        Mfb_Update(),
        Mfb_MakeMenu(), Mfb_MakeDialog();
#endif

extern int  Plt5_Init(), Plt5_NewViewport(), Plt5_Close(), Plt5_Clear(),
        Plt5_DrawLine(), Plt5_Arc(), Plt5_Text(),
        Plt5_DefineLinestyle(), Plt5_SetLinestyle(), Plt5_SetColor(),
        Plt5_Update();

extern int  PS_Init(), PS_NewViewport(), PS_Close(), PS_Clear(),
        PS_DrawLine(), PS_Arc(), PS_Text(),
        PS_DefineLinestyle(), PS_SetLinestyle(), PS_SetColor(),
        PS_Update();

DISPDEVICE device[] = {

    {"error", 0, 0, 0, 0, 0, 0, nop, nop,
    nop, nop,
    nop, nop, nop, nop, nop,
    nop, nop, nop,
    nop, nop, nop, gen_Input,
    nop,},

#ifdef GI_X11
    {"X11", 0, 0, 1024, 864, 0, 0, X11_Init, X11_NewViewport,
    X11_Close, X11_Clear,
    X11_DrawLine, X11_Arc, X11_Text, X11_DefineColor, X11_DefineLinestyle,
    X11_SetLinestyle, X11_SetColor, X11_Update,
    nodev, nodev, nodev, X11_Input,
    gen_DatatoScreen,},
#endif

#ifdef GI_X
    {"X", 0, 0, 1024, 864, 0, 0, X_Init, X_NewViewport,
    X_Close, X_Clear,
    X_DrawLine, X_Arc, X_Text, X_DefineColor, X_DefineLinestyle,
    X_SetLinestyle, X_SetColor, X_Update,
    X_Track, X_MakeMenu, X_MakeDialog, X_Input,
    gen_DatatoScreen,},
#endif

#ifdef GI_MFB
    {"MFB", 0, 0, 1000, 1000, 0, 0, Mfb_Init, Mfb_NewViewport,
    Mfb_Close, Mfb_Clear,
    Mfb_DrawLine, Mfb_Arc, Mfb_Text, Mfb_DefineColor, Mfb_DefineLinestyle,
    Mfb_SetLinestyle, Mfb_SetColor, Mfb_Update,
    nodev, Mfb_MakeMenu, Mfb_MakeDialog, gen_Input,
    gen_DatatoScreen,},
#endif

    {"plot5", 0, 0, 1000, 1000, 0, 0, Plt5_Init, Plt5_NewViewport,
    Plt5_Close, Plt5_Clear,
    Plt5_DrawLine, Plt5_Arc, Plt5_Text, nodev, nodev,
    Plt5_SetLinestyle, Plt5_SetColor, Plt5_Update,
    nodev, nodev, nodev, nodev,
    gen_DatatoScreen,},

    {"postscript", 0, 0, 1000, 1000, 0, 0, PS_Init, PS_NewViewport,
    PS_Close, PS_Clear,
    PS_DrawLine, PS_Arc, PS_Text, nodev, nodev,
    PS_SetLinestyle, PS_SetColor, PS_Update,
    nodev, nodev, nodev, nodev,
    gen_DatatoScreen,},

    {"printf", 0, 0, 24, 80, 0, 0, nodev, nodev,
    nodev, nodev,
    nodev, nodev, nodev, nodev, nodev,
    nodev, nodev, nodev,
    nodev, nodev, nodev, nodev,
    nodev,},

};

DISPDEVICE *dispdev;

#define XtNumber(arr)       (sizeof(arr) / sizeof(arr[0]))

DISPDEVICE *FindDev(name)
char *name;
{
    int i;

    for (i=0; i < XtNumber(device); i++) {
      if (!strcmp(name, device[i].name)) {
        return(&device[i]);
      }
    }
    sprintf(ErrorMessage, "Can't find device %s.", name);
    internalerror(ErrorMessage);
    return(&device[0]);

}

DevInit()
{

    char buf[128];

/* note: do better determination */

/*
    dumb tradition that got passed on from gi_interface
    to do compile time determination
*/
#ifdef GI_MFB
#if defined(GI_X) || defined(GI_X11)
    /* determine display type */
    if (getenv("DISPLAY") || cp_getvar("display", VT_STRING, buf)) {
#ifdef GI_X
      dispdev = FindDev("X");
#endif
#ifdef GI_X11
      dispdev = FindDev("X11");
#endif
    } else {
      dispdev = FindDev("MFB");
    }
#else       /* MFB but not X */
    dispdev = FindDev("MFB");
#endif
#else
#if defined(GI_X) || defined(GI_X11)    /* X but not MFB */
#ifdef GI_X
    dispdev = FindDev("X");
#endif
#ifdef GI_X11
      dispdev = FindDev("X11");
#endif
#else       /* no MFB nor X */
    externalerror(
        "no graphics packages; need to define either GI_X, GI_X11, or GI_MFB");
    dispdev = FindDev("error");
#endif
#endif      /* compile time determination */

    if ((*(dispdev->Init))()) {
      fprintf(cp_err,
        "Warning: can't initialize display device for graphics.\n");
      dispdev = FindDev("error");
    }

}

/* NewViewport is responsible for filling in graph->viewport */
NewViewport(pgraph)
GRAPH *pgraph;
{

    (*(dispdev->NewViewport))(pgraph);

}

DevClose()
{

    (*(dispdev->Close))();

}

DevClear()
{

    (*(dispdev->Clear))();

}

DrawLine(x1, y1, x2, y2)
int x1, y1, x2, y2;
{
    (*(dispdev->DrawLine))(x1, y1, x2, y2);

}

Arc(x0, y0, radius, theta1, theta2)
int x0, y0, radius;
double theta1, theta2;
{

    (*(dispdev->Arc))(x0, y0, radius, theta1, theta2);

}

Text(text, x, y)
char *text;
int x, y;
{

    (*(dispdev->Text))(text, x, y);

}

DefineColor(colorid, red, green, blue)
int colorid;
double red, green, blue;
{

    (*(dispdev->DefineColor))(colorid, red, green, blue);

}

DefineLinestyle(linestyleid, mask)
int linestyleid;
int mask;
{

    (*(dispdev->DefineLinestyle))(linestyleid, mask);

}

SetLinestyle(linestyleid)
int linestyleid;
{

    (*(dispdev->SetLinestyle))(linestyleid);

}

SetColor(colorid)
int colorid;
{

    (*(dispdev->SetColor))(colorid);

}

Update()
{

    (*(dispdev->Update))();

}

/* note: screen coordinates are relative to window
    so need to add viewport offsets */
static gen_DatatoScreen(graph, x, y, screenx, screeny)
GRAPH *graph;
double x, y;
int *screenx, *screeny;
{

    double low, high;

    /* note: may want to cache datawindowsize/viewportsize */ /* done */

    /* note: think this out---Is 1 part of the viewport? Do we handle
        this correctly? */

    /* have to handle several types of grids */

    /* note: we can't compensate for X's demented y-coordinate system here
        since the grid routines use DrawLine w/o calling this routine */
    if ((graph->grid.gridtype == GRID_LOGLOG) ||
            (graph->grid.gridtype == GRID_YLOG)) {
      low = mylog10(graph->datawindow.ymin);
      high = mylog10(graph->datawindow.ymax);
      *screeny = rnd( (((mylog10(y) - low) / (high - low))
            * graph->viewport.height)
            + graph->viewportyoff );
    } else {
      *screeny = rnd( ((y - graph->datawindow.ymin) / graph->aspectratioy)
            + graph->viewportyoff );
    }

    if ((graph->grid.gridtype == GRID_LOGLOG) ||
            (graph->grid.gridtype == GRID_XLOG)) {
      low = mylog10(graph->datawindow.xmin);
      high = mylog10(graph->datawindow.xmax);
      *screenx = rnd( ((mylog10(x) - low) / (high - low))
            * graph->viewport.width
            + graph ->viewportxoff);
    } else {
      *screenx = rnd( (x - graph->datawindow.xmin) / graph->aspectratiox
            + graph ->viewportxoff);
    }

}

DatatoScreen(graph, x, y, screenx, screeny)
GRAPH *graph;
double x, y;
int *screenx, *screeny;
{

    (*(dispdev->DatatoScreen))(graph, x, y, screenx, screeny);

}

/*
NDCtoScreen(x0, y0, px, py)
double x0, y0;
int *px, *py;
{

    (*(dispdev->NDCtoScreen))(x0, y0, px, py);

}
*/

Input(request, response)
REQUEST *request;
RESPONSE *response;
{

    (*(dispdev->Input))(request, response);

}

static
gen_Input(request, response)
REQUEST *request;
RESPONSE *response;
{

    switch (request->option) {
      case char_option:
        response->reply.ch = inchar(request->fp);
        response->option = request->option;
        break;
      default:
        /* just ignore, since we don't want a million error messages */
        response->option = error_option;
        break;
    }
    return;

}

/* no operation, do nothing */
static int nop()
{
    return(1);  /* so NewViewport will fail */
}

static nodev()
{

    sprintf(ErrorMessage,
        "This operation is not defined for display type %s.",
        dispdev->name);
    internalerror(ErrorMessage);
    return(1);

}

SaveText(graph, text, x, y)
GRAPH *graph;
char *text;
int x, y;
{

    struct _keyed *keyed;

    keyed = (struct _keyed *) calloc(1, sizeof(struct _keyed));

    if (!graph->keyed) {
      graph->keyed = keyed;
    } else {
      keyed->next = graph->keyed;
      graph->keyed = keyed;
    }

    keyed->text = malloc(strlen(text) + 1);
    strcpy(keyed->text, text);

    keyed->x = x;
    keyed->y = y;

    keyed->colorindex = graph->currentcolor;

}

/* if given name of a hardcopy device, finds it and switches devices
   if given NULL, switches back */
DevSwitch(devname)
char *devname;
{

    static DISPDEVICE *lastdev = NULL;

    if (devname != NULL) {
      if (lastdev != NULL) {
        internalerror("DevSwitch w/o changing back");
        return (1);
      }
      lastdev = dispdev;
      dispdev = FindDev(devname);
      if (!strcmp(dispdev->name, "error")) {
        internalerror("no hardcopy device");
        dispdev = lastdev;  /* undo */
        lastdev = NULL;
        return (1);
      }
      (*(dispdev->Init))();
    } else {
      (*(dispdev->Close))();
      dispdev = lastdev;
      lastdev = NULL;
    }
    return(0);

}
