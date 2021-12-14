/*
 * Copyright (c) 1988 Thomas L. Quarles
 */

#include "prefix.h"
#include <stdio.h>
#include "IFsim.h"
#include "INPdefs.h"
#include "INPmacs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
#include "util.h"
#include "suffix.h"

RCSID("INP2dot.c $Revision: 1.2 $ on $Date: 91/05/17 11:01:27 $")

int
INP2dot(ckt,tab,current,task,gnode)
    GENERIC *ckt;
    INPtables *tab;
    card *current;
    GENERIC *task;
    GENERIC *gnode;

{

    /* .<something> Many possibilities */

char *line; /* the part of the current line left to parse */
char *name; /* the resistor's name */
char *nname1;   /* the first node's name */
char *nname2;   /* the second node's name */
GENERIC *node1; /* the first node's node pointer */
GENERIC *node2; /* the second node's node pointer */
int error;      /* error code temporary */
IFvalue ptemp;  /* a value structure to package resistance into */
IFvalue *parm;  /* a pointer to a value struct for function returns */
IFparm *prm;    /* pointer to parameter to search through array */
char *token;    /* a token from the line */
int which;      /* which analysis we are performing */
int i;          /* generic loop variable */
GENERIC *foo;   /* pointer to analysis */
int length;     /* length of a name */
char *steptype; /* ac analysis, type of stepping function */
double dtemp;   /* random double precision temporary */
char *word;     /* something to stick a word of input into */


    line = current->line;
    INPgetTok(&line,&token,1);
    if(strcmp(token,".model")==0) {
        /* don't have to do anything, since models were all done in
         * pass 1 
         */
        return(0);
    } else if ( (strcmp(token,".width")==0) ||
            (strcmp(token,".print")==0) ||
            (strcmp(token,".plot")==0)  ){
        /* obsolete - ignore */
        LITERR(" Warning: obsolete control card - ignored \n")
        return(0);
    } else if ( (strcmp(token,".temp")==0)){
        /* .temp temp1 temp2 temp3 temp4 ..... */
        /* not yet implemented - warn & ignore */
        LITERR(" Warning: .TEMP card obsolete - use .options TEMP and TNOM\n")
        return(0);
    } else if ( (strcmp(token,".op")==0)){
        /* .op */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"DCOP")==0) {
                which=i;
                break;
            }
        }
        if(which == -1) {
            LITERR("DC operating point analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"operating point",&foo, task))
        return(0);
    } else if ( (strcmp(token,".nodeset")==0)){
        /* .nodeset */
        which = -1;
        for(prm=ft_sim->nodeParms;
                prm<ft_sim->nodeParms+ft_sim->numNodeParms;prm++) {
            if(strcmp(prm->keyword,"nodeset")==0) {
                which=prm->id;
                break;
            }
        }
        if(which == -1) {
            LITERR("nodeset unknown to simulator. \n")
            return(0);
        }
        for(;;) { /* loop until we run out of data */
            INPgetTok(&line,&name,1);
            /* check to see if in the form V(xxx) and grab the xxx */
            if( *name == (char)NULL) break; /* end of line */
            length = strlen(name);
            if( (*name == 'V' || *(name) == 'v') && (length == 1)){
                /* looks like V - must be V(xx) - get xx now*/
                INPgetTok(&line,&name,1);
                INPtermInsert(ckt,&name,tab,&node1);
                ptemp.rValue = INPevaluate(&line,&error,1);
                IFC(setNodeParm,(ckt,node1,which,&ptemp,(IFvalue*)NULL))
                continue;
            }
            LITERR(" Error: .nodeset syntax error.\n")
            break;
        }
        return(0);
    } else if ( (strcmp(token,".disto")==0)){
        /* .disto */
        /* not implemented - warn & ignore */
        LITERR("Warning: distortion analysis not implemented-ignored\n")
        return(0);
    } else if ( (strcmp(token,".noise")==0)){
        /* .noise */
        /* not implemented - warn & ignore */
        LITERR("Warning: noise analysis not implemented -ignored\n")
        return(0);
    } else if ( (strcmp(token,".four")==0) || (strcmp(token,".fourier")==0) ){
        /* .four */
        /* not implemented - warn & ignore */
        LITERR("Use fourier command to obtain fourier analysis\n")
        return(0);
    } else if ( (strcmp(token,".ic")==0)){
        /* .ic */
        which = -1;
        for(prm=ft_sim->nodeParms;
                prm<ft_sim->nodeParms+ft_sim->numNodeParms;prm++) {
            if(strcmp(prm->keyword,"ic")==0) {
                which=prm->id;
                break;
            }
        }
        if(which==-1) {
            LITERR("ic unknown to simulator. \n")
            return(0);
        }
        for(;;) { /* loop until we run out of data */
            INPgetTok(&line,&name,1);
            /* check to see if in the form V(xxx) and grab the xxx */
            if( *name == (char)NULL) break; /* end of line */
            length = strlen(name);
            if( (*name == 'V' || *(name) == 'v') && (length == 1)){
                /* looks like V - must be V(xx) - get xx now*/
                INPgetTok(&line,&name,1);
                INPtermInsert(ckt,&name,tab,&node1);
                ptemp.rValue = INPevaluate(&line,&error,1);
                IFC(setNodeParm,(ckt,node1,which,&ptemp,(IFvalue*)NULL))
                continue;
            }
            LITERR(" Error: .ic syntax error.\n")
            break;
        }
        return(0);
    } else if ( (strcmp(token,".ac")==0)){
        /* .ac {DEC OCT LIN} NP FSTART FSTOP */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"AC")==0) {
                which=i;
                break;
            }
        }
        if(which == -1) {
            LITERR("AC small signal analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"acan",&foo, task))
        INPgetTok(&line,&steptype,1); /* get DEC, OCT, or LIN */
        ptemp.iValue=1;
        GCA(INPapName,(ckt,which,foo,steptype,&ptemp))
        parm=INPgetValue(ckt,&line,IF_INTEGER,tab);/* number of points*/
        GCA(INPapName,(ckt,which,foo,"numsteps",parm))
        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* fstart */
        GCA(INPapName,(ckt,which,foo,"start",parm))
        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* fstop */
        GCA(INPapName,(ckt,which,foo,"stop",parm))
        return(0);
    } else if ( (strcmp(token,".sens")==0)){
        /* .sens {AC} {DC} {TRAN} [dev=nnn parm=nnn]* */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"SEN")==0) {
                which=i;
                break;
            }
        }
        if(which == -1) {
            LITERR("sensetivity analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"sens",&foo, task))
        while(*line) { /* read the entire line */
            INPgetTok(&line,&token,1);
            for(i=0;i<ft_sim->analyses[which]->numParms;i++) {
                /* find the parameter */
                if(0==strcmp(token ,
                        ft_sim->analyses[which]->analysisParms[i].
                        keyword) ){
                    /* found it, analysis which, parameter i */
                    if(ft_sim->analyses[which]->analysisParms[i].
                            dataType & IF_FLAG) {
                        /* one of the keywords! */
                        ptemp.iValue = 1;
                        error = (*(ft_sim->setAnalysisParm))(ckt,
                                foo, ft_sim->analyses[which]->
                                analysisParms[i].id,&ptemp,(IFvalue*)NULL);
                        if(error) current->error = INPerrCat(
                                current->error, INPerror(error));
                    } else {
                        parm = INPgetValue(ckt,&line,ft_sim->
                                analyses[which]->analysisParms[i].
                                dataType,tab);
                        error = (*(ft_sim->setAnalysisParm))(ckt,
                                foo, ft_sim->analyses[which]->
                                analysisParms[i].id,parm,(IFvalue*)NULL);
                        if(error) current->error = INPerrCat(
                                current->error, INPerror(error));
                        
                    }
                    break;
                }
            }
            if(i==ft_sim->analyses[which]->numParms) {
                /* didn't find it! */
                LITERR(" Error: unknown parameter on .sens-ignored \n")
            }
        }
        return(0);
    } else if ( (strcmp(token,".pz")==0)){
        /* .pz nodeI nodeG nodeJ nodeK {V I} {POL ZER PZ} */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"PZ")==0) {
                which=i;
                break;
            }
        }
        if(which == -1) {
            LITERR("pole-zero analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"pole-zero analysis",&foo, task))
        parm = INPgetValue(ckt,&line,IF_NODE,tab); 
        GCA(INPapName,(ckt,which,foo,"nodei",parm))
        parm = INPgetValue(ckt,&line,IF_NODE,tab); 
        GCA(INPapName,(ckt,which,foo,"nodeg",parm))
        parm = INPgetValue(ckt,&line,IF_NODE,tab); 
        GCA(INPapName,(ckt,which,foo,"nodej",parm))
        parm = INPgetValue(ckt,&line,IF_NODE,tab); 
        GCA(INPapName,(ckt,which,foo,"nodek",parm))
        INPgetTok(&line,&steptype,1); /* get V or I */
        ptemp.iValue=1;
        GCA(INPapName,(ckt,which,foo,steptype,&ptemp))
        INPgetTok(&line,&steptype,1); /* get POL, ZER, or PZ */
        ptemp.iValue=1;
        GCA(INPapName,(ckt,which,foo,steptype,&ptemp))
        return(0);
    } else if ( (strcmp(token,".dc")==0)){
        /* .dc SRC1NAME Vstart1 Vstop1 Vinc1 [SRC2NAME Vstart2 */
        /*        Vstop2 Vinc2 */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"DCTransfer")==0) {
                which=i;
                break;
            }
        }
        if(which==-1) {
            LITERR("DC transfer curve analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"DCtransferCurve",&foo, task))
        INPgetTok(&line,&name,1);
        INPinsert(&name,tab);
        ptemp.uValue=name;
        GCA(INPapName,(ckt,which,foo,"name1",&ptemp))
        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* vstart1 */
        GCA(INPapName,(ckt,which,foo,"start1",parm))
        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* vstop1 */
        GCA(INPapName,(ckt,which,foo,"stop1",parm))
        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* vinc1 */
        GCA(INPapName,(ckt,which,foo,"step1",parm))
        if(*line) {
            INPgetTok(&line,&name,1);
            INPinsert(&name,tab);
            ptemp.uValue=name;
            GCA(INPapName,(ckt,which,foo,"name2",&ptemp))
            parm = INPgetValue(ckt,&line,IF_REAL,tab); /* vstart1 */
            GCA(INPapName,(ckt,which,foo,"start2",parm))
            parm = INPgetValue(ckt,&line,IF_REAL,tab); /* vstop1 */
            GCA(INPapName,(ckt,which,foo,"stop2",parm))
            parm = INPgetValue(ckt,&line,IF_REAL,tab); /* vinc1 */
            GCA(INPapName,(ckt,which,foo,"step2",parm))
        }
        return(0);
    } else if ( (strcmp(token,".tf")==0)){
        /* .tf v( node1, node2 ) src */
        /* .tf vsrc2             src */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"TF")==0) {
                which=i;
                break;
            }
        }
        if(which==-1) {
            LITERR("DC Transfer function analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"transferFunction",&foo,task))
        INPgetTok(&line,&name,0);
        /* name is now either V or I or a serious error */
        if(*name == 'v' && strlen(name)==1) {
            if(*line != '(' /* match) */ ) { 
                /* error, bad input format */
            }
            INPgetTok(&line,&nname1,0);
            INPtermInsert(ckt,&nname1,tab,&node1);
            ptemp.nValue=node1;
            GCA(INPapName,(ckt,which,foo,"outpos",&ptemp))
            if(*line != /* match ( */ ')') {
                INPgetTok(&line,&nname2,1);
                INPtermInsert(ckt,&nname2,tab,&node2);
                ptemp.nValue=node2;
                GCA(INPapName,(ckt,which,foo,"outneg",&ptemp))
                ptemp.sValue = (char *)
                        MALLOC(sizeof(char)*(5+strlen(nname1)+strlen(nname2)));
                (void)sprintf(ptemp.sValue,"V(%s,%s)",nname1,nname2);
                GCA(INPapName,(ckt,which,foo,"outname",&ptemp))
            } else {
                ptemp.nValue=gnode;
                GCA(INPapName,(ckt,which,foo,"outneg",&ptemp))
                ptemp.sValue = (char *)MALLOC(sizeof(char)*(4+strlen(nname1)));
                (void)sprintf(ptemp.sValue,"V(%s)",nname1);
                GCA(INPapName,(ckt,which,foo,"outname",&ptemp))
            }
        } else {
            INPgetTok(&line,&name,1);
            INPinsert(&name,tab);
            ptemp.uValue=name;
            GCA(INPapName,(ckt,which,foo,"outsrc",&ptemp))
        }
        INPgetTok(&line,&name,1);
        INPinsert(&name,tab);
        ptemp.uValue=name;
        GCA(INPapName,(ckt,which,foo,"insrc",&ptemp))
        return(0);
    } else if ( (strcmp(token,".tran")==0)){
        /* .tran Tstep Tstop <Tstart <Tmax> > <UIC> */
        which = -1;
        for(i=0;i<ft_sim->numAnalyses;i++) {
            if(strcmp(ft_sim->analyses[i]->name,"TRAN")==0) {
                which=i;
                break;
            }
        }
        if(which == -1) {
            LITERR("Transient analysis unsupported\n")
            return(0);
        }
        IFC(newAnalysis,(ckt,which,"transient",&foo, task))
        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* Tstep */
        GCA(INPapName,(ckt,which,foo,"tstep",parm))

/* gtri - add - wbk - 5/17/91 - Modify tmax default */
        /* Set tmax = tstep unless explicitly given on .tran card */
        GCA(INPapName,(ckt,which,foo,"tmax",parm))
/* gtri - end - wbk - 5/17/91 - Modify tmax default */

        parm = INPgetValue(ckt,&line,IF_REAL,tab); /* Tstop */
        GCA(INPapName,(ckt,which,foo,"tstop",parm))
        if(*line) {
            dtemp = INPevaluate(&line,&error,1);  /* tstart? */
            if(error==0) {
                ptemp.rValue=dtemp;
                GCA(INPapName,(ckt,which,foo,"tstart",&ptemp))
                dtemp = INPevaluate(&line,&error,1);  /* tmax? */
                if(error==0) {
                    ptemp.rValue=dtemp;
                    GCA(INPapName,(ckt,which,foo,"tmax",&ptemp))
                }
            }
        }
        if(*line) {
            INPgetTok(&line,&word,1); /* uic? */
            if(strcmp(word,"uic")==0) {
                ptemp.iValue=1;
                GCA(INPapName,(ckt,which,foo,"uic",&ptemp))
            } else {
                LITERR(" Error: unknown parameter on .tran - ignored\n")
            }
        }
        return(0);
    } else if ( (strcmp(token,".subckt")==0) ||
            (strcmp(token,".ends")==0)  ){
        /* not yet implemented - warn & ignore */
        LITERR(" Warning: Subcircuits not yet implemented - ignored \n")
        return(0);
    } else if ( (strcmp(token,".end")==0)){
        /* .end - end of input */
        /* not allowed to pay attention to additional input - return */
        return(1);
        /*NOTREACHED*/
    } else if ( (strcmp(token,".options")==0) ||
            (strcmp(token,".option")==0) ||
            (strcmp(token,".opt")==0) ){
        /* .option - specify program options - rather complicated */
        /* use a subroutine to handle all of them to keep this */
        /* subroutine managable */
        INPdoOpts(ckt,ft_curckt->ci_curOpt,current,tab);
        return(0);
    }
    LITERR(" unimplemented control card - error \n")
    return(0);
}
