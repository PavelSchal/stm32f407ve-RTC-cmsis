/*!
 * \file rtc_tim.h
 * \author Pavel Schal
 * \version 1.0
 * \date 29.12.2020
 * \warning Die Datei rtc_tim.h v_1.0 wurde noch nicht getestet.
 * \brief Initialisation von RTC_TIM.
 *
 * Diese Datei beinhaltet die RTC_TIM Einstellungen des Mikrokontrollers.
 *
 * \attention Das .h/.c Modul kann frei und ohne Lizenz verwendet werden. Nur der erste Doxigen-Block in
 * 			  jeder Datei soll bleiben.
*/
/**/
#ifndef RTC_TIM_H_
#define RTC_TIM_H_
/**/
/*!
 * Die Struktur von RTC Modul enthaelt Zeit und Datum. Kann global verwendet werden.
 */
typedef struct sRTC_struct{
	 unsigned year   : 8 ;
	 unsigned month  : 5 ;
	 unsigned week   : 3 ;
	 unsigned date   : 6 ;
	 unsigned hour   : 6 ;
	 unsigned minute : 7 ;
	 unsigned sec    : 7 ;
}RTC_struct;
/**/
//extern volatile RTC_struct *rtc_value ;
/**/
void RtcTimInit( void ) ;
void RTC_Update( RTC_struct volatile *value ) ;
void get_RTC_Daten( RTC_struct *value ) ;
/**/
#endif /* RTC_TIM_H_ */
