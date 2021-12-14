/*
 * Copyright (c) 1985      Hong June Park, Thomas L. Quarles
 */
#ifndef BSIM
#define BSIM "BSIMdefs.h $Revision: 1.1 $  on $Date: 91/04/02 11:25:12 $ "

#include "IFsim.h"
#include "GENdefs.h"
#include "CKTdefs.h"
#include "complex.h"

    /* declarations for BSIM MOSFETs */

/* information needed for each instance */

typedef struct sBSIMinstance {
    struct sBSIMmodel *BSIMmodPtr;      /* pointer to model */
    struct sBSIMinstance *BSIMnextInstance;  /* pointer to next instance of 
                                              *current model*/
    IFuid BSIMname; /* pointer to character string naming this instance */

    int BSIMdNode;  /* number of the gate node of the mosfet */
    int BSIMgNode;  /* number of the gate node of the mosfet */
    int BSIMsNode;  /* number of the source node of the mosfet */
    int BSIMbNode;  /* number of the bulk node of the mosfet */
    int BSIMdNodePrime; /* number of the internal drain node of the mosfet */
    int BSIMsNodePrime; /* number of the internal source node of the mosfet */

    double BSIMl;   /* the length of the channel region */
    double BSIMw;   /* the width of the channel region */
    double BSIMdrainArea;   /* the area of the drain diffusion */
    double BSIMsourceArea;  /* the area of the source diffusion */
    double BSIMdrainSquares;    /* the length of the drain in squares */
    double BSIMsourceSquares;   /* the length of the source in squares */
    double BSIMdrainPerimeter;
    double BSIMsourcePerimeter;
    double BSIMsourceConductance;   /*conductance of source(or 0):set in setup*/
    double BSIMdrainConductance;    /*conductance of drain(or 0):set in setup*/

    double BSIMicVBS;   /* initial condition B-S voltage */
    double BSIMicVDS;   /* initial condition D-S voltage */
    double BSIMicVGS;   /* initial condition G-S voltage */
    double BSIMvon;
    double BSIMvdsat;
    int BSIMoff;        /* non-zero to indicate device is off for dc analysis*/
    int BSIMmode;       /* device mode : 1 = normal, -1 = inverse */

    double BSIMvfb;      /* flat band voltage at given L and W */
    double BSIMphi;      /* surface potential at strong inversion */
    double BSIMK1;       /* bulk effect coefficient 1             */
    double BSIMK2;       /* bulk effect coefficient 2             */
    double BSIMeta;      /* drain induced barrier lowering        */
    double BSIMetaB;     /* Vbs dependence of Eta                 */
    double BSIMetaD;     /* Vds dependence of Eta                 */
    double BSIMbetaZero; /* Beta at vds = 0 and vgs = Vth         */
    double BSIMbetaZeroB; /* Vbs dependence of BetaZero           */
    double BSIMbetaVdd;  /* Beta at vds=Vdd and vgs=Vth           */
    double BSIMbetaVddB; /* Vbs dependence of BetaVdd             */
    double BSIMbetaVddD; /* Vds dependence of BetaVdd             */
    double BSIMugs;      /* Mobility degradation due to gate field*/
    double BSIMugsB;     /* Vbs dependence of Ugs                 */
    double BSIMuds;      /* Drift Velocity Saturation due to Vds  */
    double BSIMudsB;     /* Vbs dependence of Uds                 */
    double BSIMudsD;     /* Vds dependence of Uds                 */
    double BSIMsubthSlope; /* slope of subthreshold current with Vgs*/
    double BSIMsubthSlopeB; /* Vbs dependence of Subthreshold Slope */
    double BSIMsubthSlopeD; /* Vds dependence of Subthreshold Slope */
    double BSIMGDoverlapCap;/* Gate Drain Overlap Capacitance       */
    double BSIMGSoverlapCap;/* Gate Source Overlap Capacitance      */
    double BSIMGBoverlapCap;/* Gate Bulk Overlap Capacitance        */
    double BSIMvt0;
    double BSIMvdd;         /* Supply Voltage                       */
    double BSIMtemp;
    double BSIMoxideThickness;
    double BSIMchannelChargePartitionFlag;


    unsigned BSIMlGiven :1;
    unsigned BSIMwGiven :1;
    unsigned BSIMdrainAreaGiven :1;
    unsigned BSIMsourceAreaGiven    :1;
    unsigned BSIMdrainSquaresGiven  :1;
    unsigned BSIMsourceSquaresGiven :1;
    unsigned BSIMdrainPerimeterGiven    :1;
    unsigned BSIMsourcePerimeterGiven   :1;
    unsigned BSIMdNodePrimeSet  :1;
    unsigned BSIMsNodePrimeSet  :1;
    unsigned BSIMicVBSGiven :1;
    unsigned BSIMicVDSGiven :1;
    unsigned BSIMicVGSGiven :1;
    unsigned BSIMvonGiven   :1;
    unsigned BSIMvdsatGiven :1;


    double *BSIMDdPtr;      /* pointer to sparse matrix element at
                                     * (Drain node,drain node) */
    double *BSIMGgPtr;      /* pointer to sparse matrix element at
                                     * (gate node,gate node) */
    double *BSIMSsPtr;      /* pointer to sparse matrix element at
                                     * (source node,source node) */
    double *BSIMBbPtr;      /* pointer to sparse matrix element at
                                     * (bulk node,bulk node) */
    double *BSIMDPdpPtr;    /* pointer to sparse matrix element at
                                     * (drain prime node,drain prime node) */
    double *BSIMSPspPtr;    /* pointer to sparse matrix element at
                                     * (source prime node,source prime node) */
    double *BSIMDdpPtr;     /* pointer to sparse matrix element at
                                     * (drain node,drain prime node) */
    double *BSIMGbPtr;      /* pointer to sparse matrix element at
                                     * (gate node,bulk node) */
    double *BSIMGdpPtr;     /* pointer to sparse matrix element at
                                     * (gate node,drain prime node) */
    double *BSIMGspPtr;     /* pointer to sparse matrix element at
                                     * (gate node,source prime node) */
    double *BSIMSspPtr;     /* pointer to sparse matrix element at
                                     * (source node,source prime node) */
    double *BSIMBdpPtr;     /* pointer to sparse matrix element at
                                     * (bulk node,drain prime node) */
    double *BSIMBspPtr;     /* pointer to sparse matrix element at
                                     * (bulk node,source prime node) */
    double *BSIMDPspPtr;    /* pointer to sparse matrix element at
                                     * (drain prime node,source prime node) */
    double *BSIMDPdPtr;     /* pointer to sparse matrix element at
                                     * (drain prime node,drain node) */
    double *BSIMBgPtr;      /* pointer to sparse matrix element at
                                     * (bulk node,gate node) */
    double *BSIMDPgPtr;     /* pointer to sparse matrix element at
                                     * (drain prime node,gate node) */

    double *BSIMSPgPtr;     /* pointer to sparse matrix element at
                                     * (source prime node,gate node) */
    double *BSIMSPsPtr;     /* pointer to sparse matrix element at
                                     * (source prime node,source node) */
    double *BSIMDPbPtr;     /* pointer to sparse matrix element at
                                     * (drain prime node,bulk node) */
    double *BSIMSPbPtr;     /* pointer to sparse matrix element at
                                     * (source prime node,bulk node) */
    double *BSIMSPdpPtr;    /* pointer to sparse matrix element at
                                     * (source prime node,drain prime node) */

    int BSIMstates;     /* index into state table for this device */
#define BSIMvbd BSIMstates+ 0
#define BSIMvbs BSIMstates+ 1
#define BSIMvgs BSIMstates+ 2
#define BSIMvds BSIMstates+ 3
#define BSIMcd BSIMstates+ 4
#define BSIMid BSIMstates+ 4
#define BSIMcbs BSIMstates+ 5
#define BSIMibs BSIMstates+ 5
#define BSIMcbd BSIMstates+ 6
#define BSIMibd BSIMstates+ 6
#define BSIMgm BSIMstates+ 7
#define BSIMgds BSIMstates+ 8
#define BSIMgmbs BSIMstates+ 9
#define BSIMgbd BSIMstates+ 10
#define BSIMgbs BSIMstates+ 11
#define BSIMqb BSIMstates+ 12
#define BSIMcqb BSIMstates+ 13
#define BSIMiqb BSIMstates+ 13
#define BSIMqg BSIMstates+ 14
#define BSIMcqg BSIMstates+ 15
#define BSIMiqg BSIMstates+ 15
#define BSIMqd BSIMstates+ 16
#define BSIMcqd BSIMstates+ 17
#define BSIMiqd BSIMstates+ 17
#define BSIMcggb BSIMstates+ 18
#define BSIMcgdb BSIMstates+ 19
#define BSIMcgsb BSIMstates+ 20
#define BSIMcbgb BSIMstates+ 21
#define BSIMcbdb BSIMstates+ 22
#define BSIMcbsb BSIMstates+ 23
#define BSIMcapbd BSIMstates+ 24
#define BSIMiqbd BSIMstates+ 25
#define BSIMcqbd BSIMstates+ 25
#define BSIMcapbs BSIMstates+ 26
#define BSIMiqbs BSIMstates+ 27
#define BSIMcqbs BSIMstates+ 27
#define BSIMcdgb BSIMstates+ 28
#define BSIMcddb BSIMstates+ 29
#define BSIMcdsb BSIMstates+ 30
#define BSIMvono BSIMstates+ 31
#define BSIMvdsato BSIMstates+ 32
#define BSIMqbs  BSIMstates+ 33
#define BSIMqbd  BSIMstates+ 34

#define BSIMnumStates 35           

} BSIMinstance ;


/* per model data */

typedef struct sBSIMmodel {       /* model structure for a resistor */
    int BSIMmodType;    /* type index of this device type */
    struct sBSIMmodel *BSIMnextModel;    /* pointer to next possible model 
                                          *in linked list */
    BSIMinstance * BSIMinstances; /* pointer to list of instances 
                                   * that have this model */
    IFuid BSIMmodName;       /* pointer to character string naming this model */
    int BSIMtype;       /* device type : 1 = nmos,  -1 = pmos */

    double BSIMvfb0;
    double BSIMvfbL;
    double BSIMvfbW;
    double BSIMphi0;
    double BSIMphiL;
    double BSIMphiW;
    double BSIMK10;
    double BSIMK1L;
    double BSIMK1W;
    double BSIMK20;
    double BSIMK2L;
    double BSIMK2W;
    double BSIMeta0;
    double BSIMetaL;
    double BSIMetaW;
    double BSIMetaB0;
    double BSIMetaBl;
    double BSIMetaBw;
    double BSIMetaD0;
    double BSIMetaDl;
    double BSIMetaDw;
    double BSIMdeltaL;
    double BSIMdeltaW;
    double BSIMmobZero;
    double BSIMmobZeroB0;
    double BSIMmobZeroBl;
    double BSIMmobZeroBw ;
    double BSIMmobVdd0;
    double BSIMmobVddl;
    double BSIMmobVddw;
    double BSIMmobVddB0;
    double BSIMmobVddBl;
    double BSIMmobVddBw;
    double BSIMmobVddD0;
    double BSIMmobVddDl;
    double BSIMmobVddDw;
    double BSIMugs0;
    double BSIMugsL;
    double BSIMugsW;
    double BSIMugsB0;
    double BSIMugsBL;
    double BSIMugsBW;
    double BSIMuds0;
    double BSIMudsL;
    double BSIMudsW;
    double BSIMudsB0;
    double BSIMudsBL;
    double BSIMudsBW;
    double BSIMudsD0;
    double BSIMudsDL;
    double BSIMudsDW;
    double BSIMsubthSlope0;
    double BSIMsubthSlopeL;
    double BSIMsubthSlopeW;
    double BSIMsubthSlopeB0;
    double BSIMsubthSlopeBL;
    double BSIMsubthSlopeBW;
    double BSIMsubthSlopeD0;
    double BSIMsubthSlopeDL;
    double BSIMsubthSlopeDW;
    double BSIMoxideThickness;              /* unit: micron  */
    double BSIMCox;                         /* unit: F/cm**2 */
    double BSIMtemp;
    double BSIMvdd;
    double BSIMgateSourceOverlapCap;
    double BSIMgateDrainOverlapCap;
    double BSIMgateBulkOverlapCap;
    double BSIMchannelChargePartitionFlag;

    double BSIMsheetResistance;
    double BSIMjctSatCurDensity;
    double BSIMbulkJctPotential;
    double BSIMbulkJctBotGradingCoeff;
    double BSIMbulkJctSideGradingCoeff;
    double BSIMsidewallJctPotential;
    double BSIMunitAreaJctCap;
    double BSIMunitLengthSidewallJctCap;
    double BSIMdefaultWidth;
    double BSIMdeltaLength;


    unsigned  BSIMvfb0Given   :1;
    unsigned  BSIMvfbLGiven   :1;
    unsigned  BSIMvfbWGiven   :1;
    unsigned  BSIMphi0Given   :1;
    unsigned  BSIMphiLGiven   :1;
    unsigned  BSIMphiWGiven   :1;
    unsigned  BSIMK10Given   :1;
    unsigned  BSIMK1LGiven   :1;
    unsigned  BSIMK1WGiven   :1;
    unsigned  BSIMK20Given   :1;
    unsigned  BSIMK2LGiven   :1;
    unsigned  BSIMK2WGiven   :1;
    unsigned  BSIMeta0Given   :1;
    unsigned  BSIMetaLGiven   :1;
    unsigned  BSIMetaWGiven   :1;
    unsigned  BSIMetaB0Given   :1;
    unsigned  BSIMetaBlGiven   :1;
    unsigned  BSIMetaBwGiven   :1;
    unsigned  BSIMetaD0Given   :1;
    unsigned  BSIMetaDlGiven   :1;
    unsigned  BSIMetaDwGiven   :1;
    unsigned  BSIMdeltaLGiven   :1;
    unsigned  BSIMdeltaWGiven   :1;
    unsigned  BSIMmobZeroGiven   :1;
    unsigned  BSIMmobZeroB0Given   :1;
    unsigned  BSIMmobZeroBlGiven   :1;
    unsigned  BSIMmobZeroBwGiven   :1;
    unsigned  BSIMmobVdd0Given   :1;
    unsigned  BSIMmobVddlGiven   :1;
    unsigned  BSIMmobVddwGiven   :1;
    unsigned  BSIMmobVddB0Given   :1;
    unsigned  BSIMmobVddBlGiven   :1;
    unsigned  BSIMmobVddBwGiven   :1;
    unsigned  BSIMmobVddD0Given   :1;
    unsigned  BSIMmobVddDlGiven   :1;
    unsigned  BSIMmobVddDwGiven   :1;
    unsigned  BSIMugs0Given   :1;
    unsigned  BSIMugsLGiven   :1;
    unsigned  BSIMugsWGiven   :1;
    unsigned  BSIMugsB0Given   :1;
    unsigned  BSIMugsBLGiven   :1;
    unsigned  BSIMugsBWGiven   :1;
    unsigned  BSIMuds0Given   :1;
    unsigned  BSIMudsLGiven   :1;
    unsigned  BSIMudsWGiven   :1;
    unsigned  BSIMudsB0Given   :1;
    unsigned  BSIMudsBLGiven   :1;
    unsigned  BSIMudsBWGiven   :1;
    unsigned  BSIMudsD0Given   :1;
    unsigned  BSIMudsDLGiven   :1;
    unsigned  BSIMudsDWGiven   :1;
    unsigned  BSIMsubthSlope0Given   :1;
    unsigned  BSIMsubthSlopeLGiven   :1;
    unsigned  BSIMsubthSlopeWGiven   :1;
    unsigned  BSIMsubthSlopeB0Given   :1;
    unsigned  BSIMsubthSlopeBLGiven   :1;
    unsigned  BSIMsubthSlopeBWGiven   :1;
    unsigned  BSIMsubthSlopeD0Given   :1;
    unsigned  BSIMsubthSlopeDLGiven   :1;
    unsigned  BSIMsubthSlopeDWGiven   :1;
    unsigned  BSIMoxideThicknessGiven   :1;
    unsigned  BSIMtempGiven   :1;
    unsigned  BSIMvddGiven   :1;
    unsigned  BSIMgateSourceOverlapCapGiven   :1;
    unsigned  BSIMgateDrainOverlapCapGiven   :1;
    unsigned  BSIMgateBulkOverlapCapGiven   :1;
    unsigned  BSIMchannelChargePartitionFlagGiven   :1;
    unsigned  BSIMsheetResistanceGiven   :1;
    unsigned  BSIMjctSatCurDensityGiven   :1;
    unsigned  BSIMbulkJctPotentialGiven   :1;
    unsigned  BSIMbulkJctBotGradingCoeffGiven   :1;
    unsigned  BSIMsidewallJctPotentialGiven   :1;
    unsigned  BSIMbulkJctSideGradingCoeffGiven   :1;
    unsigned  BSIMunitAreaJctCapGiven   :1;
    unsigned  BSIMunitLengthSidewallJctCapGiven   :1;
    unsigned  BSIMdefaultWidthGiven   :1;
    unsigned  BSIMdeltaLengthGiven   :1;
    unsigned  BSIMtypeGiven   :1;

} BSIMmodel;


#ifndef NMOS
#define NMOS 1
#define PMOS -1
#endif /*NMOS*/


/* device parameters */
#define BSIM_W 1
#define BSIM_L 2
#define BSIM_AS 3
#define BSIM_AD 4
#define BSIM_PS 5
#define BSIM_PD 6
#define BSIM_NRS 7
#define BSIM_NRD 8
#define BSIM_OFF 9
#define BSIM_IC_VBS 10
#define BSIM_IC_VDS 11
#define BSIM_IC_VGS 12
#define BSIM_IC 13

/* model parameters */
#define BSIM_MOD_VFB0 101
#define BSIM_MOD_VFBL 102
#define BSIM_MOD_VFBW 103
#define BSIM_MOD_PHI0 104
#define BSIM_MOD_PHIL 105
#define BSIM_MOD_PHIW 106
#define BSIM_MOD_K10 107
#define BSIM_MOD_K1L 108
#define BSIM_MOD_K1W 109
#define BSIM_MOD_K20 110
#define BSIM_MOD_K2L 111
#define BSIM_MOD_K2W 112
#define BSIM_MOD_ETA0 113
#define BSIM_MOD_ETAL 114
#define BSIM_MOD_ETAW 115
#define BSIM_MOD_ETAB0 116
#define BSIM_MOD_ETABL 117
#define BSIM_MOD_ETABW 118
#define BSIM_MOD_ETAD0 119
#define BSIM_MOD_ETADL 120
#define BSIM_MOD_ETADW 121
#define BSIM_MOD_DELTAL 122
#define BSIM_MOD_DELTAW 123
#define BSIM_MOD_MOBZERO 124
#define BSIM_MOD_MOBZEROB0 125
#define BSIM_MOD_MOBZEROBL 126
#define BSIM_MOD_MOBZEROBW 127
#define BSIM_MOD_MOBVDD0 128
#define BSIM_MOD_MOBVDDL 129
#define BSIM_MOD_MOBVDDW 130
#define BSIM_MOD_MOBVDDB0 131
#define BSIM_MOD_MOBVDDBL 132
#define BSIM_MOD_MOBVDDBW 133
#define BSIM_MOD_MOBVDDD0 134
#define BSIM_MOD_MOBVDDDL 135
#define BSIM_MOD_MOBVDDDW 136
#define BSIM_MOD_UGS0 137
#define BSIM_MOD_UGSL 138
#define BSIM_MOD_UGSW 139
#define BSIM_MOD_UGSB0 140
#define BSIM_MOD_UGSBL 141
#define BSIM_MOD_UGSBW 142
#define BSIM_MOD_UDS0 143
#define BSIM_MOD_UDSL 144
#define BSIM_MOD_UDSW 145
#define BSIM_MOD_UDSB0 146
#define BSIM_MOD_UDSBL 147
#define BSIM_MOD_UDSBW 148
#define BSIM_MOD_UDSD0 149
#define BSIM_MOD_UDSDL 150
#define BSIM_MOD_UDSDW 151
#define BSIM_MOD_N00 152
#define BSIM_MOD_N0L 153
#define BSIM_MOD_N0W 154
#define BSIM_MOD_NB0 155
#define BSIM_MOD_NBL 156
#define BSIM_MOD_NBW 157
#define BSIM_MOD_ND0 158
#define BSIM_MOD_NDL 159
#define BSIM_MOD_NDW 160
#define BSIM_MOD_TOX 161
#define BSIM_MOD_TEMP 162
#define BSIM_MOD_VDD 163
#define BSIM_MOD_CGSO 164
#define BSIM_MOD_CGDO 165
#define BSIM_MOD_CGBO 166
#define BSIM_MOD_XPART 167
#define BSIM_MOD_RSH 168
#define BSIM_MOD_JS 169
#define BSIM_MOD_PB 170
#define BSIM_MOD_MJ 171
#define BSIM_MOD_PBSW 172
#define BSIM_MOD_MJSW 173
#define BSIM_MOD_CJ 174
#define BSIM_MOD_CJSW 175
#define BSIM_MOD_DEFWIDTH 176
#define BSIM_MOD_DELLENGTH 177
#define BSIM_MOD_NMOS 178
#define BSIM_MOD_PMOS 179

/* device questions */
#define BSIM_DNODE              201
#define BSIM_GNODE              202
#define BSIM_SNODE              203
#define BSIM_BNODE              204
#define BSIM_DNODEPRIME         205
#define BSIM_SNODEPRIME         206
#define BSIM_VBD                207
#define BSIM_VBS                208
#define BSIM_VGS                209
#define BSIM_VDS                210
#define BSIM_CD         211
#define BSIM_CBS        212
#define BSIM_CBD        213
#define BSIM_GM         214
#define BSIM_GDS        215
#define BSIM_GMBS       216
#define BSIM_GBD        217
#define BSIM_GBS        218
#define BSIM_QB         219
#define BSIM_CQB        220
#define BSIM_QG         221
#define BSIM_CQG        222
#define BSIM_QD         223
#define BSIM_CQD        224
#define BSIM_CGG        225
#define BSIM_CGD        226
#define BSIM_CGS        227
#define BSIM_CBG        228
#define BSIM_CAPBD      231
#define BSIM_CQBD       232
#define BSIM_CAPBS      233
#define BSIM_CQBS       234
#define BSIM_CDG        235
#define BSIM_CDD        236
#define BSIM_CDS        237
#define BSIM_VON        238
#define BSIM_QBS        239
#define BSIM_QBD        240
#define BSIM_SOURCECONDUCT      241
#define BSIM_DRAINCONDUCT       242

/* model questions */

#ifdef __STDC__
extern int BSIMacLoad(GENmodel *,CKTcircuit*);
extern int BSIMask(CKTcircuit *,GENinstance*,int,IFvalue*,IFvalue*);
extern int BSIMconvTest(GENmodel *,CKTcircuit*);
extern int BSIMdelete(GENmodel*,IFuid,GENinstance**);
extern void BSIMdestroy(GENmodel**);
extern void BSIMevaluate(double,double,double,BSIMinstance*,BSIMmodel*,
        double*,double*,double*, double*, double*, double*, double*, 
        double*, double*, double*, double*, double*, double*, double*, 
        double*, double*, double*, double*, CKTcircuit*);
extern int BSIMgetic(GENmodel*,CKTcircuit*);
extern int BSIMload(GENmodel*,CKTcircuit*);
extern int BSIMmAsk(CKTcircuit*,GENmodel *,int, IFvalue*);
extern int BSIMmDelete(GENmodel**,IFuid,GENmodel*);
extern int BSIMmParam(int,IFvalue*,GENmodel*);
extern void BSIMmosCap(CKTcircuit*, double, double, double, double,
        double, double, double, double, double, double, double,
        double, double, double, double, double, double, double*,
        double*, double*, double*, double*, double*, double*, double*,
        double*, double*, double*, double*, double*, double*, double*, 
        double*);
extern int BSIMparam(int,IFvalue*,GENinstance*,IFvalue*);
extern int BSIMpzLoad(GENmodel*,CKTcircuit*,SPcomplex*);
extern int BSIMsetup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int BSIMtemp(GENmodel*,CKTcircuit*);
extern int BSIMtrunc(GENmodel*,CKTcircuit*,double*);
#else /* stdc */
extern int BSIMacLoad();
extern int BSIMask();
extern int BSIMconvTest();
extern int BSIMdelete();
extern void BSIMdestroy();
extern void BSIMevaluate();
extern int BSIMgetic();
extern int BSIMload();
extern int BSIMmAsk();
extern int BSIMmDelete();
extern int BSIMmParam();
extern void BSIMmosCap();
extern int BSIMparam();
extern int BSIMpzLoad();
extern int BSIMsetup();
extern int BSIMtemp();
extern int BSIMtrunc();
#endif /* stdc */

#endif /*BSIM*/
