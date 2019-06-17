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

// Prototype statements for functions found within this file.
interrupt void MainISR(void);
interrupt void OffsetISR(void);


void main(void)
{
    DeviceInit();
    en_driver(0);
    while (EnableFlag==FALSE);
    CpuTimer0Regs.PRD.all =  mSec1;     // A tasks
    CpuTimer1Regs.PRD.all =  mSec50*8;     // B tasks
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
    ChSel[1]=1;     // ChSelect: ADC A1-> Phase A source Current
    ChSel[2]=2;     // ChSelect: ADC A2-> Phase B source Current
    ChSel[3]=3;     // ChSelect: ADC A3-> Phase A Voltage
    ChSel[4]=4; // ChSelect: ADC A4-> Phase B Voltage
    ChSel[5]=5; // ChSelect: ADC A5-> Phase A load Current
    ChSel[6]=6; // ChSelect: ADC A6-> Phase B load Current
    ChSel[7]=7;     // ChSelect: ADC A7-> DC Bus  Voltage
// Initialize ADC module
    ADC_MACRO_INIT(ChSel,TrigSel,ACQPS)
    EALLOW; // This is needed to write to EALLOW protected registers
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
    PLL3P_INIT(50,T,&pll);
    IER |= M_INT3;
// Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
// IDLE loop. Just sit and loop forever:

    for(;;)  //infinite loop
    {
        // State machine entry & exit point
        //===========================================================
        (*Alpha_State_Ptr)();   // jump to an Alpha state (A0,B0,...)
        //===========================================================

    }
} //END MAIN CODE

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
        GpioDataRegs.GPBTOGGLE.bit.GPIO61=1;
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
        //PrintNumber(number_count);
        //number_count++;
        //if(number_count==9999)number_count=0;
       }
    Alpha_State_Ptr = &A0;  // Back to State A0
}

interrupt void MainISR(void)
{
    //GpioDataRegs.GPASET.bit.GPIO12=1;
        cur_load.As=-((AdcMirror.ADCRESULT5)*3.3/4096-offsetC)*3.8;     //Load curr Phase A
        cur_load.Bs=-((AdcMirror.ADCRESULT6)*3.3/4096-offsetD)*3.8;     //Load curr Phase B
        vols.As=-((AdcMirror.ADCRESULT3)*3.3/4096-offsetA)*62;        //Volt Phase A
        vols.Bs=-((AdcMirror.ADCRESULT4)*3.3/4096-offsetB)*62;        //Volt Phase B
        cur_source.As=-((AdcMirror.ADCRESULT1)*3.3/4096-offsetE)*1.9;       //Source curr Phase A
        cur_source.Bs=-((AdcMirror.ADCRESULT2)*3.3/4096-offsetF)*1.9;       //Source curr Phase B
        //VI.Icl = -VI.Ial-VI.Ibl;
        vols.Cs = -vols.As - vols.Bs;
        cur_source.Cs = -cur_source.As - cur_source.Bs;
        cur_load.Cs = -cur_load.As - cur_load.Bs;
//Vdc link
        Vdc=(AdcMirror.ADCRESULT7)*3.3*44/4096;
//FIR Filter
        Vdc = fir(Vdc,VdcDelay,coeffs);
        //Ism=PI_Vdc(Vdc_ref-Vdc);
        vols.Sin = sin(pll.theta[1]);
        vols.Cos = cos(pll.theta[1]);
        ABC_DQ(&vols);
     //--------------------------------------------------------------
        cur_source.Sin = vols.Sin;
        cur_source.Cos = vols.Cos;
        ABC_DQ(&cur_source);
     //--------------------------------------------------------------
        pll.vq[0] = vols.Qs;
        PLL3P(&pll);
        // ------------------------------------------------------------------------------
        //  Connect inputs of the SVGEN_DQ module and call the space-vector gen. macro
        // ------------------------------------------------------------------------------
//      svgen1.Ualpha = ipark1.Alpha;
//      svgen1.Ubeta = ipark1.Beta;
//      SVGENDQ_MACRO(svgen1)
//      pwm1.MfuncC1 = svgen1.Ta;
//      pwm1.MfuncC2 = svgen1.Tb;
//      pwm1.MfuncC3 = svgen1.Tc;
        //PWM_MACRO(1,2,3,pwm1)
        DlogCh1 = (int16)(cur_source.As*1000);
        DlogCh2 = (int16)(cur_source.Bs*1000);
        DlogCh3 = (int16)(cur_load.As*1000);
        DlogCh4 = (int16)(cur_load.Bs*1000);
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
    if (IsrTicker > 25000)
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
