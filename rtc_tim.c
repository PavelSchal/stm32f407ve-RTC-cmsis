/*!
 * \file rtc_tim.c
 * \author Pavel Schal
 * \version 1.0
 * \date 29.12.2020
 * \warning Die Datei rtc_tim.c v_1.0 wurde noch nicht getestet.
 * \brief Initialisation von rtc timer.
 *
 * Diese Datei beinhaltet die RTC_TIM Einstellungen des Mikrokontrollers.
 *
 * \attention Das .h/.c Modul kann frei und ohne Lizenz verwendet werden. Nur der erste Doxigen-Block in
 * 			  jeder Datei soll bleiben.
*/
/**/
#include "main.h"
/**/
/* Variablen und Konstanten */
volatile RTC_struct rtc_value = { 0,0,0,0,0,0,0 } ;	/* Initialisation Werte*/
/**/
/*!
 * Die Funktion set_RTC_TR_Register( ) updatet den RTC_TR Register.
 * \param[ out ] void
 * \param[ in ]  RTC_struct volatile *value enthaelt die Zeit und Datum fuer RTC Modul.
 * \warning Funktion ist inkapsuliert. Kann nicht von anderen Modulen verwendet werden.
 */
static void set_RTC_TR_Register(  RTC_struct volatile *value  ) {
	uint32_t TR = 0x00 ;
	/**/
	TR |= (uint32_t)( ( ( ( value -> hour / 10 ) & 0x03 ) <<  20 )  | ( ( ( value -> hour % 10 ) & 0x0f ) << 16 ) ) ;// | (uint32_t)(value->week/10*16 + value->week%10) << 13 | (uint32_t)(value->month/10*16 + value->month%10) << 8 | (uint32_t)(value->date/10*16 + value->date%10); /* Datum einstellen */
	TR |= (uint32_t)( ( ( ( value -> minute / 10 ) & 0x03 ) <<  12 )  | ( ( ( value -> minute % 10 ) & 0x0f ) << 8 ) ) ;
	TR |= (uint32_t)( ( ( ( value -> sec / 10 ) & 0x07 ) <<  4 )  | ( ( ( value -> sec % 10 ) & 0x0f ) << 0 ) ) ;
	RTC->TR = TR;
}
/**/
/*!
 * Die Funktion set_RTC_DR_Register( ) updatet den RTC_TR Register.
 * \param[ out ] void
 * \param[ in ]  RTC_struct volatile *value enthaelt die Zeit und Datum fuer RTC Modul.
 * \warning Funktion ist inkapsuliert. Kann nicht von anderen Modulen verwendet werden.
 */
static void set_RTC_DR_Register(  RTC_struct volatile *value  ) {
	uint32_t DR = 0x00 ;											/* Variable fuer Time und Date Werte */
	/**/
	DR |= (uint32_t)( ( ( ( value -> year / 10 ) & 0x0f ) <<  20 )  | ( ( ( value -> year % 10 ) & 0x0f ) << 16 ) ) ;// | (uint32_t)(value->week/10*16 + value->week%10) << 13 | (uint32_t)(value->month/10*16 + value->month%10) << 8 | (uint32_t)(value->date/10*16 + value->date%10); /* Datum einstellen */
	DR |= (uint32_t)( ( ( ( value -> month / 10 ) & 0x01 ) <<  12 )  | ( ( ( value -> month % 10 ) & 0x0f ) << 8 ) ) ;
	DR |= (uint32_t)( ( ( ( value -> week ) & 0x07 ) <<  13 ) ) ;
	DR |= (uint32_t)( ( ( ( value -> date / 10 ) & 0x03 ) <<  4 )  | ( ( ( value -> date % 10 ) & 0x0f ) << 0 ) ) ;
	RTC->DR = DR ;
}
/**/
/*!
 * Die Funktion RtcTimInit( ) updatet die RTC Modul Register.
 * \param[ out ] void
 * \param[ in ]  void
 * \warning
 */
void RtcTimInit( void ) {
	RCC->APB1ENR |= RCC_APB1ENR_PWREN	;						/* PWR Modul clock enable */
	PWR->CR |= PWR_CR_DBP	;									/* 1: Access to RTC and RTC Backup registers and backup SRAM enabled */
	/**/
	/*RTC clock */
	RCC -> BDCR |= RCC_BDCR_RTCSEL_0 | RCC_BDCR_RTCSEL_1 ;	    /* 11: HSE oscillator  */
	RCC->CFGR |= ( 8 << 16 ) ;							   		/* 8 Mhz devide to 1Mhz, WICHTIG: HSE / 32 aus Referenzmanual wird vernachlaessigt !!! */
	RCC->BDCR |= RCC_BDCR_RTCEN ; 						  		/* 1: RTC clock enabled */
	/**/
	RTC_Update(	&rtc_value ) ;									/* Update RTC TR und DR Register */
}
/**/
/*!
 * Die Funktion RTC_Update( ) updatet die RTC Modul Register.
 * \param[ out ] void
 * \param[ in ]  RTC_struct volatile *value enthaelt die Zeit und Datum fuer RTC Modul.
 * \warning
 */
void RTC_Update( RTC_struct volatile *value ) {
	/**/
	/* The following steps are required to unlock the write protection on all the RTC registers\
		except for RTC_ISR[13:8], RTC_TAFCR, and RTC_BKPxR. */
	RTC->WPR = 0xCA;														/* von Referenzmanual */
	RTC->WPR = 0x53;														/* von Referenzmanual */
	/**/
	RTC->ISR |= RTC_ISR_INIT ;												/* Initialization mode */
	while( !(RTC->ISR & RTC_ISR_INITF) ) { asm( "NOP\n\t" ) ; }				/* Calendar registers update is allowed. */
	RTC->PRER |= (uint32_t)( ( 125 - 1 ) << 16  ) ;							/* 1 MHz / 125 = 8 kHz, WICHTIG: HSE / 32 aus Referenzmanual wird vernachlaessigt !!! */
	RTC->PRER |= (uint32_t)( ( 8000 - 1 ) << 0  ) ;							/* 8 kHz / 8000 = 1 Hz */
	/**/
	set_RTC_TR_Register( value ) ;											/* RTC_TR Register ausfuellen */
	set_RTC_DR_Register( value ) ;											/* RTC_DR Register ausfuellen */
	/**/
	RTC->ISR &= ~(RTC_ISR_INIT);	/* disable Initialization mode */
	RTC->WPR = 0xFF;				/* lock the write protection again */
}
/**/
/*!
 * Die Funktion get_RTC_Daten( ) erlaubt die Werte aus RTC Modul zu erhalten.
 * \param[ out ] void
 * \param[ in ]  RTC_struct volatile *value enthaelt die Felder fuer Zeit und Datum fuer RTC Modul.
 * \warning
 */
void get_RTC_Daten( RTC_struct *value ) {
	/**/
	value -> hour = ( ( ( ( ( RTC->TR >> 20 ) ) & 0x03 ) * 10 )  + ( ( ( RTC->TR >> 16 ) ) & 0x0f ) ) ;
	value -> minute = ( ( ( ( ( RTC->TR >> 12 ) ) & 0x03 ) * 10 )  + ( ( ( RTC->TR  >> 8 ) ) & 0x0f ) ) ;
	value -> sec = ( ( ( ( ( RTC->TR  >>  4 ) ) & 0x07 ) *  10  )  +  ( ( ( RTC->TR >> 0 ) )  & 0x0f ) ) ;
	/**/
	value -> year = ( ( ( ( RTC->DR >>  20 ) & 0x0f ) * 10 )  | ( ( ( RTC->DR  >> 16 ) & 0x0f ) ) ) ;
	value -> month = ( ( ( ( RTC->DR >>  12  ) & 0x01 ) * 10 )  | ( ( ( RTC->DR >> 8 ) & 0x0f )  ) ) ;
	value -> week = ( ( RTC->DR >>  13 ) & 0x07 ) ;
	value -> date = ( ( ( ( RTC->DR >>  4 ) & 0x03 ) * 10 )  | ( ( ( RTC->DR  >> 0 ) & 0x0f ) ) ) ;
}
