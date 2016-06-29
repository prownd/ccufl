#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "md5.h"

static void MDPrint (MD5_CTX *mdContext){
  int i;
  for (i = 0; i < 16; i++)
    printf ("%02x", mdContext->digest[i]);
}

static void MDString (char *inString){
  MD5_CTX mdContext;
  unsigned int len = strlen (inString);

  MD5Init (&mdContext);
  MD5Update (&mdContext, inString, len);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  printf (" \"%s\"\n\n", inString);
}

static void MDFile (char *filename){
  FILE *inFile = fopen (filename, "rb");
  MD5_CTX mdContext;
  int bytes;
  unsigned char data[1024];

  if (inFile == NULL) {
    printf ("%s can't be opened.\n", filename);
    return;
  }

  MD5Init (&mdContext);
  while ((bytes = fread (data, 1, 1024, inFile)) != 0)
    MD5Update (&mdContext, data, bytes);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  printf (" %s\n", filename);
  fclose (inFile);
}

void main (int argc,char *argv[]){
  printf ("MD5 test suite results:\n\n");
  MDString ("");
  MDString ("abcdefghijklmnopqrstuvwxyz");
  MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MDString("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
  /* Contents of file foo are "abc" */
  MDFile ("foo.txt");
}

