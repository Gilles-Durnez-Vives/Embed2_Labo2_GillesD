#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <mysql.h>

// Start van uitbreiding

#define MAXLEN 150
#define EXTRA 10                       /* 9 for field name "textToAdd", 1 for "="  --> 10*/
#define MAXINPUT MAXLEN+EXTRA+2

static void print_http_header (const char * content_type)
{
    printf ("Content-Type: %s\n\n", content_type);
}

void unencode(char *src, char *last, char *dest)
{
  for(; src != last; src++, dest++)
    if(*src == '+')
        *dest = ' ';
    else if(*src == '%') {
        int code;
        if(sscanf(src+1, "%2x", &code) != 1) code = '?';
        *dest = code;
        src +=2; }
    else
        *dest = *src;
  *dest = ' ';
  *++dest = '\0';
}

int main()
{
  char *lenstr;
  char *cookie;
  char* cookieData;
  char JsonURL[30];
  char input[MAXINPUT], data[MAXINPUT];
  long len;

  FILE *JsonFile;
  char JsonBuffer[MAXINPUT+12] = "";

  MYSQL *con = mysql_init(NULL);
  MYSQL_RES* result;
  MYSQL_ROW row;
  char queryString [170];

  struct tm *timeinfo ;
  time_t rawtime ;
  char currentTime [128];

  // Print Header
  print_http_header("text/html");
  printf("<TITLE>Response</TITLE>\n");

  lenstr = getenv("CONTENT_LENGTH");
  cookie = getenv("HTTP_COOKIE");
  cookieData = cookie+12;             // 12 voor "usernameCGI=" (cookie name)

  // Initialise MySQL DB
  if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", "labo2GillesDurnez", "labo2GillesDurnezPW", "labo2GD", 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
  
  if(lenstr == NULL || sscanf(lenstr,"%ld",&len)!=1 || len > MAXLEN)
    printf("<P>Error in invocation - wrong FORM probably.");
  else 
  {
    fgets(input, len+1, stdin);
    unencode(input+EXTRA, input+len, data);

    // Get current time to add to file
    rawtime = time (NULL) ;
    timeinfo = localtime(&rawtime) ;  
    strftime(currentTime,128,"%H:%M:%S %d-%b-%Y",timeinfo);

    ////////////////////////////////////////////////////
    // Update the json file with the new comments.
    sprintf(JsonURL,"/var/www/html/%s.json",cookieData);
    
    // Check if json exists
    if (access(JsonURL, F_OK) != 0)
    {
      // json does not exist make framework
      JsonFile = fopen(JsonURL, "w");
      sprintf(JsonBuffer, "{\"Comments\":[[\"%s\",\"%s\"]]}", data, currentTime);
      fseek(JsonFile, 0, SEEK_SET);
    }
    else
    {
      // json exists append to json
      JsonFile = fopen(JsonURL, "r+");
      sprintf(JsonBuffer, ",[\"%s\",\"%s\"]]}", data, currentTime);
      fseek(JsonFile, -2, SEEK_END);
    }

    fputs(JsonBuffer, JsonFile);
    fclose(JsonFile);
  	
    /////////////////////////////////////////////////////
    // Push new data to DB
    sprintf(queryString,"INSERT INTO comments (time, username, comment) VALUES('%s','%s','%s');",currentTime,cookieData,data);
    if (mysql_query(con, queryString))
    {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
    }

    // Get All data from DB to json file.
    char sqlBuffer [20000] = "{\"Comments\":[";
    char rowBuffer [200];
    int count = 0;
    // Get data from DB
    mysql_query(con, "SELECT * FROM comments");
    result = mysql_store_result(con);
    while((row = mysql_fetch_row(result)))
    {
      // Chek for start of json
      if(count == 1)
      {
        // append to json with ,
        sprintf(rowBuffer,",[\"%s\",\"%s\",\"%s\"]",row[0],row[1],row[2]);
      }
      else
      {
        // Start of json no , needed
        sprintf(rowBuffer,"[\"%s\",\"%s\",\"%s\"]",row[0],row[1],row[2]);
        count++;
      }
      strcat(sqlBuffer,rowBuffer);
    }
    // Close json and push to file
    strcat(sqlBuffer,"]}");
    JsonFile = fopen("/var/www/html/data.json", "w");
    fseek(JsonFile, 0, SEEK_SET);

    fputs(sqlBuffer, JsonFile);
    fclose(JsonFile);

    printf("<P>Thank you! The following contribution of yours has been stored:<BR><p>%s</p>",data);
    printf("<a href=\"#\" onclick=\"javascript:window.history.back(-1);return false;\">Return (please refresh page after return to see results)</a>");
  }
  return 0;
}
