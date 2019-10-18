/*-------------------------------------------------------------------------------
Next, Include project specific include files.
-------------------------------------------------------------------------------*/
#include "aci_fe.h"             // Include header for the ACIFE object
#include "aci_fe_const.h"       // Include header for the ACIFE_CONST object
#include "aci_se.h"             // Include header for the ACISE object
#include "aci_se_const.h"       // Include header for the ACISE_CONST object


#include "park.h"               // Include header for the PARK object
#include "ipark.h"              // Include header for the IPARK object
#include "pi.h"                 // Include header for the PI object
#include "clarke.h"             // Include header for the CLARKE object
#include "svgen.h"              // Include header for the SVGENDQ object
#include "rampgen.h"            // Include header for the RAMPGEN object
#include "rmp_cntl.h"           // Include header for the RMPCNTL object
#include "volt_calc.h"          // Include header for the PHASEVOLTAGE object
#include "speed_pr.h"           // Include header for the SPEED_MEAS_CAP object
#include "speed_fr.h"           // Include header for the SPEED_MEAS_QEP object


#include "f2833xpwm.h"          // Include header for the PWMGEN object
#include "f2833xpwmdac.h"       // Include header for the PWMDAC object
#include "f2833xqep.h"          // Include header for the QEP object
#include "f2833xileg_vdc.h"     // Include header for the ILEG2DCBUSMEAS object
#include "f2833xcap.h"          // Include header for the CAP object
#include "DSP2833x_EPwm_defines.h" // Include header for PWM defines


#include "dlog4ch-SAHF.h"           // Include header for the DLOG_4CH object
void DeviceInit();
void HVDMC_Protection(void);

// State Machine function prototypes
//------------------------------------
// Alpha states
void A0(void);  //state A0
void B0(void);  //state B0
void C0(void);  //state C0

// Variable declarations
void (*Alpha_State_Ptr)(void);  // Base States pointer

// Default ADC initialization
int ChSel[16]   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int TrigSel[16] = {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
int ACQPS[16]   = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
int DlogCh1 = 0;
int DlogCh2 = 0;
int DlogCh3 = 0;
int DlogCh4 = 0;
float p_in, q_in;
int en_pwm=0;
#define pllQ    _IQ21
#define Qmpy    _IQ21mpy
// Instance a PWM driver instance

void en_driver(int dis_pwm);
void PrintNumber(Uint16 number);
float fir(float Xn, float *xDelay, float *coeffs);

#define L_FILTER  0.01 // Henry
// Global variables used in this system
const Uint16 font[17] = {
    0x3f, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6d, /* 5 */
    0x7d, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6f, /* 9 */
    0x00, /* (space) */
    0x77, /* A */
    0x7c, /* B */
    0x39, /* C */
    0x5e, /* D */
    0x79, /* E */
    0x71 /* F */
};
volatile Uint16 buffer[8]={10,10,10,10,10,10,13,11};
//Offset
float offsetA=0, offsetB=0, offsetC=0, offsetD=0, offsetE=0, offsetF=0;
float K1=_IQ(0.998);        //Offset filter coefficient K1: 0.05/(T+0.05);
float K2=_IQ(0.001999); //Offset filter coefficient K2: T/(T+0.05);
float32 T = 0.001/ISR_FREQUENCY;    // Samping period (sec), see parameter.h
Uint32 IsrTicker = 0, count_138=0;
volatile Uint16 EnableFlag = FALSE, number_count=0;
//Filter
typedef struct {
    _iq  Vas;
    _iq  Vbs;
    _iq  Vcs;
    _iq  Vans;
    _iq  Vbns;
    _iq  Vcns;
    _iq  Ial;
    _iq  Ibl;
    _iq  Icl;
    _iq  Iainv;
    _iq  Ibinv;
    _iq  Icinv;
    _iq  Ias;
    _iq  Ibs;
    _iq  Ics;
} ADC;
ADC VI = {0, 0, 0, 0, 0, 0, 0, 0, 0};
#define FILTER_LEN          11              // filter length
_iq VaDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ia
_iq IaDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ia
_iq VbDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
_iq IbDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
_iq Il_aDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
_iq Il_bDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
_iq IcDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
_iq VdcDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
_iq IdDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
_iq IqDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
_iq VdDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
_iq VqDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
//this coefficients get from fdatool of Matlab, Fs = 10khz, Fc = 150Hz, ;length = 11, window = rectangular
_iq coeffs[FILTER_LEN] = {0.0828,0.0876,0.0914,0.0942,0.0959,0.0964,0.0959,0.0942,0.0914,0.0876,0.0828};  // filter coefficients

typedef struct {
    _iq As;
    _iq Bs;
    _iq Cs;
    _iq max;
    _iq min;
    _iq offset;
    _iq Alpha;
    _iq Beta;
    _iq Ds;
    _iq Qs;
    _iq Sin;
    _iq Cos;
    _iq Ka;
    _iq Kb;
    _iq Kc;
} DATA3P;
#define DATA3P_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
unsigned int wivdc=0;
float ui0vdc = 0, Vdc_ref=70, Vdc=0, Ism=3.0;
float Kp=1, Ki=0.1;
float UdcMAX=6,UdcMIN=-6 ; // Vdc/sqrt(3)
typedef struct { int32  vq[2];
                 int32  ylf[2];
                 int32  fo;
                 int32  fn;
                 int32  theta[2];
                 int32  dT;
                 int32  B0;
                 int32  B1;
                }PLL3P_OBJ;
#define PLL3P_DEFAULTS {0,0,0,0,0,0,0,0}

void en_driver(int dis_pwm){
if(dis_pwm==0)
       {
       EALLOW;
       EPwm1Regs.TZFRC.bit.OST = 1;
       EPwm2Regs.TZFRC.bit.OST = 1;
       EPwm3Regs.TZFRC.bit.OST = 1;
       EDIS;
       }
   else if(dis_pwm==1){
       EALLOW;
       EPwm1Regs.TZCLR.bit.OST = 1;
       EPwm2Regs.TZCLR.bit.OST = 1;
       EPwm3Regs.TZCLR.bit.OST = 1;
       EDIS;
}
}
void ABC_DQ(DATA3P *v);
void DQ_ABC(DATA3P *v);
void PLL3P_INIT(int fgrid, long deltaT, PLL3P_OBJ *v);
void PLL3P(PLL3P_OBJ *v);
void ABC_DQ(DATA3P *v)
{
    float k1 = 0.6666667, k2 = 0.3333333, k3 = 0.5773502;
    v->Alpha = k1*v->As - k2*v->Bs - k2*v->Cs;
    v->Beta  = k3*(v->Bs - v->Cs);
    v->Ds =   v->Alpha*v->Cos + v->Beta*v->Sin;
    v->Qs = - v->Alpha*v->Sin + v->Beta*v->Cos;
}
typedef struct {  _iq  Ref;
                  _iq  Fbk;
                  _iq  Out;
                  _iq  Kp;
                  _iq  Ki;
                  _iq  Umax;
                  _iq  Umin;
                  _iq  up;
                  _iq  ui;
                  _iq  v1;
                  _iq  i1;
                  _iq  w1;
                } PIDATA;
void PI_CONTROLER(PIDATA *v);
#define PIDATA_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0}
void PI_CONTROLER(PIDATA *v)
{
    v->up = _IQmpy(v->Kp, (v->Ref - v->Fbk));
    v->ui = (v->Out == v->v1)?(_IQmpy(v->Ki, v->up)+ v->i1) : v->i1;
    v->i1 = v->ui;
    v->v1 = v->up + v->ui;
    v->Out= _IQsat(v->v1, v->Umax, v->Umin);
    //v->w1 = (v->Out == v->v1) ? _IQ(1.0) : _IQ(0.0);
}
void DQ_ABC(DATA3P *v)
{
    float k1 = 0.5, k2 = 0.8660254038;
    v->Alpha = v->Ds*v->Cos - v->Qs*v->Sin;
    v->Beta  = v->Ds*v->Sin + v->Qs*v->Cos;
    v->As = v->Alpha;
    v->Bs = - k1*v->Alpha + k2*v->Beta;
    v->Cs = - v->As - v->Bs;
}

float fir(float Xn, float *xDelay, float *coeffs)
{
Uint16 i = 0;
float y = 0;                                    // result
//cap nhat mau moi nhat
*(xDelay + 0) = Xn;
//tinh ngo ra
for (i = 0; i< FILTER_LEN; i++) y += *(coeffs + i) * *(xDelay + i);
//cap nhat du lieu vao bo dem
for (i = FILTER_LEN-1; i > 0; i--)  *(xDelay + i) = *(xDelay + i - 1);

/*** Finish up ***/
    return(y);
}
void PrintNumber(Uint16 number)
{
    // Check max and min
    if (number > 9999)number = 0;
    // Convert integer to bcd digits
    buffer[3] = number / 1000;
    if(buffer[3]==0)buffer[3]=10;
    buffer[2] = number % 1000 / 100;
    if(buffer[2]==0){if(buffer[3]==0)buffer[2]=10;}
    buffer[1] = number % 100 / 10;
    if(buffer[1]==0){if(buffer[2]==0){if(buffer[3]==0)buffer[1]=10;}}
    buffer[0] = number %10;
}
//===========================================================================
// No more.
//===========================================================================
