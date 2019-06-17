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

#if (DSP2803x_DEVICE_H==1)
#include "f2803xileg_vdc.h"     // Include header for the ILEG2DCBUSMEAS object
#include "f2803xpwm.h"          // Include header for the PWMGEN object
#include "f2803xpwmdac.h"       // Include header for the PWMDAC object
#include "f2803xqep.h"          // Include header for the QEP object
#include "f2803xcap.h"          // Include header for the CAP object
#include "DSP2803x_EPwm_defines.h" // Include header for PWM defines
#endif

#if (DSP2833x_DEVICE_H==1)
#include "f2833xpwm.h"          // Include header for the PWMGEN object
#include "f2833xpwmdac.h"       // Include header for the PWMDAC object
#include "f2833xqep.h"          // Include header for the QEP object
#include "f2833xileg_vdc.h"     // Include header for the ILEG2DCBUSMEAS object
#include "f2833xcap.h"          // Include header for the CAP object
#include "DSP2833x_EPwm_defines.h" // Include header for PWM defines
#endif


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
int16 DlogCh1 = 0;
int16 DlogCh2 = 0;
int16 DlogCh3 = 0;
int16 DlogCh4 = 0;

// Instance a PWM driver instance
PWMGEN pwm1 = PWMGEN_DEFAULTS;
SVGEN svgen1 = SVGEN_DEFAULTS;
DLOG_4CH dlog = DLOG_4CH_DEFAULTS;
void en_driver(int dis_pwm);
void PrintNumber(Uint16 number);
float fir(float Xn, float *xDelay, float *coeffs);
#define pllQ    _IQ21
#define Qmpy    _IQ21mpy
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
typedef struct {  float  Vas;
float  Vbs;
float  Vcs;
float  Vans;
float  Vbns;
float  Vcns;
float  Ial;
float  Ibl;
float  Icl;
float  Iainv;
float  Ibinv;
float  Icinv;
float  Ias;
float  Ibs;
float  Ics;
} ADC;
ADC VI = {0, 0, 0, 0, 0, 0, 0, 0, 0};
#define FILTER_LEN          11              // filter length
float VaDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ia
float IaDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ia
float VbDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
float IbDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
float VcDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
float IcDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
float VdcDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
//this coefficients get from fdatool of Matlab, Fs = 10khz, Fc = 150Hz, ;length = 11, window = rectangular
float coeffs[FILTER_LEN] = {0.0828,0.0876,0.0914,0.0942,0.0959,0.0964,0.0959,0.0942,0.0914,0.0876,0.0828};  // filter coefficients
//*********** Structure Definition ********//
typedef struct{
float32 B1_lf;
float32 B0_lf;
float32 A1_lf;
}SPLL_3ph_SRF_F_LPF_COEFF;
typedef struct{
float32 v_q[2];
float32 ylf[2];
float32 fo; // output frequency of PLL
float32 fn; //nominal frequency
float32 theta[2];
float32 delta_T;
SPLL_3ph_SRF_F_LPF_COEFF lpf_coeff;
}SPLL_3ph_SRF_F;
typedef struct { float As;
float Bs;
float Cs;
float max;
float min;
float offset;
float Alpha;
float Beta;
float Ds;
float Qs;
float Sin;
float Cos;
float Ka;
float Kb;
float Kc;
                } DATA3P;
#define DATA3P_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
unsigned int wivdc=0;
float ui0vdc = 0, Vdc_ref=70, Vdc=0, Ism=3.0;
float Kp=1, Ki=0.1;
float UdcMAX=6,UdcMIN=-6 ; // Vdc/sqrt(3)
typedef struct { float  vq[2];
float  ylf[2];
float  fo;
float  fn;
float  theta[2];
float  dT;
float  B0;
float  B1;
}PLL3P_OBJ;
#define PLL3P_DEFAULTS {0,0,0,0,0,0,0,0}
DATA3P vref = DATA3P_DEFAULTS;
DATA3P cur_load = DATA3P_DEFAULTS;
DATA3P cur_source = DATA3P_DEFAULTS;
DATA3P vols = DATA3P_DEFAULTS;
PLL3P_OBJ pll = PLL3P_DEFAULTS;
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
void PLL3P_INIT(float fgrid, float deltaT, PLL3P_OBJ *v);
void PLL3P(PLL3P_OBJ *v);
void ABC_DQ(DATA3P *v)
{
    float k1 = 0.6666667, k2 = 0.3333333, k3 = 0.5773502;
    v->Alpha = k1*v->As - k2*v->Bs - k2*v->Cs;
    v->Beta  = k3*(v->Bs - v->Cs);
    v->Ds =   v->Alpha*v->Cos + v->Beta*v->Sin;
    v->Qs = - v->Alpha*v->Sin + v->Beta*v->Cos;
}
float PI_Vdc (float e1)
{
    float v1 = 0, u1 = 0;
    float up1 = 0,ui1 = 0;
    up1 = Kp * e1;
    ui1 = ui0vdc + wivdc*Ki*e1*0.0001;
    ui0vdc = ui1;
    v1 = up1 + ui1;
    if (v1 > UdcMAX)u1 = UdcMAX;
    else if (v1 < UdcMIN)u1 = UdcMIN;
    else u1 = v1;
    if(v1 == u1) wivdc = 1; //tich phan da bao hoa
    if(v1 != u1) wivdc = 0; // tich phan chua bao hoa
    return u1;
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
void PLL3P_INIT(float fgrid, float deltaT, PLL3P_OBJ *v)
{
    v->vq[0]  = 0.0;
    v->vq[1]  = 0.0;
    v->ylf[0] = 0.0;
    v->ylf[1] = 0.0;

    v->fo = 0.0;
    v->fn = fgrid;

    v->theta[0] = 0.0;
    v->theta[1] = 0.0;

    v->B0 = 166.9743;
    v->B1 = -166.266;

    v->dT = deltaT;
}

void PLL3P(PLL3P_OBJ *v)
{
    v->ylf[0] = v->ylf[1] + v->B0*v->vq[0] + v->B1*v->vq[1];
    v->ylf[1] = v->ylf[0];
    v->vq[1] = v->vq[0];
    v->ylf[0] = (v->ylf[0]>200)?200:v->ylf[0];
    v->fo = v->fn + v->ylf[0];
    v->theta[0] = v->theta[1] + v->fo*v->dT;
    v->theta[0] = (v->theta[0]>1)?(v->theta[0] - 1):v->theta[0];
    v->theta[1] = v->theta[0];
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
