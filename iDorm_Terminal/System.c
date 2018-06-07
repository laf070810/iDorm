/*
 * System.c
 *
 *  Created on: 2018Äê5ÔÂ25ÈÕ
 *      Author: LAF
 */

#include "System.h"

void InitAll(void)
{
    increaseVCoreToLevel2();
    initClockTo16MHz();
    UCA0_UART_Init();
    UCA1_UART_Init();
	LCD_init();
	LCD_clear();
}

uint16_t setVCoreUp(uint8_t level){
    uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

    //The code flow for increasing the Vcore has been altered to work around
    //the erratum FLASH37.
    //Please refer to the Errata sheet to know if a specific device is affected
    //DO NOT ALTER THIS FUNCTION

    //Open PMM registers for write access
    PMMCTL0_H = 0xA5;

    //Disable dedicated Interrupts
    //Backup all registers
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE |
                SVSLPE | SVMHVLRIE | SVMHIE |
                SVSMHDLYIE | SVMLVLRIE | SVMLIE |
                SVSMLDLYIE
                );
    SVSMHCTL_backup = SVSMHCTL;
    SVSMLCTL_backup = SVSMLCTL;

    //Clear flags
    PMMIFG = 0;

    //Set SVM highside to new level and check if a VCore increase is possible
    SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);

    //Wait until SVM highside is settled
    while((PMMIFG & SVSMHDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMHDLYIFG;

    //Check if a VCore increase is possible
    if((PMMIFG & SVMHIFG) == SVMHIFG)
    {
        //-> Vcc is too low for a Vcore increase
        //recover the previous settings
        PMMIFG &= ~SVSMHDLYIFG;
        SVSMHCTL = SVSMHCTL_backup;

        //Wait until SVM highside is settled
        while((PMMIFG & SVSMHDLYIFG) == 0)
        {
            ;
        }

        //Clear all Flags
        PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                     SVMLVLRIFG | SVMLIFG |
                     SVSMLDLYIFG
                     );

        //Restore PMM interrupt enable register
        PMMRIE = PMMRIE_backup;
        //Lock PMM registers for write access
        PMMCTL0_H = 0x00;
        //return: voltage not set
        return false;
    }

    //Set also SVS highside to new level
    //Vcc is high enough for a Vcore increase
    SVSMHCTL |= (SVSHRVL0 * level);

    //Wait until SVM highside is settled
    while((PMMIFG & SVSMHDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMHDLYIFG;

    //Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;

    //Set SVM, SVS low side to new level
    SVSMLCTL = SVMLE | (SVSMLRRL0 * level) |
               SVSLE | (SVSLRVL0 * level);

    //Wait until SVM, SVS low side is settled
    while((PMMIFG & SVSMLDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMLDLYIFG;
    //SVS, SVM core and high side are now set to protect for the new core level

    //Restore Low side settings
    //Clear all other bits _except_ level settings
    SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 +
                 SVSMLRRL1 + SVSMLRRL2
                 );

    //Clear level settings in the backup register,keep all other bits
    SVSMLCTL_backup &=
        ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    //Restore low-side SVS monitor settings
    SVSMLCTL |= SVSMLCTL_backup;

    //Restore High side settings
    //Clear all other bits except level settings
    SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 +
                 SVSMHRRL0 + SVSMHRRL1 +
                 SVSMHRRL2
                 );

    //Clear level settings in the backup register,keep all other bits
    SVSMHCTL_backup &=
        ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    //Restore backup
    SVSMHCTL |= SVSMHCTL_backup;

    //Wait until high side, low side settled
    while(((PMMIFG & SVSMLDLYIFG) == 0) &&
          ((PMMIFG & SVSMHDLYIFG) == 0))
    {
        ;
    }

    //Clear all Flags
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG
                );

    //Restore PMM interrupt enable register
    PMMRIE = PMMRIE_backup;

    //Lock PMM registers for write access
    PMMCTL0_H = 0x00;

    return true;
}

bool increaseVCoreToLevel2()
{
    uint8_t level = 2;
    uint8_t actlevel;
    bool status = true;

    //Set Mask for Max. level
    level &= PMMCOREV_3;

    //Get actual VCore
    actlevel = PMMCTL0 & PMMCOREV_3;

    //step by step increase or decrease
    while((level != actlevel) && (status == true))
    {
        if(level > actlevel)
        {
            status = setVCoreUp(++actlevel);
        }
    }

    return (status);
}

void initClockTo16MHz()
{
    P5SEL |= BIT4+BIT5;                         // Select XT1

    UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_0;                        // Set ACLK = XT1CLK
    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_0 + 487;                   // Set DCO Multiplier for 16MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (487 + 1) * 32768 = 16MHz
                                              // Set FLL Div = fDCOCLK
    __bic_SR_register(SCG0);                  // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 16 MHz / 32,768 Hz = 500000 = MCLK cycles for DCO to settle
    __delay_cycles(500000);//
    // Loop until XT1,XT2 & DCO fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                          // Clear fault flags
    }while (SFRIFG1&OFIFG);                         // Test oscillator fault flag
}

void UCA0_UART_Init()
{
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
#if UCA0_UART_MODE == SMCLK_115200

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 16000000/(16*115200) = 8.6805
    // Fractional portion = 0.6805
    // Use Table 24-5 in Family User Guide
    UCA0BR0 = 8;                               // 16000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCOS16 | UCBRF_11 | UCBRS_0;

#elif UCA0_UART_MODE == SMCLK_9600

    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 16000000/(16*9600) = 104.1667
    // Fractional portion = 0.1667
    // Use Table 24-5 in Family User Guide
    UCA0BR0 = 104;                            // 16000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCOS16 | UCBRF_3 | UCBRS_0;

#elif UCA0_UART_MODE == ACLK_9600

    UCA0CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
    // Baud Rate calculation
    // 32768/(9600) = 3.4133
    // Fractional portion = 0.4133
    // Use Table 24-5 in Family User Guide
    UCA0BR0 = 3;                             // 32768/9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCBRS_3;    //0x0300 is UCBRSx = 0x03

#else
    # error "Please specify baud rate to 115200 or 9600"
#endif

    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    P3SEL = BIT3 + BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
}

void UCA1_UART_Init(void)
{
    // Configure USCI_A1 for UART mode
        UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
#if UCA1_UART_MODE == SMCLK_115200

        UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
        // Baud Rate calculation
        // 16000000/(16*115200) = 8.6805
        // Fractional portion = 0.6805
        // Use Table 24-5 in Family User Guide
        UCA1BR0 = 8;                               // 16000000/16/9600
        UCA1BR1 = 0x00;
        UCA1MCTL |= UCOS16 | UCBRF_11 | UCBRS_0;

#elif UCA1_UART_MODE == SMCLK_9600

        UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
        // Baud Rate calculation
        // 16000000/(16*9600) = 104.1667
        // Fractional portion = 0.1667
        // Use Table 24-5 in Family User Guide
        UCA1BR0 = 104;                            // 16000000/16/9600
        UCA1BR1 = 0x00;
        UCA1MCTL |= UCOS16 | UCBRF_3 | UCBRS_0;

#elif UCA1_UART_MODE == ACLK_9600

        UCA1CTLW0 |= UCSSEL__ACLK;               // CLK = ACLK
        // Baud Rate calculation
        // 32768/(9600) = 3.4133
        // Fractional portion = 0.4133
        // Use Table 24-5 in Family User Guide
        UCA1BR0 = 3;                             // 32768/9600
        UCA1BR1 = 0x00;
        UCA1MCTL |= UCBRS_3;    //0x0300 is UCBRSx = 0x03

    #else
        # error "Please specify baud rate to 115200 or 9600"
    #endif

        UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
        UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

        P4SEL = BIT4 + BIT5;                        // P3.4,5 = USCI_A0 TXD/RXD
}

void UCA0_UART_SendString(char * s)
{
    int i, len = strlen(s);

    for (i = 0; i < len; i++)
    {
        UCA0TXBUF = s[i];
        while (!(UCA0IFG & UCTXIFG));
    }
}

void UCA1_UART_SendString(char * s)
{
    int i, len = strlen(s);

    for (i = 0; i < len; i++)
    {
        UCA1TXBUF = s[i];
        while (!(UCA1IFG & UCTXIFG));
    }
}
