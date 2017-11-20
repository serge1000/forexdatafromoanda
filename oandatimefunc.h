#include <stdio.h>

#include <stdlib.h>

#include <string.h>


/*2014-07-03T04:00:00Z   - 20 chars */


// headers of project time functions -----------

//struct tm time_t2tm( time_t time1,  struct tm res);

/*struct tm *time_t2tm( time_t time1); //OK

time_t tm2time_t(const struct tm *tm);//OK

struct tm oandatimestr2tm(char *oandatimestr);//OK

char *tm2oandatimestr(struct tm time1,size_t dlen,char *res);

char * add_time_to_oandatimestr( char * oandatimestr,  char *res, int timeinsec);

char * deduct_time_from_oandatimestr( char * oandatimestr,  char *res, int timeinsec);*/

//END OF  headers of project time functions ----





char *add_time_to_oandatimestr( char *oandatimestr, char *res, int timeinsec){



	return res;

}

char *deduct_time_from_oandatimestr( char * oandatimestr,  char *res, int timeinsec){

	return res;

}

struct tm *time_t2tm( time_t time1){

	return gmtime( &time1 );

}

time_t tm2time_t(const struct tm *tm){

	// Month-to-day offset for non-leap-years.

    static const int month_day[12] =

    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};



    // Most of the calculation is easy; leap years are the main difficulty.

    int month = tm->tm_mon % 12;

    int year = tm->tm_year + tm->tm_mon / 12;

    if (month < 0) {   // Negative values % 12 are still negative.

        month += 12;

        --year;

    }



    // This is the number of Februaries since 1900.

    const int year_for_leap = (month > 1) ? year + 1 : year;



    time_t rt = tm->tm_sec                             // Seconds

        + 60 * (tm->tm_min                          // Minute = 60 seconds

        + 60 * (tm->tm_hour                         // Hour = 60 minutes

        + 24 * (month_day[month] + tm->tm_mday - 1  // Day = 24 hours

        + 365 * (year - 70)                         // Year = 365 days

        + (year_for_leap - 69) / 4                  // Every 4 years is     leap...

        - (year_for_leap - 1) / 100                 // Except centuries...

        + (year_for_leap + 299) / 400)));           // Except 400s.

    return rt < 0 ? -1 : rt;

}



struct tm oandatimestr2tm(char *oandatimestr){

	struct tm res;



	res.tm_year = atoi(strtok(oandatimestr, "-"))- 1900;      //The number of years since 1900 

	res.tm_mon = atoi(strtok(NULL, "-")) - 1;                 //month, range 0 to 11     

	res.tm_mday = atoi(strtok(NULL, "T"));				      //day of the month, range 1 to 31 

	res.tm_hour = atoi(strtok(NULL, ":"));					  //hours, range 0 to 23   

	res.tm_min = atoi(strtok(NULL, ":"));					  //minutes, range 0 to 59

	res.tm_sec = atoi(strtok(NULL, "."));			   		  //seconds,  range 0 to 59         

	res.tm_wday = 0;										  //day of the week, range 0 to 6   

	res.tm_yday = 0;										  //day in the year, range 0 to 365 

	res.tm_isdst = 0;										  //daylight saving time  



	return  res;

}



char *tm2oandatimestr(struct tm time1,size_t dlen,char *res1){

    snprintf(res1, dlen, "%04d-%02d-%02dT%02d:%02d:%02d.000000Z",time1.tm_year+1900,time1.tm_mon+1,time1.tm_mday,time1.tm_hour,time1.tm_min,time1.tm_sec );

	return res1;

}



char *add_one_period_to_oandatimestr( char *oandatimestr,  char* oandagranularity){

	long int oandagranularity_in_sec = 0;



		if(!strncmp(oandagranularity, "S5", 2)){oandagranularity_in_sec = 5;}

		else if(!strncmp(oandagranularity, "S10", 3)){oandagranularity_in_sec = 10;}

		else if(!strncmp(oandagranularity, "S15", 3)){oandagranularity_in_sec = 15;}

		else if(!strncmp(oandagranularity, "S30", 3)){oandagranularity_in_sec = 30;}

		else if(!strncmp(oandagranularity, "M1", 2)){oandagranularity_in_sec = 60;}

		else if(!strncmp(oandagranularity, "M2", 2)){oandagranularity_in_sec = 60*2;}

		else if(!strncmp(oandagranularity, "M3", 2)){oandagranularity_in_sec = 60*3;}

		else if(!strncmp(oandagranularity, "M4", 2)){oandagranularity_in_sec = 60*4;}

		else if(!strncmp(oandagranularity, "M5", 2)){oandagranularity_in_sec = 60*5;}

		else if(!strncmp(oandagranularity, "M10", 3)){oandagranularity_in_sec = 60*10;}

		else if(!strncmp(oandagranularity, "M15", 3)){oandagranularity_in_sec = 60*15;}

		else if(!strncmp(oandagranularity, "M30", 3)){oandagranularity_in_sec = 60*30;}

		else if(!strncmp(oandagranularity, "H1", 2)){oandagranularity_in_sec = 3600;}

		else if(!strncmp(oandagranularity, "H2", 2)){oandagranularity_in_sec = 3600*2;}

		else if(!strncmp(oandagranularity, "H3", 2)){oandagranularity_in_sec = 3600*3;}

		else if(!strncmp(oandagranularity, "H4", 2)){oandagranularity_in_sec = 3600*4;}

		else if(!strncmp(oandagranularity, "H6", 2)){oandagranularity_in_sec = 3600*5;}

		else if(!strncmp(oandagranularity, "H8", 2)){oandagranularity_in_sec = 3600*8;}

		else if(!strncmp(oandagranularity, "H12", 3)){oandagranularity_in_sec = 3600*12;}

		else if(!strncmp(oandagranularity, "D", 1)){oandagranularity_in_sec = 3600*24;}

		else if(!strncmp(oandagranularity, "W", 1)){oandagranularity_in_sec = 3600*24*7;}

		else if(!strncmp(oandagranularity, "M", 1)){oandagranularity_in_sec = 3600*24*30;}

		else { printf ("\nWe have a problem  in add_one_period_to_oandatimestr.\n");}

	//	printf("\nhere 1, time before %s , sec %d    gran  %s\n",oandatimestr,oandagranularity_in_sec,oandagranularity);getchar();

		struct tm  res = oandatimestr2tm(oandatimestr);

	//			printf("\nhere 2\n");getchar();

		time_t oandatimestr_in_sec = tm2time_t(&res);


		oandatimestr_in_sec = oandatimestr_in_sec + oandagranularity_in_sec;

		struct tm *tm1 = time_t2tm( oandatimestr_in_sec);

//		char res1[28]="2016-01-03T22:05:00.000000Z";

		tm2oandatimestr(*tm1,28,oandagranularity);

	return oandagranularity;

}

