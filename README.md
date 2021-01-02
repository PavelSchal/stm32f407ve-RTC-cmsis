# stm32f407ve-RTC-cmsis
RTC .c / .h  Modul bietet einen Beispielcode für stm32f407ve ( cmsis ) basierend auf "Blackboard STM32F407" in Eclipse IDE.

main.c Beispiel
...
/**/
RTC_struct initRtcDaten ;         /* Anfangswerte von Zeit und Datum */
RTC_struct getRtcDaten ;          /* Werte vom laufenden RTC Modul auslesen  */
/**/
...
int main( int argc, char* argv[ ] ) {
	/**/
	initRtcDaten.year = 120 ;
	initRtcDaten.month = 10 ;
	initRtcDaten.week = 5 ;
	initRtcDaten.date = 3 ;
	initRtcDaten.hour = 1 ;
	initRtcDaten.minute = 0 ;
	initRtcDaten.sec = 0 ;
  .
  .
  .
  RtcTimInit( ) ;                /* RTC Modul Initialisition */
  RTC_Update( &initRtcDaten ) ;  /* RTC Modul update Zeit, Datum. Zeit und Datum wie gewuenscht einstellen */
  /**/
	while( 1 ) {
	get_RTC_Daten( &getRtcDaten ) ; /* Zeit und Datum abfragen */
	}
} 
