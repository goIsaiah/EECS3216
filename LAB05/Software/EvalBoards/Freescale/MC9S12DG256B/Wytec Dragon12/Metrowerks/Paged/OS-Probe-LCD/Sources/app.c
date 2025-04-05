/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                         (c) Copyright 1998-2003, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*                                          Sample code
*                                          MC9S12DP256B 
*                                       Wytec Dragon12 EVB
*
* File : app.c
* By   : Eric Shufro
*********************************************************************************************************
*/

#include    <includes.h>


/*
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/
   
   
/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

    OS_STK        AppStartTaskStk[APP_TASK_START_STK_SIZE];
    OS_STK        LCD_TestTaskStk[LCD_TASK_STK_SIZE];
    OS_STK        SevenSegTestTaskStk[SEVEN_SEG_TEST_TASK_STK_SIZE];
    OS_STK        KeypadRdTaskStk[KEYPAD_RD_TASK_STK_SIZE];    
    			 
    OS_FLAG_GRP  *keypadEnFlagGrp;    


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  AppTaskCreate(void);
static  void  LCD_TestTask(void *p_arg);
static  void  SevenSegTestTask(void *p_arg);
static  void  KeypadRdTask(void *p_arg);


#if (uC_PROBE_OS_PLUGIN > 0) || (uC_PROBE_COM_MODULE > 0)
extern  void  AppProbeInit(void);
#endif


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary 68HC12 and C initialization.
* Arguments   : none
*********************************************************************************************************
*/

void  main (void)
{
    INT8U   err;


    OSInit();                                                           /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreateExt(AppStartTask,
                    (void *)0,
                    (OS_STK *)&AppStartTaskStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppStartTaskStk[0],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);

    OSStart();                                                          /* Start multitasking (i.e. give control to uC/OS-II)       */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*				3) After this created from main(), it runs and initializes additional application
*                  modules and tasks. Rather than deleting the task, it is simply suspended
*                  periodically. This tasks body could be used for additional work if desired.
*********************************************************************************************************
*/

static  void  AppStartTask (void *p_arg)
{		
   (void)p_arg;
   		  
    BSP_Init();                                                         /* Initialize the ticker, and other BSP related functions   */

#if OS_TASK_STAT_EN > 0
    OSStatInit();                                                       /* Start stats task                                         */
#endif

#if (uC_PROBE_OS_PLUGIN > 0) || (uC_PROBE_COM_MODULE > 0)
    AppProbeInit();                                                     /* Initialize uC/Probe modules                              */
#endif
    
    AppTaskCreate();                                                    /* Create additional tasks using this user defined function */
    
    while (TRUE) {                                                      /* Task body, always written as an infinite loop            */
        OSTimeDlyHMSM(0, 0, 5, 0);                                      /* Delay the task                                           */
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                     CREATE APPLICATION TASKS
*
* Description : This function demonstrates how to create a new application task. 
* 
* Notes:        1) Each task should be a unique function prototypes as 
*                  static  void  mytaskname (void *p_arg). 
*               2) Additionally, each task should contain an infinite loop and call at least one
*                  OS resource on each pass of the loop. An OS resource may be a call to OSTimeDly(),
*                  OSTimeDlyHMSM(), or one of the message box, semaphore or other OS handled resource.
*               3) Each task must have its own stack. Be sure that the stack is declared large
*                  enough or the entire system may crash or experience erradic results if your stack
*                  grows and overwrites other variables in memory.
*               
* Arguments   : none
* Notes       : none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    INT8U  err;

    
    OSTaskCreateExt(LCD_TestTask,
                    (void *)0,
                    (OS_STK *)&LCD_TestTaskStk[LCD_TASK_STK_SIZE-1],
                    LCD_TEST_TASK_PRIO,
                    LCD_TEST_TASK_PRIO,
                    (OS_STK *)&LCD_TestTaskStk[0],
                    LCD_TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    OSTaskNameSet(LCD_TEST_TASK_PRIO, "LCD Test Task", &err);    

    OSTaskCreateExt(SevenSegTestTask,
                    (void *)0,
                    (OS_STK *)&SevenSegTestTaskStk[SEVEN_SEG_TEST_TASK_STK_SIZE-1],
                    SEVEN_SEG_TEST_TASK_PRIO,
                    SEVEN_SEG_TEST_TASK_PRIO,
                    (OS_STK *)&SevenSegTestTaskStk[0],
                    SEVEN_SEG_TEST_TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    OSTaskNameSet(SEVEN_SEG_TEST_TASK_PRIO, "SevenSegTest Task", &err);          

    OSTaskCreateExt(KeypadRdTask,
                    (void *)0,
                    (OS_STK *)&KeypadRdTaskStk[KEYPAD_RD_TASK_STK_SIZE-1],
                    KEYPAD_RD_TASK_PRIO,
                    KEYPAD_RD_TASK_PRIO,
                    (OS_STK *)&KeypadRdTaskStk[0],
                    KEYPAD_RD_TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
    OSTaskNameSet(KEYPAD_RD_TASK_PRIO, "KeypadRd Task", &err);          
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                             SevenSegWriteTask
*
* Description: This task displays messages on the Dragon12 (16x2) LCD screen and is
*              responsible for initializing the LCD hardware. Care MUST be taken to
*              ensure that the LCD hardware is initialized before other tasks
*              attempt to write to it. If necessary the DispInit() function
*              may be called from the start task or bsp_init().
*********************************************************************************************************
*/

static  void  LCD_TestTask (void *p_arg)
{
       CPU_INT08S  i;
       CPU_INT08U  err;
                                                                        /* Keypad Enabled Message                                   */
const  CPU_INT08U  KeypadEnStr[18]    = {"Keypad Enabled"};                     

                                                                        /* Keypad Disabled Message                                  */                       
const  CPU_INT08U  KeypadDisStr[18]   = {"Keypad Disabled"};                    

                                                                        /* Power On Welcome Message. three seperate msgs / rows     */
const  CPU_INT08U  WelcomeStr[6][18]  = {"Welcome to the", "Dragon 12 EVB.  ",                                                                          
                                         "This demo runs", "Micrium uC/OS-II",  
                                         "on a 48 MHz   ", "MC9S12DG256B CPU"}; 
    
                                                                        /* Define a message to scroll on the LCDs top line          */
const  CPU_INT08U  aboutStr[]         = {"Did you know that uC/OS-II can "
                                         "provide multi-tasking and "
                                         "real-time services to your "
                                         "embedded applications? In fact, "
                                         "uC/OS-II provides services "
                                         "such as task delays, "
                                         "semaphores, message mailboxes, "  
                                         "timers, event flags, memory "		
                                         "management, mutexes, queues, "
                                         "and much more! "
                                         };
                                         
   CPU_INT08U  *aboutStrPtr;                                            /* Declare a pointer to aboutStr, used for scroll effect    */
    
                                   
                                  

   (void)p_arg;
                   
    DispInit(2, 16);                                                    /* Initialize uC/LCD for a 2 row by 16 column display       */

             
    while (DEF_TRUE) {                                                  /* All tasks bodies include an infinite loop                */           
        DispClrScr();                                                   /* Start by clearing the screen                             */

        for (i = 0; i < 6; i+=2) {										/* With the Keypad task suspended, both LCD rows are avail. */
            DispStr(0, 0, WelcomeStr[i]);                               /* Display row 0 of Welcome Message i                       */
            DispStr(1, 0, WelcomeStr[i+1]);                             /* Display row 1 of Welcome Message i                       */        
            OSTimeDlyHMSM(0, 0, 2, 0);                                  /* Delay between updating the message                       */
        }        

        DispClrLine(1);                                                 /* Clear LCD ROW 1 before unblocking the keypad task        */
                
        OSFlagPost(keypadEnFlagGrp, 0x01, OS_FLAG_SET, &err);           /* Set flag bit 0 of the keypad enable flag group           */
                                                                        /* This will unblock the keypad task which will use the     */
                                                                        /* bottom row of the LCD while not disabled                 */
                                                                            
        while (err != OS_NO_ERR) {                                      /* If a flag posting error occured,                         */
            OSTimeDlyHMSM(0, 0, 1, 0);                                  /* delay and try again until NO ERROR is returned           */
            OSFlagPost(keypadEnFlagGrp, 0x01, OS_FLAG_SET, &err);       /* Set flag bit 0 of the keypad enable flag group           */
        }


        DispClrLine(0);                                                 /* Clear line 0 of the LCD                                  */
        for (i = 0; i < 3; i++) {                                       /* Blink the Keypad Enabled String 3 times                  */
            DispStr(0, 0, KeypadEnStr);                                 /* Display the Keypad Enabled message                       */     
            OSTimeDlyHMSM(0, 0, 0,500);                                 /* Show the message for a while                             */
            DispClrLine(0);                                             /* Next clear line 0 of the LCD                             */
            OSTimeDlyHMSM(0, 0, 0,500);                                 /* Lastly, wait a bit before repeating the message          */
        }

        aboutStrPtr = aboutStr;                                         /* Point to the start of the about message string           */

																		/* Start scrolling from right to left. This involves        */
        																/* starting from the last column and working toward the     */
                                                                        /* first column until the screen fills up. The remaining    */
                                                                        /* characters may start from position 0.                    */
        for (i = 15; i >= 0; i--) {										/* For the first 16 characters...                           */
            if (*aboutStrPtr != '\0') {						            /* If we have not reached the end of the string             */
                DispStr(0, i, aboutStrPtr);                             /* Display (col i - distance to end of lcd line) # of chars */
                OSTimeDlyHMSM(0, 0, 0, 100);                            /* Delay before updating the screen with the shifted msg    */             
            }
        }

        while ((aboutStr + sizeof(aboutStr) - aboutStrPtr) >  16) {     /* While there are greater than 16 chars left to display    */
            DispStr(0, 0, aboutStrPtr++);                               /* Display 16 chars and increment (shift) message by 1 char */
            OSTimeDlyHMSM(0, 0, 0, 100);                                /* Delay before displaying the shifted message              */
        }

        for (i = 15; i >= 0; i--) {										/* For the last 16 characters                               */
            if (*aboutStrPtr != '\0') {						            /* If we have not reached the end of the string             */
                DispStr(0, 0, aboutStrPtr++);                           /* Display (col i - distance to end of lcd line) # of chars */
                DispChar(0, i, ' ');                                    /* Leave spaces chars behind as the str scrolls off screen  */
                OSTimeDlyHMSM(0, 0, 0, 100);                            /* Delay before updating the screen with the shifted msg    */             
            }
        }
                
        DispClrLine(1);                                                 /* Clear LCD ROW 1 before blocking the keypad task          */
        
        OSFlagPost(keypadEnFlagGrp, 0x01, OS_FLAG_CLR, &err);           /* Disable the keypad task by clearing its event flag 0     */

        while (err != OS_NO_ERR) {                                      /* If an error occured (perhaps the flag group is not       */
            OSTimeDlyHMSM(0, 0, 1, 0);                                  /* initialized yet, then delay and try again until no error */
            OSFlagPost(keypadEnFlagGrp, 0x01, OS_FLAG_CLR, &err);       /* Clear flag bit 0 of the keypad enable flag group         */
        }        
        
        DispClrScr();                                                   /* Clear the screen                                         */
        for (i = 0; i < 3; i++) {                                       /* Blink the Keypad Disabled String 3 times                 */
            DispStr(0, 0, KeypadDisStr);                                /* Display the Keypad Disabled message                      */     
            OSTimeDlyHMSM(0, 0, 0,500);                                 /* Show the message for a while                             */
            DispClrLine(0);                                             /* Next clear line 0 of the LCD                             */
            OSTimeDlyHMSM(0, 0, 0,500);                                 /* Lastly, wait a bit before repeating the message          */
        }          
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                             SevenSegWriteTask
*
* Description: This task displays messages on the Dragon12 (16x2) LCD screen
*********************************************************************************************************
*/

static  void  SevenSegTestTask (void *p_arg)
{
    CPU_INT16U  num;


   (void)p_arg;
   
    SevenSegDisp_Init();	                                            /* Initialize the 7-Seg I/O and periodic refresh interrupt  */
    
    num = 0;
        
    while (DEF_TRUE) {                                                  /* All tasks bodies include an infinite loop                */   
        SevenSegWrite(num);                                             /* Output the value to the screen                           */        
        num = ((num + 1) % 10000);                                      /* Increment the # being displayed, wrap after 9,999        */
        OSTimeDlyHMSM(0, 0, 0, 10);                                     /* Delay the task for 50ms and repeat the process           */
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                             Keypad Read Task
*
* Description: This task periodically reads the Wytec Dragon12 EVB keyapd and
*              displays the value on the bottom row of the LCD screen.
*********************************************************************************************************
*/

static  void  KeypadRdTask (void *p_arg)
{
    CPU_INT08U  key;
    CPU_INT08U  out_str[17];
    CPU_INT08U  key_map[] = {'1', '2', '3', 'A',
                             '4', '5', '6', 'B', 
                             '7', '8', '9', 'C',
                             '*', '0', '#', 'D'
                             };
    CPU_INT08U  err;
        

   (void)p_arg;
       
    KeypadInitPort();                                                   /* Initialize the keypad hardware                           */
    
    keypadEnFlagGrp = OSFlagCreate(0, &err);                            /* Create an event flag group. All flags initialized to 0   */
        
    while (err != OS_NO_ERR) {                                          /* If an error code was returned, loop until successful     */
        OSTimeDlyHMSM(0, 0, 1, 0);				                        /* Delay for 1 second, wait for resources to be freed       */
        keypadEnFlagGrp = OSFlagCreate(0, &err);                        /* Try to create the flag group again                       */
    }
    
    OSFlagPend(keypadEnFlagGrp, 0x01, OS_FLAG_WAIT_SET_ALL, 0, &err);   /* Wait until bit 1 of the flag group to become set         */
                                                                        /* The goal is to prevent this task from accessing the      */
                                                                        /* bottom row of the LCD until the LCD task has finished    */
                                                                        /* displaying its introduction message. This message will   */
                                                                        /* require both lines of the LCD, so we must wait to use it */
            
    DispClrLine(1);                                                     /* Clear the bottom line of the 2 line display              */
                
    while (DEF_TRUE) {                                                  /* All tasks bodies include an infinite loop                */   
        OSFlagPend(keypadEnFlagGrp, 0x01, OS_FLAG_WAIT_SET_ALL,0,&err); /* Suspend the task if flag 0 has been cleared              */
        key = KeypadReadPort();                                         /* Scan the keypad. Returns 0-15 or 0xFF if nothing pushed  */
        if (key == 0xFF) {            
            err = sprintf(out_str, "Keypad is IDLE");                   /* Write "You Pressed" and the key number 'n' to a string   */      
        } else {
            err = sprintf(out_str, "You Pressed: %c", key_map[key]);    /* Write "You Pressed" and the key number 'n' to a string   */      
        }
        
        DispStr(1, 0, out_str);                                         /* Display the keypad message on row 1 of the LCD screen    */        
        OSTimeDlyHMSM(0, 0, 0,100);                                     /* Read the keypad every 100ms                              */
    }
}

// You may find these codes useful: 

/**
 * power up ATD and enable PAD04 pin
 */
static void atd_init(void);
static void atd_init(void)
{
	ATD0CTL2 = 0x80;     /* Power up A/D, no interrupts */
	ATD0CTL3 = 0x00;     /* Do eight conversions */
	//ATD0CTL4 = 0x85;   /* 8-bit mode */
	ATD0CTL4 = 0x05;     /* 10-bit mode */
	ATD0CTL5 = 0xA4;     /* 1 0 1 0 0 1 0 0
            							| | | |   \___/
						            	| | | |     |
							            | | | |     \__ Bit 4 of Port AD --> PAD04
							            | | | \________ MULT = 0 => one channel only
							            | | \__________ Scan = 1 => continuous conversion
							            | \____________ DSGN = 0 => unsigned
							            \______________ DJM = 1 => right justified */
}



/************************************ PWM STARTS ***********************************/

#define PORTIO_8		*(volatile unsigned char *)
#define PORTIO_16		*(volatile unsigned short int *)

#define IO_BASE	0

#define     PWMCNT01_16BIT      PORTIO_16(IO_BASE + 0xac)     /* pwm channel 0,1 counter, 16bit */
#define     PWMCNT23_16BIT      PORTIO_16(IO_BASE + 0xae)     /* pwm channel 2,3 counter, 16bit */
#define     PWMCNT45_16BIT      PORTIO_16(IO_BASE + 0xb0)     /* pwm channel 4,5 counter, 16bit */
#define     PWMCNT67_16BIT      PORTIO_16(IO_BASE + 0xb2)     /* pwm channel 6,7 counter, 16bit */
#define     PWMPER01_16BIT      PORTIO_16(IO_BASE + 0xb4)     /* pwm channel 0,1 period, 16bit */
#define     PWMPER23_16BIT      PORTIO_16(IO_BASE + 0xb6)     /* pwm channel 2,3 period, 16bit */
#define     PWMPER45_16BIT      PORTIO_16(IO_BASE + 0xb8)     /* pwm channel 4,5 period, 16bit */
#define     PWMPER67_16BIT      PORTIO_16(IO_BASE + 0xba)     /* pwm channel 6,7 period, 16bit */
#define     PWMDTY01_16BIT      PORTIO_16(IO_BASE + 0xbc)     /* pwm channel 0,1 duty cycle, 16bit */
#define     PWMDTY23_16BIT      PORTIO_16(IO_BASE + 0xbe)     /* pwm channel 2,3 duty cycle, 16bit */
#define     PWMDTY45_16BIT      PORTIO_16(IO_BASE + 0xc0)     /* pwm channel 4,5 duty cycle, 16bit */
#define     PWMDTY67_16BIT      PORTIO_16(IO_BASE + 0xc2)     /* pwm channel 6,7 duty cycle, 16bit */


/*
 * below are some temp. vars. to save the values we get from PC or KEYPAD ...
 */
char pwm_mode, pwm_channel_number, pwm_polarity, pwm_alignment, pwm_is_using_scaled_clock, pwm_main_clock_divider;
unsigned int pwm_scale_clock_divider, pwm_period_reg_value, pwm_duty_reg_value;

/*
 * bit value of enable and disable flags in PWME
 */
#define ENABLE 1
#define DISABLE 0

/*
 * bit value of polarity
 * polarity can be high at start of the pulse or can be low
 */
#define PWM_POLARITY_IS_HIGH_AT_BEGINNING_OF_THE_PULSE 1
#define PWM_POLARITY_IS_LOW_AT_BEGINNING_OF_THE_PULSE 0

/*
 * alignment: pulse can be left or center aligned 
 */
#define PWM_ALIGNMENT_CENTER_ALIGN 1
#define PWM_ALIGNMENT_LEFT_ALIGN 0


/*
 * 16bit or 8bit of opration
 */
#define PWM_8BIT_OPERATION_MODE 8
#define PWM_16BIT_OPERATION_MODE 16

/*
 * channels
 */
#define PWM_8BIT_CHANNEL_0 0
#define PWM_8BIT_CHANNEL_1 1
#define PWM_8BIT_CHANNEL_2 2
#define PWM_8BIT_CHANNEL_3 3
#define PWM_8BIT_CHANNEL_4 4
#define PWM_8BIT_CHANNEL_5 5
#define PWM_8BIT_CHANNEL_6 6
#define PWM_8BIT_CHANNEL_7 7
#define PWM_16BIT_CHANNEL_0 0 //combined 8bit channel1 and 0, CH1 is used for output pin
#define PWM_16BIT_CHANNEL_1 1 //combined 8bit channel3 and 2, CH3 is used for output pin
#define PWM_16BIT_CHANNEL_2 2 //combined 8bit channel5 and 5, CH5 is used for output pin
#define PWM_16BIT_CHANNEL_3 3 //combined 8bit channel7 and 6, CH7 is used for output pin


/*
 * we have two clock source ... A and B
 * some channels use A and some use B
 */
#define PWM_IS_USING_CLOCK_A 0
#define PWM_IS_USING_CLOCK_B 1

/*
 * are we going to use scale our clock source further or not
 */
#define PWM_IS_USING_SCALED_CLOCK 1
#define PWM_IS_NOT_USING_SCALED_CLOCK 0



/**
 * cannel: 8 channel in 8bit or 4 channel in 16 bit
 * status: enable, disable ...
 * mode: 8bit mode or 16bit
 * polarity: hight at start or low at start
 * alignment: left or center aligned
 * is_using_scaled_clock: SA or SB ...  
 */
typedef struct pwm_struct
  {
    char channel;
    char status;
    char mode;
    char polarity;
    char alignment;
    char is_using_scaled_clock;
    char main_clock_divider_reg_value;
    int scaled_clock_divider_reg_value;
    unsigned int duty_reg_value;
    unsigned int period_reg_value;
  }
PWM;

/*
 * We can precalculate and preset the setting for diffrenet Freq. 
 * and whenever we want just call the related index in batch to simply run that
 * settings ...
 * 
 * here we can preload 50 diffrent settings for channels.
 */  
PWM pwm_batch[50];


/**
 * helps us to see which of clock A or B this givven channle is using ...
 * channel: 0-7 for 8bit and 0-3 for 16bit modes
 * mode: is 16bit or 8bit
 */
char pwm_is_using_which_clock(char channel, char mode);
char pwm_is_using_which_clock(char channel, char mode)
{
  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      if (channel == PWM_8BIT_CHANNEL_0 ||
          channel == PWM_8BIT_CHANNEL_1 ||
          channel == PWM_8BIT_CHANNEL_4 ||
          channel == PWM_8BIT_CHANNEL_5
         )
        return PWM_IS_USING_CLOCK_A;
      else
        return PWM_IS_USING_CLOCK_B;
    }
  else // mode == PWM_16BIT_OPERATION_MODE
    {
      if (channel == PWM_8BIT_CHANNEL_0 ||
          channel == PWM_8BIT_CHANNEL_2)
        return PWM_IS_USING_CLOCK_A;
      else
        return PWM_IS_USING_CLOCK_B;
    }
}



/*
 * get a port and set the coresponding port number to 1 or 0
 */
void helper__bit_setter(volatile unsigned char * PORT, char bit_number, char bit_value);
void helper__bit_setter(volatile unsigned char * PORT, char bit_number, char bit_value)
{
  if (bit_value == 1)
    {
      *PORT |= 1 << bit_number;
    }
  else if (bit_value == 0)
    {
      *PORT &= ~(1 << bit_number);
    }
}


/**
 * set the priod and duty cycel registers for a channel
 */
void pwm_set_period_and_duty_registers(char channel, char mode, unsigned int period_value, unsigned int duty_value );
void pwm_set_period_and_duty_registers(char channel, char mode, unsigned int period_value, unsigned int duty_value )
{
  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      char new_period_value = (char) period_value;
      char new_duty_value = (char) duty_value;

      switch (channel)
        {
        case 0:
          {
            PWMPER0 = new_period_value;
            PWMDTY0 = new_duty_value;
          }
        case 1:
          {
            PWMPER1 = new_period_value;
            PWMDTY1 = new_duty_value;
          }
        case 2:
          {
            PWMPER2 = new_period_value;
            PWMDTY2 = new_duty_value;
          }
        case 3:
          {
            PWMPER3 = new_period_value;
            PWMDTY3 = new_duty_value;
          }
        case 4:
          {
            PWMPER4 = new_period_value;
            PWMDTY4 = new_duty_value;
          }
        case 5:
          {
            PWMPER5 = new_period_value;
            PWMDTY5 = new_duty_value;
          }
        case 6:
          {
            PWMPER6 = new_period_value;
            PWMDTY6 = new_duty_value;
          }
        case 7:
          {
            PWMPER7 = new_period_value;
            PWMDTY7 = new_duty_value;
          }
        }
    }
  else
    {
      switch (channel)
        {
        case 0:
          {
            PWMPER01_16BIT = period_value;
            PWMDTY01_16BIT = duty_value;
          }
        case 1:
          {

            PWMPER23_16BIT = period_value;
            PWMDTY23_16BIT = duty_value;
          }
        case 2:
          {
            PWMPER45_16BIT = period_value;
            PWMDTY45_16BIT = duty_value;
          }
        case 3:
          {

            PWMPER67_16BIT = period_value;
            PWMDTY67_16BIT = duty_value;
          }
        }
    }

}




/**
 * PWME - PWM Enable Register
 * To enable PWM on channel0 and 1 you should write PWME = 0x03 ... 
 * Once concatenated mode is enabled (CONxx bits set in PWMCTL register) then 
 * enabling/disabling the corresponding 16-bit PWM channel is controlled by 
 * the low order PWMEx bit.
 * 
 * status is enabled or disabled ...
 * mode is 16bit or 8bit
 */
void pwm_set_status(char pwm_channel_number, char mode, char status);
void pwm_set_status(char pwm_channel_number, char mode, char status)
{

  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      helper__bit_setter(&PWME, pwm_channel_number, status);
    }
  else
    {
      helper__bit_setter(&PWME, pwm_channel_number+pwm_channel_number+1, status);
    }
}



/*
 * if we like to change the settings for a PWM we have to reset the 
 * configurations first by writing somthing to PWM counter to reset
 * the counter ...
 */ 
void pwm_reset_settings(char channel, char mode);
void pwm_reset_settings(char channel, char mode)
{
  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      switch (channel)
        {
        case 0:
          {
            PWMCNT0 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT0 = 1;
          }
        case 1:
          {
            PWMCNT1 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT1 = 1;
          }
        case 2:
          {
            PWMCNT2 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT2 = 1;
          }
        case 3:
          {
            PWMCNT3 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT3 = 1;
          }
        case 4:
          {
            PWMCNT4 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT4 = 1;
          }
        case 5:
          {
            PWMCNT5 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT5 = 1;
          }
        case 6:
          {
            PWMCNT6 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT6 = 1;
          }
        case 7:
          {
            PWMCNT7 = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT7 = 1;
          }
        }
    }
  else
    {
      switch (channel)
        {
        case 0:
          {
            PWMCNT01_16BIT = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT01_16BIT = 1;
          }
        case 1:
          {
            PWMCNT23_16BIT = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT23_16BIT = 1;
          }
        case 2:
          {
            PWMCNT45_16BIT = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT45_16BIT = 1;
          }
        case 3:
          {
            PWMCNT67_16BIT = 1;
            pwm_set_status(channel,mode, DISABLE);
            PWMCNT67_16BIT = 1;
          }
        }
    }

}






/**
 * PWMPOL - PWM Polarity Register
 * If the polarity bit is one, the PWM channel output is high at the beginning 
 * of the cycle and then goes low when the duty count is reached. Conversely, 
 * if the polarity bit is zero, the output starts low and then goes high when 
 * the duty count is reached.
 */
void pwm_set_polarity(char pwm_channel_number, char mode, char polarity);
void pwm_set_polarity(char pwm_channel_number, char mode,char polarity)
{
  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      helper__bit_setter(&PWMPOL, pwm_channel_number, polarity);
    }
  else
    {
      helper__bit_setter(&PWMPOL, pwm_channel_number+pwm_channel_number+1, polarity);
    }
}




/**
 * PWMCAE  - PWM Center Align Enable Register
 * If the CAEx bit is set to a one, the corresponding PWM output will be center 
 * aligned. If the CAEx bit is cleared, the corresponding PWM output will be left 
 * aligned
 */
void pwm_set_alignment(char pwm_channel_number, char mode, char alignment);
void pwm_set_alignment(char pwm_channel_number, char mode, char alignment)
{
  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      helper__bit_setter(&PWMCAE, pwm_channel_number, alignment);
    }
  else
    {
      helper__bit_setter(&PWMCAE, pwm_channel_number+pwm_channel_number+1, alignment);
    }
}

/*
 * helps us to set the 16bit or 8bit mode of opration for PWM channel.
 */ 
void pwm_set_operation_mode_flags(char pwm_channel_number, char mode);
void pwm_set_operation_mode_flags(char pwm_channel_number, char mode)
{
  if (mode == PWM_16BIT_OPERATION_MODE)
    {
      // why 4??  just an ofset to reach CONxx bits
      helper__bit_setter(&PWMCTL, (pwm_channel_number+4), ENABLE);
    }
  else if (mode == PWM_8BIT_OPERATION_MODE)
    {
      helper__bit_setter(&PWMCTL, ((pwm_channel_number/2) +4), DISABLE);
    }
}



/*
 * set the main clock settings for a channel
 */
void pwm_set_main_clock(char channel, char mode, char is_using_scaled);
void pwm_set_main_clock(char channel, char mode, char is_using_scaled)
{
  if (mode == PWM_8BIT_OPERATION_MODE)
    {
      if (is_using_scaled)
        {
          helper__bit_setter(&PWMCLK,channel, PWM_IS_USING_SCALED_CLOCK);
        }
      else
        {
          helper__bit_setter(&PWMCLK,channel, PWM_IS_NOT_USING_SCALED_CLOCK);
        }
    }
  else // mode == PWM_16BIT_OPERATION_MODE
    {
      char bit = channel+channel+1; //to reach proper bit

      if (is_using_scaled)
        {
          helper__bit_setter(&PWMCLK,bit, PWM_IS_USING_SCALED_CLOCK);
        }
      else
        {
          helper__bit_setter(&PWMCLK,bit, PWM_IS_NOT_USING_SCALED_CLOCK);
        }
    }
}

/*
 * set the main clock divider which can be 0-7
 * 
 * 2^0 or 2^1 ... 2^7
 */ 
void pwm_set_main_clock_divider(char channel, char mode, char divider);
void pwm_set_main_clock_divider(char channel, char mode, char divider)
{
  //Selects prescale clock source for clocks A and B independently: XXX  PCKB2 PCKB1 PCKB0 XXX  PCKA2 PCKA1 PCKA0
  if (pwm_is_using_which_clock(channel, mode) == PWM_IS_USING_CLOCK_A)
    {
      PWMPRCLK &= 0xf0; //reset the pins for A
      PWMPRCLK |=  divider;
    }
  else
    {
      PWMPRCLK &= 0x0f; //reset the pins for B
      PWMPRCLK |=  divider << 4;
    }
}



/*
 * set the scaled clock divider register, IF we are using scaled mode
 */
void pwm_set_scale_clock_divider(char channel, char mode, char is_using_scaled, char divider );
void pwm_set_scale_clock_divider(char channel, char mode, char is_using_scaled, char divider )
{
  if (is_using_scaled)
    {
      if (pwm_is_using_which_clock(channel, mode) == PWM_IS_USING_CLOCK_A)
        {
          PWMSCLA = 0;
          PWMSCLA |= divider;
        }
      else
        {
          PWMSCLB = 0;
          PWMSCLB |= divider;
        }
    }
}



/**
 * pc can send commands to micro controller. Format will be
 * pc:command_code&arg1&arg2&
 * comand code can be from 1 to (2^16  -1)
 * args can be from 1 to (2^16  -1)
 * sample:
 * "pc:125&0&10" 
 */
void process_the_retrieved_command_from_pc(int command,  unsigned int arg1, unsigned int arg2);
void process_the_retrieved_command_from_pc(int command,  unsigned int arg1, unsigned int arg2)
{
  // these series of commands will control the PWM section
  if (command > 100 && command < 200)
    {
      /* 
       * command for pwm channel///////////////////////////////////////////////
       * 
       * examples: 
       * pc:111&0&1& means use PWM 8bit on channel 1
       * pc:111&1&0& means use PWM 16bit on channel 0
       */
      if (command == 111)
        {
          //16bit or 8bit
          if (arg1 == 0)
            pwm_mode = PWM_8BIT_OPERATION_MODE;
          else if (arg1 == 1)
            pwm_mode = PWM_16BIT_OPERATION_MODE;

          pwm_channel_number = (char) arg2;
        }


      /*
       * set the boolean flags /////////////////////////
       * 
       * exampels:
       * pc:122&0&1& means use polarity high at start
       * pc:122&1&0& means use left aligned for the pulse
       */
      else if (command == 122)
        {
          if (arg1 == 0) //polarity
            {
              if (arg2 == 0)
                pwm_polarity = PWM_POLARITY_IS_LOW_AT_BEGINNING_OF_THE_PULSE;
              else if (arg2 == 1)
                pwm_polarity = PWM_POLARITY_IS_HIGH_AT_BEGINNING_OF_THE_PULSE;
            }
          else if (arg1 == 1) //align
            {
              if (arg2 == 0)
                pwm_alignment = PWM_ALIGNMENT_LEFT_ALIGN;
              else if (arg2 == 1)
                pwm_alignment = PWM_ALIGNMENT_CENTER_ALIGN;
            }
          else if (arg1 == 2) //is using SA clock
            {
              if (arg2 == 0)
                pwm_is_using_scaled_clock = PWM_IS_NOT_USING_SCALED_CLOCK;
              else if (arg2 == 1)
                pwm_is_using_scaled_clock = PWM_IS_USING_SCALED_CLOCK;
            }
        }

      else if (command == 133) ///values ///////////////////////////////////
        {
          if (arg1 == 0) //main divider value
            pwm_main_clock_divider = arg2;
          else if (arg1 == 1) //scaled divider value
            pwm_scale_clock_divider = arg2;
          else if (arg1 == 2) //period value
            pwm_period_reg_value = arg2;
          else if (arg1 == 3) //duty value
            pwm_duty_reg_value = arg2;
        }
      else if (command == 144) ///UPDATE SETTINGS///////////////////////////////
        {
          if (arg1 == 1 && arg2 == 1)
            {
              pwm_reset_settings(pwm_channel_number, pwm_mode);

              pwm_set_polarity(pwm_channel_number, pwm_mode, pwm_polarity);
              pwm_set_alignment(pwm_channel_number, pwm_mode, pwm_alignment);
              pwm_set_operation_mode_flags(pwm_channel_number, pwm_mode);
              pwm_set_main_clock(pwm_channel_number, pwm_mode, pwm_is_using_scaled_clock);
              pwm_set_main_clock_divider(pwm_channel_number, pwm_mode, pwm_main_clock_divider);
              pwm_set_scale_clock_divider(pwm_channel_number, pwm_mode, pwm_is_using_scaled_clock, pwm_scale_clock_divider);
              pwm_set_period_and_duty_registers(pwm_channel_number, pwm_mode, pwm_period_reg_value, pwm_duty_reg_value);

              pwm_set_status(pwm_channel_number, pwm_mode, ENABLE);

              
            }
        }
      else if (command == 155) ///UPDATE the BATCH PWM///////////////////////////////////
        {
          if (arg1 == 1)//update batch array
            {
              pwm_batch[arg2].alignment = pwm_alignment;
              pwm_batch[arg2].channel = pwm_channel_number;
              pwm_batch[arg2].duty_reg_value = pwm_duty_reg_value;
              pwm_batch[arg2].is_using_scaled_clock = pwm_is_using_scaled_clock;
              pwm_batch[arg2].main_clock_divider_reg_value = pwm_main_clock_divider;
              pwm_batch[arg2].mode = pwm_mode;
              pwm_batch[arg2].period_reg_value = pwm_period_reg_value;
              pwm_batch[arg2].polarity = pwm_polarity;
              pwm_batch[arg2].scaled_clock_divider_reg_value = pwm_scale_clock_divider;
              
              
            }
          else if (arg1 == 2) //load a pwm settings from batch array
            {

              pwm_alignment =  pwm_batch[arg2].alignment ;
              pwm_channel_number =  pwm_batch[arg2].channel;
              pwm_duty_reg_value = pwm_batch[arg2].duty_reg_value ;
              pwm_is_using_scaled_clock =  pwm_batch[arg2].is_using_scaled_clock ;
              pwm_main_clock_divider =  pwm_batch[arg2].main_clock_divider_reg_value ;
              pwm_mode =  pwm_batch[arg2].mode ;
              pwm_period_reg_value = pwm_batch[arg2].period_reg_value ;
              pwm_polarity = pwm_batch[arg2].polarity  ;
              pwm_scale_clock_divider =  pwm_batch[arg2].scaled_clock_divider_reg_value ;


              pwm_reset_settings(pwm_channel_number, pwm_mode);
              pwm_set_polarity(pwm_channel_number, pwm_mode, pwm_polarity);
              pwm_set_alignment(pwm_channel_number, pwm_mode, pwm_alignment);
              pwm_set_operation_mode_flags(pwm_channel_number, pwm_mode);
              pwm_set_main_clock(pwm_channel_number, pwm_mode, pwm_is_using_scaled_clock);
              pwm_set_main_clock_divider(pwm_channel_number, pwm_mode, pwm_main_clock_divider);
              pwm_set_scale_clock_divider(pwm_channel_number, pwm_mode, pwm_is_using_scaled_clock, pwm_scale_clock_divider);
              pwm_set_period_and_duty_registers(pwm_channel_number, pwm_mode, pwm_period_reg_value, pwm_duty_reg_value);
              pwm_set_status(pwm_channel_number, pwm_mode, ENABLE);
             }

        }
    }

}


//Servo connected to PP07
static void  change_servo(unsigned int input);
static void  change_servo(unsigned int input)
{
  
  if (input == 200) ;  
  else if (input > 195 || input < 175) return;
    

	// 8bit as arg1 = 0, channel 7 as arg2 = 7
	process_the_retrieved_command_from_pc(111,0,7);	
	process_the_retrieved_command_from_pc(122,0,0);
	process_the_retrieved_command_from_pc(122,1,1);
	process_the_retrieved_command_from_pc(122,2,1);
	process_the_retrieved_command_from_pc(133,0,3);
	process_the_retrieved_command_from_pc(133,1,75);
	process_the_retrieved_command_from_pc(133,2,200);
	process_the_retrieved_command_from_pc(133,3,input);
	process_the_retrieved_command_from_pc(144,1,1);	

}

static unsigned int get_ir_range_and_change_servo_motor_accordingly(unsigned int atd_result);
static unsigned int get_ir_range_and_change_servo_motor_accordingly(unsigned int atd_result)
{
	int range = 0 ; //range is in cm

	if (atd_result >= 519) {  range = 10; change_servo(175); }
 	else if (atd_result >= 447) {  range = 12; change_servo(176); }
 	else if (atd_result >= 390) {  range = 14; change_servo(177);}
 	else if (atd_result >= 348) {  range = 16; change_servo(178);}	 
	else if (atd_result >= 315) {  range = 18; change_servo(179);}	 
	else if (atd_result >= 284) {  range = 20; change_servo(180);}
 	else if (atd_result >= 259) {  range = 22; change_servo(181);}
 	else if (atd_result >= 247) {  range = 24; change_servo(182);}
 	else if (atd_result >= 227) {  range = 26; change_servo(183);}	 
	else if (atd_result >= 210) {  range = 28; change_servo(184);}	 
	else if (atd_result >= 197) {  range = 30; change_servo(185);}
 	else if (atd_result >= 187) {  range = 32; change_servo(186);}
 	else if (atd_result >= 178) {  range = 34; change_servo(187);}
 	else if (atd_result >= 165) {  range = 36; change_servo(188);}
 	else if (atd_result >= 161) {  range = 38; change_servo(189);}
 	else if (atd_result >= 152) {  range = 40; change_servo(190);}
 	else if (atd_result >= 149) {  range = 42; change_servo(191);}
 	else if (atd_result >= 133) {  range = 44; change_servo(192);}
 	else if (atd_result >= 134) {  range = 46; change_servo(193);}
 	else if (atd_result >= 132) {  range = 48; change_servo(194);}
 	else if (atd_result >= 124) {  range = 50; change_servo(195);}
 	else if (atd_result >= 118) {  range = 52; change_servo(185);}
 	else if (atd_result >= 115) {  range = 54; change_servo(185);}
 	else if (atd_result >= 106) {  range = 56; change_servo(185);}	 
	else if (atd_result >= 104) {  range = 58; change_servo(185);}
 	else if (atd_result >= 107) {  range = 60; change_servo(185);}
 	else if (atd_result >= 102) {  range = 62; change_servo(185);}
 	else if (atd_result >= 101) {  range = 64; change_servo(185);}
 	else if (atd_result >= 98) {  range = 66; change_servo(185);}
 	else if (atd_result >= 94) {  range = 68; change_servo(185);}
 	else if (atd_result >= 86) {  range = 70; change_servo(185);}
 	else if (atd_result >= 83) {  range = 72; change_servo(185);}
 	else if (atd_result >= 82) {  range = 74; change_servo(185);}
 	else if (atd_result >= 86) {  range = 76; change_servo(185);}
 	else if (atd_result >= 84) {  range = 78; change_servo(185);}
 	else if (atd_result >= 82) {  range = 80; change_servo(185);}
 	
	
	return range;
}


////////////////////////////////////////////////////
// make sure to create a task for this in uc-OS
// and don't forget to init the IR ATD subsystem (atd_init call above)
//get_ir_range_and_change_servo_motor_accordingly(ATD0DR0); // read the IR sensor values and change the servo's position accordingly every 300 ms 




