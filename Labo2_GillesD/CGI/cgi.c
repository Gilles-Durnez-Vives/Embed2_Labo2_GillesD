#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql.h>

#define MAXLEN 150
#define EXTRA 10                       /* 9 for field name "data", 1 for "="  --> 10*/
#define MAXINPUT MAXLEN+EXTRA+2
#define DATAFILE "../data/data.json"

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
  *dest = '\n';
  *++dest = '\0';
}

int main()
{
  char *lenstr;
  char input[MAXINPUT], data[MAXINPUT];
  long len;

  print_http_header("text/html");
  printf("<TITLE>Response</TITLE>\n");

  lenstr = getenv("CONTENT_LENGTH");
  
  if(lenstr == NULL || sscanf(lenstr,"%ld",&len)!=1 || len > MAXLEN)
    printf("<P>Error in invocation - wrong FORM probably.");
  else 
  {
    fgets(input, len+1, stdin);
    unencode(input+EXTRA, input+len, data);
    printf("<P>Thank you! The following contribution of yours has been stored:<BR>%s",data);
  }
  return 0;
}
