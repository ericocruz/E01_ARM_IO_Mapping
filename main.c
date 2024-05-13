// 0.Documentation Section 
// main.c
// Runs on LM4F120 or TM4C123
// C6_InputOutput, Input from PF4, output to PF2 (blue LED)
// Author: Erico Cruz
// Date: Feb 12, 2014

// This script is intended to show how to map an MCU hardware I/Os
// LaunchPad built-in hardware
// SW1 left switch is negative logic PF4 on the Launchpad
// SW2 right switch is negative logic PF0 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define PF4                     (*((volatile unsigned long *)0x40025040))
#define PF3                     (*((volatile unsigned long *)0x40025020))
#define PF2                     (*((volatile unsigned long *)0x40025010))
#define PF1                     (*((volatile unsigned long *)0x40025008))
#define PF0                     (*((volatile unsigned long *)0x40025004))
#define GPIO_PORTF_DR2R_R       (*((volatile unsigned long *)0x40025500))
#define GPIO_PORTF_DR4R_R       (*((volatile unsigned long *)0x40025504))
#define GPIO_PORTF_DR8R_R       (*((volatile unsigned long *)0x40025508))
#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
// 2. Declarations Section
//   Global Variables
unsigned long In; // input from PF4
unsigned long Out; // output to PF2 (blue LED)
//   Function Prototypes
void PortF_Init(void);

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){     							// initialize PF0 and PF4 and make them inputs
  PortF_Init();     							// make PF3-1 out (PF3-1 built-in LEDs)
  while(1){												// (1) infinite loop
    In = GPIO_PORTF_DATA_R&0x10;  // read PF4 into Sw1 and mask bit 5
    In = In>>2;                   // shift into position PF2
		Out = GPIO_PORTF_DATA_R;
		Out = Out&0xFB;								// 11111011
		Out = Out|In;					
    GPIO_PORTF_DATA_R = Out;      // output 
  }
}

// Subroutine to initialize port F pins for input and output
// PF4 is input SW1 and PF2 is output Blue LED
// Inputs: None
// Outputs: None
// Notes: ...
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) enable clock for Port F (bit five 0010 0000)
  delay = SYSCTL_RCGC2_R;           // clock for port F takes 3-5 cycled for the clock to stabilize
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to all port PF4-0 bits (0001 1111)
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) Direction register PF4,PF0 input (0), PF1, PF2 and PF3 output (1) (0000 1110)
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4 (0001 0001)
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4 to PF1
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06
