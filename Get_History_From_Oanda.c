/* Description
 * Read history rates from oanda
 * gcc Get_History_From_Oanda.c -o Get_History_From_Oanda -ljansson -lcurl  
 */

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "oandatimefunc.h"


// Helpers to write curl output into string

struct string {
  char *ptr;
  size_t len;
};


void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}
/*
int check_input(int argc, char *argv[], char **oandaInstrument,char **oandagranularity,char **oandaYear,char *ratefilename){
	if( argc == 2 ) {
		//EUR_USD-2016-H1
	    (*oandaInstrument) = strtok(argv[1], "_");    
	    (*oandaYear) = strtok(NULL, "-"); 
	    (*oandagranularity) = strtok(NULL, "-"); 
	}
	else if( argc < 2 ) {
		printf("\n Continue with defaults.\n");
	}
	else{
	   printf("\nToo many arguments supplied.\n");
	  return 0;
	}
	// create full filename
	char temp_file_name[60] = {0};
	snprintf(temp_file_name, sizeof temp_file_name, "%s%s%s%s%s%s%s%s%s", "/home/project/history/",*oandaInstrument,"/rates/",*oandaInstrument,"-",*oandaYear,"-",*oandagranularity,".txt");
	strcpy(ratefilename,temp_file_name);
	printf("\n Continue with : %s %s %s. File name  is %s.\n",*oandaInstrument,*oandagranularity,*oandaYear,temp_file_name);
	getchar();
	return 1;
}
*/

// END OF Helpers to write curl output into string


int main( int argc, char *argv[] )  {


    CURL *curl;
    CURLcode res;
	FILE * fp;

    char temp2[40] = {0};
    char temp3[40] = {0};	

	json_t *root;
	json_error_t error;

	int stopwhileflag = 0;
	int tempflag = 0;
	long int linecounter = 0;

	// validate input and print params

	char *oandaInstrument = malloc(8);
	char *oandagranularity = malloc(3);
	char *oandaYear = malloc(5);

//	strcpy(oandaInstrument,"EUR_USD");strcpy(oandagranularity,"H1");strcpy(oandaYear,"2016");char ratefilename[] = "/home/project/history/EUR_USD-2016-H1/Rates.CSV";
//	strcpy(oandaInstrument,"EUR_USD");strcpy(oandagranularity,"H1");strcpy(oandaYear,"2016");char ratefilename[] = "/home/project/history/EUR_USD-2016-H1/Rates.CSV";
	strcpy(oandaInstrument,"EUR_USD");strcpy(oandagranularity,"M5");strcpy(oandaYear,"2017");char ratefilename[] = "/home/project/history/EUR_USD-2017-M5/Rates.CSV";

	printf("\nContinue with : %s %s %s. File name  is %s\n",oandaInstrument,oandagranularity,oandaYear,ratefilename);

	getchar();

	//open file
	if((fp = fopen(ratefilename, "w"))==NULL) {
	    printf("Cannot open file.\n");
		exit(1);
    }

	char *oandastartdate1 = (char *) malloc(28);

	snprintf(oandastartdate1, 28, "%s%s",oandaYear,"-01-01T00:00:00.000000Z");

	while(!stopwhileflag){ //-------------------------------------------------------------
    
    curl = curl_easy_init();
    if(curl) {

	    struct string s;
	    init_string(&s);

		char *startdate = curl_easy_escape(curl, oandastartdate1, 27);
		char oandaURLstr[200] = {0};
		snprintf(oandaURLstr, sizeof oandaURLstr, "%s%s%s%s%s%s", "https://api-fxpractice.oanda.com/v1/candles?instrument=", oandaInstrument, "&count=5000&candleFormat=bidask&granularity=",oandagranularity,"&start=",startdate);
	    if(startdate) {curl_free(startdate);}


	printf(">%s<",oandaURLstr);


        curl_easy_setopt(curl, CURLOPT_URL, oandaURLstr);

        struct curl_slist *chunk = NULL;

        // uncomment to add authorization header: not required for sandbox
        chunk = curl_slist_append(chunk, "Authorization: Bearer XXXXXXXXXXXXXXXXXX-XXXXXXXXXX");
        
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		
		/* use custom headers */
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        
        /* Perform the request, res will get the return code */
         res = curl_easy_perform(curl);
	//res = curl_multi_perform(curl);        

        /* Check for errors */
        
		if(res != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
                }
        
	    //printf("%s", s.ptr);

	//	fputs(s.ptr, fp);
// PARSE AND WRITE -------------------
		root = json_loads(s.ptr, 0, &error);
		free(s.ptr);

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }


json_t *candles;
const char *instrument, *granularity;
  json_unpack(root, "{s:s,s:s,s:o}",
    "instrument", &instrument, 
    "granularity", &granularity, 
     "candles", &candles);

//printf("\n--%s--\n--%s--\n--%s--\n",instrument, granularity,candles);



//return 1;

root=candles;

    if(!json_is_array(root))
    {
        fprintf(stderr, "error: root is not an array\n");
        json_decref(root);
        return 1;
    }

	int i;
    for(i = 0; i < json_array_size(root); i++)
    {
        json_t *data, *time, *openBid,*openAsk,*highBid,*highAsk,*lowBid,*lowAsk,*closeBid,*closeAsk,*volume;
        //const char *message_text;

        data = json_array_get(root, i);
        if(!json_is_object(data))
        {
            fprintf(stderr, "error: commit data %d is not an object\n", (int)(i + 1));
            json_decref(root);
            return 1;
        }

        time = json_object_get(data, "time");
        if(!json_is_string(time))
        {
            fprintf(stderr, "error: commit %d: time is not a string\n", (int)(i + 1));
            //return 1;
        }

        openBid = json_object_get(data, "openBid");
        if(!json_is_real(openBid))
        {
            fprintf(stderr, "error: commit %d: openBid is not a double --- %f\n", (int)(i + 1),openBid);
            //return 1;
        }

        openAsk = json_object_get(data, "openAsk");
        if(!json_is_real(openAsk))
        {
            fprintf(stderr, "error: commit %d: openAsk is not a double --- %f\n", (int)(i + 1),openAsk);
            //return 1;
        }

        highBid = json_object_get(data, "highBid");
        if(!json_is_real(highBid))
        {
            fprintf(stderr, "error: commit %d: highBid is not a double --- %f\n", (int)(i + 1),highBid);
            //return 1;
        }

        highAsk = json_object_get(data, "highAsk");
        if(!json_is_real(highAsk))
        {
            fprintf(stderr, "error: commit %d: highAsk is not a double--- %f\n", (int)(i + 1),highAsk);
            //return 1;
        }

        lowBid = json_object_get(data, "lowBid");
        if(!json_is_real(lowBid))
        {
            fprintf(stderr, "error: commit %d: lowBid is not a double --- %f\n", (int)(i + 1),lowBid);
            //return 1;
        }

        lowAsk = json_object_get(data, "lowAsk");
        if(!json_is_real(lowAsk))
        {
            fprintf(stderr, "error: commit %d: lowAsk is not a double --- %f\n", (int)(i + 1),lowAsk);
            //return 1;
        }

        closeBid = json_object_get(data, "closeBid");
        if(!json_is_real(closeBid))
        {
            fprintf(stderr, "error: commit %d: closeBid is not a double --- %f\n", (int)(i + 1),closeBid);
            //return 1;
        }

        closeAsk = json_object_get(data, "closeAsk");
        if(!json_is_real(closeAsk))
        {
            fprintf(stderr, "error: commit %d: closeAsk is not a double --- %f\n", (int)(i + 1),closeAsk);
            //return 1;
        }

        volume = json_object_get(data, "volume");
        if(!json_is_integer(volume))
        {
            fprintf(stderr, "error: commit %d: volume is not a int --- %d\n", (int)(i + 1),volume);
            //return 1;
        }


		tempflag = strncmp(json_string_value(time), oandaYear, 4);
        if(tempflag != 0){
			stopwhileflag = 1;
		printf("\nBREAK\n");
		//getchar();

			break;
		}
		//printf("\nBEFORE fprintf\n");getchar();
		fprintf(fp, "%s,%f,%f,%f,%f,%f,%f,%f,%f,%d\n",json_string_value(time),json_real_value(openBid),json_real_value(openAsk),json_real_value(highBid),json_real_value(highAsk),json_real_value(lowBid),json_real_value(lowAsk),json_real_value(closeBid),json_real_value(closeAsk),json_integer_value(volume));
		linecounter++;


//printf ("\n------%s---\n", json_string_value(time));
	 
//     	 snprintf(temp2, sizeof temp2, "%s", json_string_value(time)); 
	 strcpy(temp2,json_string_value(time));	
	}

	printf("\n%d lines written at the moment  %s \n",linecounter, temp2);
	//getchar();
//printf ("\n------%s---\n", json_string_value(time));
	 
//	 snprintf(temp2, sizeof temp2, "%s", json_string_value(time)); 

	 strcpy(temp3,granularity);
	 
		oandastartdate1 = add_one_period_to_oandatimestr( temp2, temp3);

		//printf("\new time str  %s\n",oandastartdate1);
		//getchar();
    json_decref(root);


//  END OF PARSE AND WRITE ------------------------

        /* always cleanup */
        curl_easy_cleanup(curl);

        /* free the custom headers */ 
        curl_slist_free_all(chunk);

//return 1;
	} // if(curl) {


    } // while(1)
	printf("\n%d lines written\n",linecounter);
	fclose (fp);
	 return 0;
}
