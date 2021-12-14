
/*
    $Header: //pepper/atesse_spice/spice3/include/RCS/FTEdevice.h,v 1.1 91/04/02 11:25:48 bill Exp $
    Copyright 1987 Jeffrey M. Hsu

    The display device structure.
*/

typedef struct {
    char *name;
    int minx, miny;
    int width, height;      /* in screen coordinate system */
    int numlinestyles, numcolors;   /* number supported */
    int (*Init)();
    int (*NewViewport)();
    int (*Close)();
    int (*Clear)();
    int (*DrawLine)();
    int (*Arc)();
    int (*Text)();
    int (*DefineColor)();
    int (*DefineLinestyle)();
    int (*SetLinestyle)();
    int (*SetColor)();
    int (*Update)();
/*  int (*NDCtoScreen)(); */
    int (*Track)();
    int (*MakeMenu)();
    int (*MakeDialog)();
    int (*Input)();
    int (*DatatoScreen)();
} DISPDEVICE;

extern DISPDEVICE *dispdev;
