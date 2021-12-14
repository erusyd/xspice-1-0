/*
 * mfbio.c
 *
 * Copyright -C- 1982 Giles C. Billingsley
 * sccsid "@(#)mfbio.c  1.9  9/3/83"
 *
 *     MFB is a graphics package that was developed by the integrated
 * circuits group of the Electronics Research Laboratory and the
 * Department of Electrical Engineering and Computer Sciences at
 * the University of California, Berkeley, California.  The programs
 * in MFB are available free of charge to any interested party.
 * The sale, resale, or use of these program for profit without the
 * express written consent of the Department of Electrical Engineering
 * and Computer Sciences, University of California, Berkeley, California,
 * is forbidden.
 */


#include "prefix.h"
#include "mfb.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "suffix.h"

/* Library routines */
char *strcpy();
extern void free();

static int MFBsaveflg = 0;    /* used for kludgy ungetc                */
static int Oldsaveflg = 0;    /* ungetc for displays without keyboards */

#define MFBFORMAT   MFBCurrent->strings

struct err errors[] = {
       MFBBADENT,   "Unknown terminal type!",
       MFBMCELNG,   "MFBCAP entry too long!",
       MFBBADMCF,   "Can't open or close mfbcap file!",
       MFBBADMCE,   "Bad mfbcap entry!",
       MFBINFMCE,   "Infinite mfbcap entry!",
       MFBBADTTY,   "stdout not in /dev!",
       MFBBADLST,   "Illegal line style!",
       MFBBADFST,   "Illegal fill style!",
       MFBBADCST,   "Illegal color style!",
       MFBBADTM1,   "No destructive text",
       MFBBADTM2,   "No overstriking text",
       MFBNODFLP,   "No defineable line styles!",
       MFBNODFFP,   "No defineable fill styles!",
       MFBNODFCO,   "No defineable colors!",
       MFBNOBLNK,   "No blinkers!",
       MFBTMBLNK,   "Too many blinkers!",
       MFBNOMASK,   "No defineable read or write masks!",
       MFBBADDEV,   "Can't open or close output device!",
       MFBBADOPT,   "Can't access or set device status!",
       MFBBADWRT,   "Error in write!",
       MFBPNTERR,   "Error in pointing device!",
       MFBNOPTFT,   "No format specified for pointing device!",
       MFBNOPNT,    "No pointing device",
       MFBNORBND,   "No Rubberbanding of pointing device",
       MFBBADALU,   "Cannot set ALU mode",
       0
       };

#define NUMERRORS   (sizeof(errors)/sizeof(struct err))




/*****************************************************************************
 *
 *                    BASIC I/O ROUTINES
 *
 *****************************************************************************/

MFBPutchar(c)
    char c;
    /*
     * Notes:
     *    Place a character in the output buffer and check for overflow.
     */
    {
    MFBCurrent->ttyBuffer[MFBCurrent->numTtyBuffer++] = c;
    if(MFBCurrent->numTtyBuffer >= TTYBUFSIZE)
    MFBUpdate();
    return( (int) c );
    }


MFBPutstr(c,n)
    char *c;
    int n;
    /*
     * Notes:
     *    Place a string of n characters in the output buffer and check
     *     for overflow.
     */
    {
    int i;
    i = n;
    /* Putchar tests for overflow after insertion, Putstr tests before */
    if((MFBCurrent->numTtyBuffer + n + 1) >= TTYBUFSIZE){
    MFBUpdate();
    MFBCurrent->numTtyBuffer = 0;
    }
    while(i--)
    MFBCurrent->ttyBuffer[MFBCurrent->numTtyBuffer++] = *c++;
    return( n );
    }


int
MFBGetchar() {
    static char c;
    char ibuffer[2];
    if( MFBsaveflg != 0 ) 
    MFBsaveflg = 0;
    else{
#ifdef vms
    c = dev_getc(MFBCurrent->fileDesc);
#else
    if(MFBCurrent->fileDesc == 1)
        c = getchar();
    else{
        if((c = (char)read(MFBCurrent->fileDesc,ibuffer,1)) > 0)
            c = ibuffer[0];
        }
#endif
    }
    return( c );
    }


int
MFBUngetchar() {
    MFBsaveflg++;
    }


int
OldGetchar() {
    /*
     * Notes:
     *    If the graphics display does not have a keyboard, mfb
     *    will use the standard getchar routine to acquire input
     *    from the user's terminal.
     */
    static char c;
    if( Oldsaveflg != 0 ) 
    Oldsaveflg = 0;
    else{
#ifdef vms
    c = dev_getchar();
#else
    c = getchar();
#endif
    }
    return( c );
    }


int
OldUngetchar() {
    Oldsaveflg++;
    }


int
MFBUpdate() {
    /*
     * Notes:
     *    Flush the output buffer to the graphics display.
     */
    int value;
#ifdef vms
    if((value = MFBCurrent->numTtyBuffer) > 0)
        dev_write(MFBCurrent->fileDesc,
        MFBCurrent->ttyBuffer,MFBCurrent->numTtyBuffer);
#else
    value =
    write(MFBCurrent->fileDesc,MFBCurrent->ttyBuffer,MFBCurrent->numTtyBuffer);
#endif
    MFBCurrent->numTtyBuffer = 0;
    return(value);
    }


int
MFBFlushInput(){
#ifndef vms
    /*
     * In the UNIX file system, all pending input is flushed whenever
     * ioctl is called with the TIOCSETP option.
     */

    /* the isatty() is necessary because user may be piping output */
    if(isatty(MFBCurrent->fileDesc)){
        if(ioctl(MFBCurrent->fileDesc,TIOCSETP,
        (char *) &MFBCurrent->graphTtyb.newttyb) < 0)
        return(MFBBADOPT);
    }
    if(MFBCurrent->fileDesc != 1 && isatty(1)){
        if(ioctl(1,TIOCSETP,(char *) &MFBCurrent->kybrdTtyb.newttyb) < 0)
        return(MFBBADOPT);
        }
#endif
    return(MFBOK);
    }



void
MFBAudio(){
    /*
     * Notes:
     *    Rings the Bell.    
     */
    if(MFBFORMAT.audio == NULL || *MFBFORMAT.audio == NULL){
    /* cntrl-g by default */
    if(MFBCurrent->fileDesc == 1)
        MFBPutchar( '\07' );
    else
        (void) putchar( '\07' );
    }
    else{
    MFBGenCode(MFBFORMAT.audio);
    }
    }





/*****************************************************************************
 *
 *                      INITIALIZATION
 *
 *****************************************************************************/

MFB
*MFBOpen(TerminalName,DeviceName,errorcode)
    char *TerminalName;
    char *DeviceName;
    int *errorcode;
    /*
     * Notes:
     *    Called to initialize a graphics terminal.  The character
     *    function MFBError(errnum) returns a pointer to a string
     *    describing the error.
     *
     *    TerminalName is the name used in the MFBCAP file.
     *
     *    DeviceName is a pointer to a string which specifies the name
     *    of the output device.  If DeviceName is a null string,the
     *    current device is assumed (i.e. filedesc = 1, for UNIX).
     *    To avoid output to a tty through stdio, use DeviceName == 
     *    ttyname(fileno(stdout))
     *
     *    errorcode returns an integer specifying the type of error,if
     *    any,which occurs during mfb initialization.
     *
     */
    {
    char *MFBGetStr();
    char *getenv();
#ifndef vms
    char *ttyname();
#endif
    char *malloc();
    char *mfbcapFile;
    char *bufPtr;
    char buf[256], *s, *r;
    char *cp_tildexpand(), *copy();
    MFB *mfb;
    int i;
    FILE *fp;

    if((mfbcapFile = getenv("MFBCAP")) == NULL) {
    /* Here we try to find the mfbcap file in the spice3 library directory.
     * This stuff is spice-dependent...
     */
    for (s = cp_tildexpand(LIBPATH); *s; ) {
        while (isspace(*s))
        s++;
        for (r = buf; *s && !isspace(*s); r++, s++)
        *r = *s;
#ifdef UNIX
        (void) strcpy(r, "mfbcap");
#else UNIX
        (void) strcpy(r, "MFBCAP.");
#endif /*UNIX*/
        if (fp = fopen(buf, "r")) {
        (void)fclose(fp);
        mfbcapFile = copy(buf);
        break;
        }
    }
    if (!mfbcapFile)
        mfbcapFile = DEFAULTMFBCAP;
    }

    if (!(mfb = (MFB *)malloc(sizeof(MFB))))
    return (NULL);
    mfb->terminalName = TerminalName;
    SetCurrentMFB(mfb);

    if((*errorcode = MFBGetEnt(gEntBuf,TerminalName,mfbcapFile)) <= 0){
    /* Don't free the MFB because User may use it for Terminal Name. */
    return(NULL);
    }

    bufPtr = mfb->strBuf;

    if(MFBGetFlag("TTY")){
    mfb->deviceType = TTY;
    if(DeviceName == NULL || *DeviceName == NULL){
#ifdef vms
        dev_open(DeviceName,&mfb->fileDesc);
        mfb->name = getenv("TERM");
#else
        mfb->fileDesc = 1;
        mfb->name = ttyname(2); /* stdout */
#endif
        }
    else{
#ifdef vms
        dev_open(DeviceName,&mfb->fileDesc);
#else
        /* try to create the file if does not exist */
        if(close(creat(DeviceName,0644)) < 0){
        *errorcode = MFBBADDEV;
        return(NULL);
        }

        if((mfb->fileDesc = open(DeviceName,2)) < 0){
        *errorcode = MFBBADDEV;
        return(NULL);
        }
#endif
        mfb->name = DeviceName;
        }

#ifdef DEBUG
    MFBZeroCounters();
#endif

    /*
     * Fill up mfb structure from mfbcap entry
     */
    mfb->litout = (Bool)MFBGetFlag("8BB");
    mfb->raw = (Bool)MFBGetFlag("RAW");
    mfb->strings.startSequence = MFBGetStr("GIS",&bufPtr);
    mfb->strings.endSequence = MFBGetStr("GFS",&bufPtr);
    
    mfb->maxX = MFBGetNum("MXC");
    mfb->maxY = MFBGetNum("MYC");
    mfb->strings.initLineStyles = MFBGetStr("ILS",&bufPtr);
    mfb->strings.initColorStyles = MFBGetStr("ICS",&bufPtr);
    mfb->strings.initFillPatterns = MFBGetStr("IFP",&bufPtr);
    
    /*
     * graphics text info
     */
    mfb->textPositionableBool = (Bool)MFBGetFlag("APT");
    mfb->textRotateBool = (Bool)MFBGetFlag("ROT");
    mfb->strings.rotateTextSequence = MFBGetStr("RTS",&bufPtr);
    mfb->fontHeight = MFBGetNum("GCH");
    mfb->fontWidth = MFBGetNum("GCW");
    mfb->fontXOffset = MFBGetNum("GTW");
    mfb->fontYOffset = MFBGetNum("GTH");
    mfb->strings.graphicsTextStart = MFBGetStr("GTS",&bufPtr);
    mfb->strings.graphicsTextEnd = MFBGetStr("GTE",&bufPtr);
    mfb->replaceTextBool = (Bool)MFBGetFlag("GTR");
    mfb->strings.replaceON = MFBGetStr("RMO",&bufPtr);
    mfb->overstrikeTextBool = (Bool)MFBGetFlag("GTO");
    mfb->strings.overstrikeON = MFBGetStr("OMO",&bufPtr);
    mfb->strings.writePixel = MFBGetStr("WPX",&bufPtr);
    
    /*
     * raster operations
     */
    mfb->minOffScreenX = MFBGetNum("OFFMX");
    mfb->minOffScreenY = MFBGetNum("OFFMY");
    mfb->offScreenDX = MFBGetNum("OFFDX");
    mfb->offScreenDY = MFBGetNum("OFFDY");
    mfb->rastRSCSFBool = (Bool)MFBGetFlag("RSCSF");
    mfb->strings.rastCopyStart = MFBGetStr("RSCPS",&bufPtr);
    mfb->strings.rastCopyEnd = MFBGetStr("RSCPE",&bufPtr);
    mfb->strings.rastCopySource = MFBGetStr("RSSRC",&bufPtr);
    mfb->strings.rastCopyDest = MFBGetStr("RSDST",&bufPtr);
    if(mfb->strings.rastCopyStart == NULL || 
        *mfb->strings.rastCopyStart == NULL)
        mfb->rastCopyBool = false;
    else
        mfb->rastCopyBool = true;
    
    /*
     * display color (intensity) control
     */
    mfb->maxColors = MFBGetNum("MCL");
    mfb->strings.screenFlood = MFBGetStr("GCS",&bufPtr);
    mfb->maxIntensity = MFBGetNum("VTI");
    mfb->vltBool = (Bool)MFBGetFlag("VLT");
    mfb->vltUseHLSBool = (Bool)MFBGetFlag("HLS");
    if(mfb->vltBool) {
        mfb->lengthOfVLT = MFBGetNum("VTL");
        mfb->strings.vltEntry = MFBGetStr("VTE",&bufPtr);
        }
    mfb->strings.setForegroundColor = MFBGetStr("SCS",&bufPtr);
    mfb->strings.channelMaskSet = MFBGetStr("VWM",&bufPtr);
    if(mfb->strings.channelMaskSet == NULL || 
        *mfb->strings.channelMaskSet == NULL)
        mfb->channelMaskBool = false;
    else
        mfb->channelMaskBool = true;
    mfb->strings.readMaskSet = MFBGetStr("SRM",&bufPtr);
    if(mfb->strings.readMaskSet == NULL || 
        *mfb->strings.readMaskSet == NULL)
        mfb->readMaskBool = false;
    else
        mfb->readMaskBool = true;
    
    /*
     * graphics input (pointing device) control
     */
    mfb->PointingDeviceBool = (Bool)MFBGetFlag("POD");
    if(mfb->PointingDeviceBool){
        mfb->buttonsBool = (Bool)MFBGetFlag("PDB");
        if(mfb->buttonsBool){
        mfb->numberOfButtons = MFBmin(MFBGetNum("NPB"),12);
        mfb->buttonMask[0] = MFBGetNum("BU1");
        mfb->buttonMask[1] = MFBGetNum("BU2");
        mfb->buttonMask[2] = MFBGetNum("BU3");
        mfb->buttonMask[3] = MFBGetNum("BU4");
        mfb->buttonMask[4] = MFBGetNum("BU5");
        mfb->buttonMask[5] = MFBGetNum("BU6");
        mfb->buttonMask[6] = MFBGetNum("BU7");
        mfb->buttonMask[7] = MFBGetNum("BU8");
        mfb->buttonMask[8] = MFBGetNum("BU9");
        mfb->buttonMask[9] = MFBGetNum("BU10");
        mfb->buttonMask[10] = MFBGetNum("BU11");
        mfb->buttonMask[11] = MFBGetNum("BU12");
        /* -1 flags an unused and inapplicable button mask */
        for(i = mfb->numberOfButtons; i < 12; ++i)
            mfb->buttonMask[i] = -1;
        }
        mfb->readImmediateBool = (Bool)MFBGetFlag("PRI");
        mfb->strings.enablePointingDevice = MFBGetStr("PDS",&bufPtr);
        mfb->strings.disablePointingDevice = MFBGetStr("PDE",&bufPtr);
        mfb->strings.enableRubberBanding = MFBGetStr("PRBON",&bufPtr);
        mfb->strings.disableRubberBanding = MFBGetStr("PRBOFF",&bufPtr);
        mfb->strings.readPointingDevice = MFBGetStr("PDR",&bufPtr);
        mfb->strings.formatPointingDevice = MFBGetStr("PDF",&bufPtr);
        }

    /*
     * keyboard control
     */
    mfb->keyboardBool = (Bool)MFBGetFlag("KYBRD");
    mfb->strings.keyboardStart = MFBGetStr("KYS",&bufPtr);
    mfb->strings.keyboardEnd = MFBGetStr("KYE",&bufPtr);
    mfb->strings.keyboardBackspace = MFBGetStr("KYB",&bufPtr);
    mfb->keyboardXOffset = MFBGetNum("KYX");
    mfb->keyboardYOffset = MFBGetNum("KYY");
    mfb->strings.audio = MFBGetStr("BELL",&bufPtr);

    /* 
     * blinker control
     */
    mfb->blinkersBool = (Bool)MFBGetFlag("BLD");
    mfb->maxBlinkers = MFBGetNum("NBL");
    mfb->numBlinkers = 0;
    mfb->strings.blinkerON = MFBGetStr("BLS",&bufPtr);
    mfb->strings.blinkerOFF = MFBGetStr("BLE",&bufPtr);

    /*
     * line geometry control
     */
    mfb->maxLineStyles = MFBGetNum("MLS");
    mfb->reissueLineStyleBool = (Bool)MFBGetFlag("RLS");
    mfb->linePatternDefineBool = (Bool)MFBGetFlag("DLP");
    mfb->strings.setLineStyle = MFBGetStr("SLS",&bufPtr);
    mfb->strings.setSolidLineStyle = MFBGetStr("SSLS",&bufPtr);
    mfb->strings.movePenSequence = MFBGetStr("MPS",&bufPtr);
    mfb->strings.drawLineSequence = MFBGetStr("DLS",&bufPtr);
    mfb->strings.drawSolidLineSequence = MFBGetStr("DSL",&bufPtr);
    mfb->strings.drawLineToSequence = MFBGetStr("DLT",&bufPtr);
    mfb->strings.drawSolidLineToSequence = MFBGetStr("DSLT",&bufPtr);
    if(mfb->linePatternDefineBool) {
        mfb->strings.lineDefineStart = MFBGetStr("LDS",&bufPtr);
        mfb->strings.lineDefineFormat = MFBGetStr("LDF",&bufPtr);
        mfb->strings.lineDefineEnd = MFBGetStr("LDE",&bufPtr);
        mfb->lineDefineLength = MFBGetNum("LDL");
        }

    /*
     * box geometry control
     */
    mfb->strings.drawBoxSequence = MFBGetStr("DBS",&bufPtr);
    mfb->strings.drawSolidBoxSequence = MFBGetStr("DSB",&bufPtr);

    /*
     * polygon geometry control
     */
    mfb->filledPlygnBool = (Bool)MFBGetFlag("FPOLY");
    mfb->strings.beginPlygnSequence = MFBGetStr("PLS",&bufPtr);
    mfb->strings.beginSolidPlygnSequence = MFBGetStr("PLSOL",&bufPtr);
    mfb->strings.sendPlygnVertex = MFBGetStr("PLV",&bufPtr);
    mfb->strings.endPlygnSequence = MFBGetStr("PLE",&bufPtr);

    /*
     * fill pattern control
     */
    mfb->maxFillPatterns = MFBGetNum("MFP");
    mfb->fillPtrnDefineBool = (Bool)MFBGetFlag("DFP");
    mfb->strings.setFillPattern = MFBGetStr("SFP",&bufPtr);
    mfb->strings.setSolidFillPattern = MFBGetStr("SSFP",&bufPtr);
    if(mfb->fillPtrnDefineBool) {
        mfb->fillDefineHeight = MFBGetNum("FDH");
        mfb->fillDefineWidth = MFBGetNum("FDW");
        mfb->fillDefineRowMajorBool = (Bool)MFBGetFlag("FDR");
        mfb->strings.fillDefineStart = MFBGetStr("FDS",&bufPtr);
        mfb->strings.fillDefineFormat = MFBGetStr("FDF",&bufPtr);
        mfb->strings.fillDefineEnd = MFBGetStr("FDE",&bufPtr);
        }

    /*
     * Circle geometry control
     */
    mfb->strings.drawCircleSequence = MFBGetStr("DCS",&bufPtr);

    /*
     * ALU modes
     */
    mfb->strings.setALUJAM = MFBGetStr("ALUJAM",&bufPtr);
    mfb->strings.setALUOR = MFBGetStr("ALUOR",&bufPtr);
    mfb->strings.setALUNOR = MFBGetStr("ALUNOR",&bufPtr);
    mfb->strings.setALUEOR = MFBGetStr("ALUEOR",&bufPtr);

    /*
     * set defaults in mfb tty structure
     */
    mfb->cursorColor1Id = 0;
    mfb->cursorColor2Id = 1;
    mfb->fgColorId = UNINITIALIZED;
    mfb->fillPattern = UNINITIALIZED;
    mfb->lineStyle = UNINITIALIZED;
    mfb->channelMask = UNINITIALIZED;
    mfb->readMask = UNINITIALIZED;
    mfb->textMode = UNINITIALIZED;
    
    /*this is changed by MFBInitialize*/
    mfb->ttyIsSet = false;

    mfb->lastX = -1;
    mfb->lastY = -1;
    }
    else{
    /*
     * Hardcopy devices not yet implemented.
     */
    mfb->deviceType = HCOPY;
    }

    /*
     * Now try to initialize everything,return status.
     */
    *errorcode = MFBInitialize();
    mfb->initializedBool = true;

    return(mfb);
    }


int
MFBInitialize()
    /*
     * Notes:
     *    Called to reinitialize a graphics terminal.
     *    TTY mode is reset if necessary (UNIX only).
     */
    {
    int i;

    if(MFBCurrent->deviceType == TTY) {
    i = MFBInitTTY();
    if(i < 0)
        return(i);

    /*
     * Output initialization start string
     */
    MFBGenCode(MFBFORMAT.startSequence);
    
    /*
     * Flush the buffer
     */
    MFBUpdate();

    /*
     * Output initialization
     */
    MFBGenCode(MFBFORMAT.initLineStyles);
    MFBGenCode(MFBFORMAT.initColorStyles);
    MFBGenCode(MFBFORMAT.initFillPatterns);
    MFBCurrent->lastX = -1;
    MFBCurrent->lastY = -1;

    /*
     * And flush the buffer again
     */
    MFBUpdate();
    return(MFBOK);
    }
    else{
    return(MFBOK);
    }
    }


int
MFBInitTTY(){
    /*
     * Notes:
     *    Initialize TTY and save current state in MFB structure.
     *    Also,the output buffer is flushed.
     */
#ifndef vms
    long litout = LLITOUT;
#endif

    if(MFBCurrent->deviceType != TTY)
    return(MFBBADTTY);
    MFBCurrent->numTtyBuffer = 0;
    if(!MFBCurrent->ttyIsSet) {
#ifndef vms
    if(*MFBCurrent->name == NULL) {       /* => stdout not in /dev */
        return(MFBBADTTY);
        }
    /*
     * Set keyboard tty options if necessary.
     * We put the terminal in CBREAK so that any keyboard
     * input will be echoed on the graphics display.
     */

    /* the isatty() is necessary because user may be piping output */
    if(MFBCurrent->fileDesc != 1 && isatty(1)){
        /*
         * Disable user messages by changing the fstats of stdin.
         * Yes, this is necessary; the user may open his own terminal
         * as the graphics device in which case user messages will
         * still interrupt the graphics via stdio.
         */
        if(fstat(1,&MFBCurrent->oldstat.kybrdstat) < 0)
        return(MFBBADOPT);
        /* 0022 corresponds to write by group and others */
        /* This is bad -- Wayne
        if(chmod(ttyname(2),MFBCurrent->oldstat.kybrdstat.st_mode & ~0022)
        < 0) return(MFBBADOPT);
        */
        if(ioctl(1,TIOCGETP,(char *) &MFBCurrent->kybrdTtyb.oldttyb) < 0)
        return(MFBBADOPT);
        if(ioctl(1,TIOCLGET,(char *) &MFBCurrent->oldstat.kybrdttyw) < 0)
        return(MFBBADOPT);
        MFBCurrent->kybrdTtyb.newttyb = MFBCurrent->kybrdTtyb.oldttyb;
        MFBCurrent->kybrdTtyb.newttyb.sg_flags &= (ALLDELAY & ~XTABS);
        MFBCurrent->kybrdTtyb.newttyb.sg_flags |= ODDP | EVENP | CBREAK;
        MFBCurrent->kybrdTtyb.newttyb.sg_erase = 8;     /* ctrl-h */
        MFBCurrent->kybrdTtyb.newttyb.sg_kill = 24;     /* ctrl-x */
        if(ioctl(1,TIOCSETP,(char *) &MFBCurrent->kybrdTtyb.newttyb) < 0)
        return(MFBBADOPT);
        }
    /*
     * Set graphics device tty options
     */

    /* the isatty() is necessary because user may be piping output */
    if(isatty(MFBCurrent->fileDesc)){
        /* Disable user messages */
        if(stat(MFBCurrent->name,&MFBCurrent->oldstat.graphstat) < 0)
            return(MFBBADOPT);
        /* 0022 corresponds to write by group and others */
        /*
        if(chmod(MFBCurrent->name,
        MFBCurrent->oldstat.graphstat.st_mode & ~0022) < 0)
        return(MFBBADOPT);
        */
        if(ioctl(MFBCurrent->fileDesc,TIOCGETP,
        (char *) &MFBCurrent->graphTtyb.oldttyb) < 0) return(MFBBADOPT);
        if(ioctl(MFBCurrent->fileDesc,TIOCLGET,
        (char *) &MFBCurrent->oldstat.graphttyw) < 0) return(MFBBADOPT);
        MFBCurrent->graphTtyb.newttyb = MFBCurrent->graphTtyb.oldttyb;
        MFBCurrent->graphTtyb.newttyb.sg_flags &= (ALLDELAY & ~XTABS);
        MFBCurrent->graphTtyb.newttyb.sg_flags |= ODDP | EVENP | CBREAK;
        if(MFBCurrent->fileDesc != 1){
            if(MFBCurrent->graphTtyb.newttyb.sg_ispeed < B1200)
                MFBCurrent->graphTtyb.newttyb.sg_ispeed = B9600;
            if(MFBCurrent->graphTtyb.newttyb.sg_ospeed < B1200)
                MFBCurrent->graphTtyb.newttyb.sg_ospeed = B9600;
            }
        if(MFBCurrent->litout){
            if(ioctl(MFBCurrent->fileDesc,TIOCLBIS,(char *) &litout) < 0)
            return(MFBBADOPT);
            }
        if(MFBCurrent->raw)
            MFBCurrent->graphTtyb.newttyb.sg_flags |= RAW;
        MFBCurrent->graphTtyb.newttyb.sg_erase = 8;     /* ctrl-h */
        MFBCurrent->graphTtyb.newttyb.sg_kill = 24;     /* ctrl-x */
        if(ioctl(MFBCurrent->fileDesc,TIOCSETP,
        (char *) &MFBCurrent->graphTtyb.newttyb) < 0) return(MFBBADOPT);
        }
#endif
    MFBCurrent->outchar = MFBPutchar;
    MFBCurrent->outstr = MFBPutstr;
    MFBCurrent->dsply_getchar = MFBGetchar;
    MFBCurrent->dsply_ungetchar = MFBUngetchar;
    if(MFBCurrent->keyboardBool){
        MFBCurrent->kybrd_getchar = MFBGetchar;
        MFBCurrent->kybrd_ungetchar = MFBUngetchar;
        }
    else{
        MFBCurrent->kybrd_getchar = OldGetchar;
        MFBCurrent->kybrd_ungetchar = OldUngetchar;
        }
    MFBCurrent->ttyIsSet = true;
    }
    return(MFBOK);
    }






/*****************************************************************************
 *
 *                       TERMINATION ROUTINES
 *
 *****************************************************************************/

int
MFBClose(){
    /*
     * Notes:
     *    Called to end the graphics sequence.  Must be last procedure
     *    invoked because it frees the MFB struct.
     */

    /*
     * Flush the buffer
     */
    MFBUpdate();
    if(!MFBCurrent->initializedBool) {
    free((char *) MFBCurrent);
    return(MFBOK);
    }

    /*
     * Put terminal in the state that existed before MFBOpen
     */
    if(MFBCurrent->deviceType == TTY) {
    /*
     * Output initialization end string
     */
    MFBGenCode(MFBFORMAT.endSequence);

    /*
     * Flush the buffer
     */
    MFBUpdate();

#ifndef vms
    /*
     * Reset graphics tty options
     *
     * NOTE: we must reset these options in just the opposite order
     * in which they were set.  The user might have opened his own
     * terminal, in which case the fileDesc is not unity, but we
     * are still setting the standard i/o device.
     */
    
    /* the isatty() is necessary because user may be piping output */
    if(isatty(MFBCurrent->fileDesc)){
        /*
        if(chmod(MFBCurrent->name,
        MFBCurrent->oldstat.graphstat.st_mode) < 0) return(MFBBADOPT);
        */
        if(ioctl(MFBCurrent->fileDesc,TIOCSETP,
        (char *) &MFBCurrent->graphTtyb.oldttyb) < 0) return(MFBBADOPT);
        if(ioctl(MFBCurrent->fileDesc,TIOCLSET,
        (char *) &MFBCurrent->oldstat.graphttyw) < 0) return(MFBBADOPT);
        }
    
    /* the isatty() is necessary because user may be piping output */
    if(MFBCurrent->fileDesc != 1 && isatty(1)){
        /*
        if(chmod(ttyname(2),MFBCurrent->oldstat.kybrdstat.st_mode) < 0)
        return(MFBBADOPT);
        */
        if(ioctl(1,TIOCSETP,(char *) &MFBCurrent->kybrdTtyb.oldttyb) < 0)
        return(MFBBADOPT);
        if(ioctl(1,TIOCLSET,(char *) &MFBCurrent->oldstat.kybrdttyw) < 0)
        return(MFBBADOPT);
        }
#endif

    /*
     * don't close stdout
     */
    if(MFBCurrent->fileDesc != 1)
        if(close(MFBCurrent->fileDesc) < 0)
        return(MFBBADDEV);
    }

    /*
     * Deinitialize the mfb structure
     */
    MFBCurrent->initializedBool = false;
    free((char *) MFBCurrent);
    return(MFBOK);
    }


int
MFBHalt(){
    /*
     * Notes:
     *    Called to disable the graphics sequence.
     *    Has same effect as MFBHalt, except that the MFBCurrent is not freed.
     *
     *    PERFECT FOR SUSPENDING A JOB IN UNIX!
     */


    /*
     * Put terminal in the state that existed before MFBOpen
     */
    if(MFBCurrent->deviceType == TTY) {
    /*
     * Clear the output buffer
     */
    MFBUpdate();

    /*
     * Output initialization end string
     */
    MFBGenCode(MFBFORMAT.endSequence);

    /*
     * Flush the buffer
     */
    MFBUpdate();

#ifndef vms
    /*
     * Reset graphics tty options
     *
     * NOTE: we must reset these options in just the opposite order
     * in which they were set.  The user might have opened his own
     * terminal, in which case the fileDesc is not unity, but we
     * are still setting the standard i/o device.
     */

    /* the isatty() is necessary because user may be piping output */
    if(isatty(MFBCurrent->fileDesc)){
        /*
        if(chmod(MFBCurrent->name,
        MFBCurrent->oldstat.graphstat.st_mode) < 0) return(MFBBADOPT);
        */
        if(ioctl(MFBCurrent->fileDesc,TIOCSETP,
        (char *) &MFBCurrent->graphTtyb.oldttyb) < 0) return(MFBBADOPT);
        if(ioctl(MFBCurrent->fileDesc,TIOCLSET,
        (char *) &MFBCurrent->oldstat.graphttyw) < 0) return(MFBBADOPT);
        }

    /* the isatty() is necessary because user may be piping output */
    if(MFBCurrent->fileDesc != 1 && isatty(1)){
        /*
        if(chmod(ttyname(2),MFBCurrent->oldstat.kybrdstat.st_mode) < 0)
        return(MFBBADOPT);
        */
        if(ioctl(1,TIOCSETP,(char *) &MFBCurrent->kybrdTtyb.oldttyb) < 0)
        return(MFBBADOPT);
        if(ioctl(1,TIOCLSET,(char *) &MFBCurrent->oldstat.kybrdttyw) < 0)
        return(MFBBADOPT);
        }
#endif
    MFBCurrent->ttyIsSet = false;
    }

    return(MFBOK);
    }





 /*****************************************************************************
 *
 *                     INPUT ROUTINES
 *
 *****************************************************************************/

int
MFBPoint(x,y,key,button)
    int *x,*y,*button;
    char *key;
    /*
     * Notes:
     *    None at this time.
     */
    {
    int status;
    int retrnd;

    if(!MFBCurrent->PointingDeviceBool)
    return(MFBNOPNT);
    if((retrnd = MFBFlushInput()) < 0)
    return(retrnd);
    /*
     * Enable Pointing Device
     */
    MFBCurrent->X = MFBCurrent->cursorColor1Id;
    MFBCurrent->Y = MFBCurrent->cursorColor2Id;
    MFBGenCode(MFBFORMAT.enablePointingDevice);
    MFBCurrent->X = MFBCurrent->Y = -1;
    MFBCurrent->Z = MFBCurrent->T = 0;

    /*
     * If Read Will Cause Immediate Transfer,Wait For a Character
     */
    if(MFBCurrent->readImmediateBool) {
    MFBUpdate();
    MFBCurrent->Z = (*MFBCurrent->dsply_getchar)();
    }

    /*
     * Trigger Pointing Device Read
     */
    MFBGenCode(MFBFORMAT.readPointingDevice);

    /*
     * And Flush The Buffer
     */
    MFBUpdate();

    /*
     * Read and Format Cursor Data
     */
    /* MFBDecode returns >= 0 if OK, character barfed on if ERROR. */
    status  = MFBDecode(MFBFORMAT.formatPointingDevice);
    *button = MFBCurrent->T;
    *key    = (char)MFBCurrent->Z;
    *x    = MFBCurrent->X;
    *y    = MFBCurrent->Y;
     
    /*
     * Disable Pointing Device
     */
    MFBGenCode(MFBFORMAT.disablePointingDevice);

    /*
     * MFBPoint may destroy last x/y,so be safe about it.
     */
    MFBCurrent->lastX  = MFBCurrent->lastY  = -1;

    if((retrnd = MFBFlushInput()) < 0)
    return(retrnd);
    if(status >= 0) {
    /* character returned */
    *key = (char)status;
    return(MFBOK);
    }
    else if(status == MFBCAPNOFORMAT)
    return(MFBNOPTFT);
    else if(status == MFBCAPOK)
    return(MFBOK);
    else
    return(MFBPNTERR);
    }


char
*MFBKeyboard(x,y,background,foreground)
    int x,y;
    int background;
    int foreground;
    {
    /*
     * Notes:
     *    Read type-in from keyboard until user types return key. 
     *    Do rub-out processing--rub out and ctrl-h keys.
     */
    int Int1;
    int X,Y;
    int MaxX,MaxY;
    int OldTextMode;
    int OldForeground;
    int OldFillPattern;
    static char MFBTypeIn[161];
    char c[2];

    if(MFBCurrent->deviceType != TTY)
    return(NULL);
    Int1 = 0;
    c[1] = NULL;
    OldTextMode = MFBCurrent->textMode;
    OldForeground = MFBCurrent->fgColorId;
    OldFillPattern = MFBCurrent->fillPattern;
    MFBSetFillPattern(0);
    MFBSetTextMode(true);
    MFBSetColor(foreground);
    if(MFBFORMAT.keyboardStart == NULL ||
    *MFBFORMAT.keyboardStart == NULL){
    /*
     * NAIVE KEYBOARD
     */
    if(!MFBCurrent->textPositionableBool)
        return(NULL);
    MaxX = MFBCurrent->maxX - MFBCurrent->keyboardXOffset;
    MaxY = MFBCurrent->maxY - MFBCurrent->keyboardYOffset;
    /* echo characters using graphtext */
    X = x + MFBCurrent->keyboardXOffset;
    Y = y + MFBCurrent->keyboardYOffset;
    for(;;){
        X += (MFBCurrent->fontWidth + MFBCurrent->fontXOffset);
        MFBUpdate();
        if((*c = (*MFBCurrent->kybrd_getchar)()) == 13 || *c == 10){
        MFBTypeIn[Int1] = NULL;
        break;
        }
        else if(*c == 8 || *c == 0177){    /* ctrl-h or del */
        X -= (MFBCurrent->fontWidth + MFBCurrent->fontXOffset);
        if(Int1 > 0){              /* can't bs further . . . */
            MFBSetColor(background);
            MFBBox(X+MFBCurrent->keyboardXOffset,
            Y+MFBCurrent->keyboardYOffset-MFBCurrent->fontYOffset,
            X+MFBCurrent->keyboardXOffset+MFBCurrent->fontWidth
                +MFBCurrent->fontXOffset,
            Y+MFBCurrent->keyboardYOffset+MFBCurrent->fontHeight);
            MFBSetColor(foreground);
            X -= (MFBCurrent->fontWidth + MFBCurrent->fontXOffset);
            Int1--;
            }
        }
        else if(*c == 24 || *c == 21){  /* ctrl-x or ctrl-u */
        MFBSetColor(background);
        MFBBox(x+MFBCurrent->keyboardXOffset,
            Y+MFBCurrent->keyboardYOffset-MFBCurrent->fontYOffset,
            X+MFBCurrent->keyboardXOffset,
            Y+MFBCurrent->keyboardYOffset+MFBCurrent->fontHeight);
        MFBSetColor(foreground);
        X = x + MFBCurrent->keyboardXOffset;
        Int1 = 0;
        }
        else if(*c == 27){  /* esc */
        MFBText(" ESCAPE",X,Y,0);
        MFBTypeIn[0] = NULL;
        Int1 = 0;
        break;
        }
        else{
        MFBTypeIn[Int1++] = *c;
        X = MFBmin(MaxX,X);
        Y = MFBmin(MaxY,Y);
        if(*c >= ' ' && *c <= '~')
            MFBText(c,X,Y,0);
        else
            MFBText("^",X,Y,0);
        }
        }
    }
    else {
    MFBCurrent->X = x + MFBCurrent->keyboardXOffset;
    MFBCurrent->Y = y + MFBCurrent->keyboardYOffset;
    MFBGenCode(MFBFORMAT.keyboardStart);
    MFBPutchar(' ');
    for(;;){
        MFBPutchar(' ');
        MFBGenCode(MFBFORMAT.keyboardBackspace);
        MFBUpdate();
        if((*c = (*MFBCurrent->kybrd_getchar)()) == 13 || *c == 10){
        MFBTypeIn[Int1] = NULL;
        break;
        }
        else if(*c == 8 || *c == 0177){
        if(Int1 == 0)   /* can't bs further . . . */
            MFBPutchar(7);
        else{
            MFBPutchar(' ');
            MFBGenCode(MFBFORMAT.keyboardBackspace);
            MFBGenCode(MFBFORMAT.keyboardBackspace);
            Int1--;
            }
        }
        else if(*c == 24 || *c == 21){  /* ctrl-x or ctrl-u */
        for(; Int1 > 0; Int1--){
            MFBPutchar(' ');
            MFBGenCode(MFBFORMAT.keyboardBackspace);
            MFBGenCode(MFBFORMAT.keyboardBackspace);
            }
        MFBPutchar(' ');
        MFBGenCode(MFBFORMAT.keyboardBackspace);
        }
        else if(*c == 27){  /* esc */
        MFBTypeIn[0] = NULL;
        break;
        }
        else{
        MFBTypeIn[Int1++] = *c;
        if(*c >= ' ' && *c <= '~')
            MFBPutchar(*c);
        else
            MFBPutchar('^');
        }
        }
    MFBGenCode(MFBFORMAT.keyboardEnd);
    }
    MFBSetTextMode(OldTextMode);
    MFBSetColor(OldForeground);
    MFBSetFillPattern(OldFillPattern);
    /*
     * MFBKeyboard may destroy last x/y,so be safe about it.
     */
    MFBCurrent->lastX = MFBCurrent->lastY  = -1;

    return(MFBTypeIn);
    }






/*****************************************************************************
 *
 *      ERROR HANDLING ROUTINES
 *
 *****************************************************************************/

char *
MFBError(errnum)
    int errnum;
    {
    int i;
    static char result[200];
    for(i = 0; i < NUMERRORS; i++) {
    if(errors[i].errnum == errnum) {
        (void) strcpy(result,errors[i].message);
        return(result);
        }
    }
    (void) sprintf(result,"Illegal Error Number %d in MFBError!",errnum);
    return(result);
    }

