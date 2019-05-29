#include "PeripheralHeaderIncludes.h"
#include "SAHF-Settings.h"
#include "IQmathLib.h"
#include "SAHF.h"
#include <math.h>


//**********************************************************************************************//
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
//**********************************************************************************************//
void sin_cos(void);
// Prototype statements for functions found within this file.
interrupt void MainISR(void);
interrupt void OffsetISR(void);
void DeviceInit();

void HVDMC_Protection(void);

// State Machine function prototypes
//------------------------------------
// Alpha states
void A0(void);  //state A0
void B0(void);  //state B0
void C0(void);  //state C0
float Vd, Vq, alpha, beta, Ism=4, dA, dB, dC, Vbase=30, HB=0.25;
float Valpha, Vbeta, Ialpha, Ibeta, Alpha_grid, Beta_grid ;
float Vpp_grid, Ipp_grid, m, Valpha_svpwm, Vbeta_svpwm, Vpp_svpwm, Vd_dk, Vq_dk;
float Vd_grid , Vq_grid, Id_grid , Iq_grid, Id, Iq;
float k1 = 0.5774, k2 = 1.1547, k3 = 0.5773502, Theta;
float ylf[2]={0,0},Vtheta[2]={0,0}, fo,vq[2]={0,0};
float tempA = 0,tempB = 0, TempCos=0, TempSin=0, e0d =0, e0q =0, WL=6.28;  //WL=2*pi*50*20*10^-3(L = 20mH);
// Variable declarations
void (*Alpha_State_Ptr)(void);  // Base States pointer
float PI_Vdc (float e1);

// Global variables used in this system

Uint16 OffsetFlag=0, dis_pwm=1;
float offsetA=0;
float offsetB=0;
float offsetC=0;
float offsetD=0;
float offsetE=0;
float offsetF=0;
float K1=_IQ(0.998);        //Offset filter coefficient K1: 0.05/(T+0.05);
float K2=_IQ(0.001999); //Offset filter coefficient K2: T/(T+0.05);
float da_1=0, db_1=0,dc_1=0, Lc=0.02, Rc=2, dA_1=0, dA_2=0, dB_1=0, dB_2=0, dC_1=0, dC_2=0;
float32 T = 0.001/ISR_FREQUENCY;    // Samping period (sec), see parameter.h

Uint32 IsrTicker = 0;
Uint16 BackTicker = 0;
Uint16 lsw=0;
Uint16 TripFlagDMC=0;               //PWM trip status

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
float Vdc;
#define FILTER_LEN          11              // filter length
float VaDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ia
float IaDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ia
float VbDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
float IbDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
float VcDelay[FILTER_LEN] = {0,0,0,0,0,0,0,0,0,0,0};    // bo dem cho dong Ib
float IcDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
float VdcDelay[FILTER_LEN]= {0,0,0,0,0,0,0,0,0,0,0}; // bo dem cho dien ap Vdc
//this coefficients get from fdatool of Matlab, Fs = 10khz, Fc = 150Hz, ;length = 11, window = rectangular
// bo loc danh cho dien ap xoay chieu
float coeffs[FILTER_LEN] = {0.0828,0.0876,0.0914,0.0942,0.0959,0.0964,0.0959,0.0942,0.0914,0.0876,0.0828};  // filter coefficients
//this coefficients get from fdatool of Matlab, Fs = 10khz, Fc = 5Hz, ;length = 11, window = rectangular
// bo loc danh cho dien ap mot chieu
/**********************************************************************
doi so la mau du lieu vua moi doc ADC
**********************************************************************/
float fir(float Xn, float *xDelay, float *coeffs)
{
Uint16 i = 0;                               // general purpose
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
unsigned int wisd = 0, wivdc=0;
float ui0isd =0, ui0isq = 0, ui0vdc = 0, Vdcref=70;
float Kpd = 1, Kid =0.1, Kpq = 1, Kiq = 0.1, Kp=1, Ki=0.1, Kpdc = 10, Kidc =1;
float UMAX = 1, UMIN = -1, UdcMAX=1,UdcMIN=-1 ; // Vdc/sqrt(3)
float PI_Vdc (float e1)
{
    float v1 = 0, u1 = 0;
    float up1 = 0,ui1 = 0;
    up1 = Kpdc * e1;
    ui1 = ui0isd + wisd*Kidc*e1*0.0001;
    ui0isd = ui1;
    v1 = up1 + ui1;
    if (v1 > 10)u1 = 10;
    else if (v1 < -10)u1 = -10;
    else u1 = v1;
    if(v1 == u1) wisd = 1; //tich phan da bao hoa
    if(v1 != u1) wisd = 0; // tich phan chua bao hoa
    return u1;
}
float PI_a (float e1)
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
float PI_b (float e1)
{
    float v1 = 0, u1 = 0;
    float up1 = 0,ui1 = 0;
    up1 = Kpd * e1;
    ui1 = ui0isd + wisd*Kid*e1*0.0001;
    ui0isd = ui1;
    v1 = up1 + ui1;
    if (v1 > UMAX)u1 = UMAX;
    else if (v1 < UMIN)u1 = UMIN;
    else u1 = v1;
    if(v1 == u1) wisd = 1; //tich phan da bao hoa
    if(v1 != u1) wisd = 0; // tich phan chua bao hoa
    return u1;
}

float PI_c (float e2)
{
    float v1 = 0, u1 = 0;
    float up1 = 0,ui1 = 0;
    up1 = Kpq * e2;
    ui1 = ui0isq + wisd*Kiq*e2*0.0001;
    ui0isq = ui1;
    v1 = up1 + ui1;
    if (v1 > UMAX)u1 = UMAX;
    else if (v1 < UMIN)u1 = UMIN;
    else u1 = v1;
    if(v1 == u1) wisd = 1; //tich phan da bao hoa
    if(v1 != u1) wisd = 0; // tich phan chua bao hoa
    return u1;
}
void main(void)
{
     DeviceInit();
    CpuTimer0Regs.PRD.all =  mSec1;     // A tasks
    CpuTimer1Regs.PRD.all =  mSec5;     // B tasks
    CpuTimer2Regs.PRD.all =  mSec50*4;  // C tasks
    Alpha_State_Ptr = &A0;
// Initialize PWM module
    pwm1.PeriodMax = SYSTEM_FREQUENCY*1000000*T/2;  // Prescaler X1 (T1), ISR period = T x 1
    pwm1.HalfPerMax=pwm1.PeriodMax/2;
    pwm1.Deadband  = 2.0*SYSTEM_FREQUENCY;          // 120 counts -> 2.0 usec for TBCLK = SYSCLK/1
    PWM_INIT_MACRO(1,2,3,pwm1)
// Initialize DATALOG module
    dlog.iptr1 = &DlogCh1;
    dlog.iptr2 = &DlogCh2;
    dlog.iptr3 = &DlogCh3;
    dlog.iptr4 = &DlogCh4;
    dlog.trig_value = 0x1;
    dlog.size = 0x0C8;
    dlog.prescalar = 5;
    dlog.init(&dlog);

// Initialize ADC for DMC Kit Rev 1.1
    ChSel[0]=1;     // Dummy meas. avoid 1st sample issue Rev0 Picollo*/
    ChSel[1]=1;     // ChSelect: ADC A1-> Phase A Current
    ChSel[2]=2;     // ChSelect: ADC B1-> Phase B Current
    ChSel[3]=3;     // ChSelect: ADC A3-> Phase C Current
    ChSel[4]=4; // ChSelect: ADC B7-> Phase A Voltage
    ChSel[5]=5; // ChSelect: ADC B6-> Phase B Voltage
    ChSel[6]=6; // ChSelect: ADC B4-> Phase C Voltage
    ChSel[7]=7;     // ChSelect: ADC A7-> DC Bus  Voltage
// Initialize ADC module
    ADC_MACRO_INIT(ChSel,TrigSel,ACQPS)
    EALLOW; // This is needed to write to EALLOW protected registers
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
    PieVectTable.EPWM1_INT = &OffsetISR;
    EDIS;
//  Enable PIE group 3 interrupt 1 for EPWM1_INT
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
//  Enable CNT_zero interrupt using EPWM1 Time-base
    EPwm1Regs.ETSEL.bit.INTEN = 1;   // Enable EPWM1INT generation
    EPwm1Regs.ETSEL.bit.INTSEL = 2;  // Enable interrupt period event
    EPwm1Regs.ETPS.bit.INTPRD = 1;   // Generate interrupt on the 1st event
    EPwm1Regs.ETCLR.bit.INT = 1;     // Enable more interrupts
// Enable CPU INT3 for EPWM1_INT:
    IER |= M_INT3;
// Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
// IDLE loop. Just sit and loop forever:
    pwm1.MfuncC1 = 0;
    pwm1.MfuncC2 = 0;
    pwm1.MfuncC3 = 0;
    for(;;)  //infinite loop
    {
        // State machine entry & exit point
        //===========================================================
        (*Alpha_State_Ptr)();   // jump to an Alpha state (A0,B0,...)
        //===========================================================

    }
} //END MAIN CODE

//=================================================================================
//  STATE-MACHINE SEQUENCING AND SYNCRONIZATION FOR SLOW BACKGROUND TASKS
//=================================================================================

//--------------------------------- FRAMEWORK -------------------------------------
void A0(void)
{
    // loop rate synchronizer for A-tasks
    if(CpuTimer0Regs.TCR.bit.TIF == 1)
    {
        CpuTimer0Regs.TCR.bit.TIF = 1;  // clear flag
    }

    Alpha_State_Ptr = &B0;      // Comment out to allow only A tasks
}

void B0(void)
{
    // loop rate synchronizer for B-tasks
    if(CpuTimer1Regs.TCR.bit.TIF == 1)
    {
        CpuTimer1Regs.TCR.bit.TIF = 1;              // clear flag
    }

    Alpha_State_Ptr = &C0;      // Allow C state tasks
}

void C0(void)
{
    // loop rate synchronizer for C-tasks
    if(CpuTimer2Regs.TCR.bit.TIF == 1)
    {
        CpuTimer2Regs.TCR.bit.TIF = 1;              // clear flag
        GpioDataRegs.GPBTOGGLE.bit.GPIO60=1;
    }

    Alpha_State_Ptr = &A0;  // Back to State A0
}

interrupt void MainISR(void)
{
    //GpioDataRegs.GPASET.bit.GPIO12=1;
        VI.Ial=((AdcMirror.ADCRESULT5)*3.3/4096-offsetC)*3.8;     //Load curr Phase A
        VI.Ibl=((AdcMirror.ADCRESULT6)*3.3/4096-offsetD)*3.8;     //Load curr Phase B
        VI.Vas=((AdcMirror.ADCRESULT3)*3.3/4096-offsetA)*62;        //Volt Phase A
        VI.Vbs=((AdcMirror.ADCRESULT4)*3.3/4096-offsetB)*62;        //Volt Phase B
        VI.Ias=((AdcMirror.ADCRESULT1)*3.3/4096-offsetE)*1.9;       //Source curr Phase A
        VI.Ibs=((AdcMirror.ADCRESULT2)*3.3/4096-offsetF)*1.9;       //Source curr Phase B
//Vdc link
        //Vdc=(AdcMirror.ADCRESULT7)*3.3*44/4096;
//FIR Filter
        //Vdc = fir(Vdc,VdcDelay,coeffs);
        //Ism=PI_Vdc(Vdcref-Vdc);
        //VI.Vas = fir(VI.Vas,VaDelay,coeffs);
        //VI.Vbs = fir(VI.Vbs,VbDelay,coeffs);
        //VI.Ial = fir(VI.Ial,IaDelay,coeffs);
        //VI.Ibl = fir(VI.Ibl,IbDelay,coeffs);
        //VI.Iainv = fir(VI.Iainv,VcDelay,coeffs);
        //VI.Ibinv = fir(VI.Ibinv,IcDelay,coeffs);
        VI.Icl = -VI.Ial-VI.Ibl;
        VI.Vcs = -VI.Vas - VI.Vbs;
        VI.Ics = -VI.Ias - VI.Ibs;
//Phase to phase transform phase to ground
/*
        dA_2 = dA_1;dA_1 =dA;
        dB_2 = dB_1;dB_1 =dB;
        dC_2 = dC_1;dC_1 =dC;
        dA=Ism*VI.Vas/Vbase-VI.Ial;
        dB=Ism*VI.Vbs/Vbase-VI.Ibl;
        dC=Ism*VI.Vcs/Vbase-VI.Icl;
        if(Vdc>30){
            dis_pwm=0;
            svgen1.Ta = da_1 + (-Lc*dA_2+(2*Lc+Rc)*dA_1-(Rc+Lc)*dA+VI.Vas/Vbase)/Vdc;da_1=svgen1.Ta;
            svgen1.Tb = db_1 + (-Lc*dB_2+(2*Lc+Rc)*dB_1-(Rc+Lc)*dB+VI.Vbs/Vbase)/Vdc;db_1=svgen1.Tb;
            svgen1.Tc = dc_1 + (-Lc*dC_2+(2*Lc+Rc)*dC_1-(Rc+Lc)*dC+VI.Vcs/Vbase)/Vdc;dc_1=svgen1.Tc;
        }
        else dis_pwm=1;
        //if(VI.Iainv<(dA-HB))svgen1.Ta=-1;
        //else if(VI.Iainv>(dA+HB)) svgen1.Ta=1;
        //if(VI.Ibinv<(dB-HB))svgen1.Tb=-1;
        //else if(VI.Ibinv>(dB+HB)) svgen1.Tb=1;
        //if(VI.Icinv<(dC-HB))svgen1.Tc=-1;
        //else if(VI.Icinv>(dC+HB)) svgen1.Tc=1;
        //svgen1.Ta=PI_a(dA - VI.Iainv);
        //svgen1.Tb=PI_b(dB - VI.Ibinv);
        //svgen1.Tc=PI_c(dC - VI.Icinv);
        if(dis_pwm==1)
            {
            EALLOW;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            EDIS;
            }
        else{
            EALLOW;
            EPwm1Regs.TZCLR.bit.OST = 1;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm3Regs.TZCLR.bit.OST = 1;
            EDIS;
            pwm1.MfuncC1 = svgen1.Ta/3750;
            pwm1.MfuncC2 = svgen1.Tb/3750;
            pwm1.MfuncC3 = svgen1.Tc/3750;
            PWM_MACRO(1,2,3,pwm1)
        }
*/
        PWM_MACRO(1,2,3,pwm1)
        DlogCh1 = (int16)(VI.Ial*1000);
        DlogCh2 = (int16)(VI.Ibl*1000);
        DlogCh3 = (int16)(VI.Vas*1000);
        DlogCh4 = (int16)(VI.Vbs*1000);
        dlog.update(&dlog);
// Enable more interrupts from this timer
    EPwm1Regs.ETCLR.bit.INT = 1;
// Acknowledge interrupt to recieve more interrupts from PIE group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    GpioDataRegs.GPACLEAR.bit.GPIO12=1;
}// MainISR Ends Here

interrupt void OffsetISR(void)
{
// Verifying the ISR
    IsrTicker++;
// DC offset measurement for ADC
    if (IsrTicker>=5000)
        {
            offsetA= K1*offsetA + K2*(AdcMirror.ADCRESULT3)*3.3/4096;           //Volt Phase A offset
            offsetB= K1*offsetB + K2*(AdcMirror.ADCRESULT4)*3.3/4096;           //Volt Phase B offset
            offsetC= K1*offsetC + K2*(AdcMirror.ADCRESULT5)*3.3/4096;           //Load curr Phase B offset
            offsetD= K1*offsetD + K2*(AdcMirror.ADCRESULT6)*3.3/4096;           //Load curr Phase B offset
            offsetE= K1*offsetE + K2*(AdcMirror.ADCRESULT1)*3.3/4096;           //Source curr Phase A offset
            offsetF= K1*offsetF + K2*(AdcMirror.ADCRESULT2)*3.3/4096;            //Source curr Phase B offset
        }
    if (IsrTicker > 50000)
    {
        EALLOW;
        PieVectTable.EPWM1_INT = &MainISR;
        EDIS;
    }
// Enable more interrupts from this timer
    EPwm1Regs.ETCLR.bit.INT = 1;
// Acknowledge interrupt to recieve more interrupts from PIE group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void HVDMC_Protection(void)
{
     EALLOW;
      EPwm1Regs.TZSEL.bit.CBC6=0x1;
      EPwm2Regs.TZSEL.bit.CBC6=0x1;
      EPwm3Regs.TZSEL.bit.CBC6=0x1;

      EPwm1Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT
      EPwm2Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT
      EPwm3Regs.TZSEL.bit.OSHT1   = 1;  //enable TZ1 for OSHT


      EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low

      EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO; // EPWMxA will go low
      EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO; // EPWMxB will go low
      EDIS;
      EPwm1Regs.TZCLR.bit.OST = 1;
      EPwm2Regs.TZCLR.bit.OST = 1;
      EPwm3Regs.TZCLR.bit.OST = 1;

//************************** End of Prot. Conf. ***************************//
}

//===========================================================================
// No more.
//===========================================================================
