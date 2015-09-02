#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int
noCaseStrcmp(const char *s, const char *t)
{
  for (; tolower(*s) == tolower(*t); s++, t++)
    if (*s == '\0')
      return 0;
  return (int) (tolower(*s) - tolower(*t));
}

static char strone[32];
static char strtwo[32];

int main(int argc, char* argv[]) {
	printf("comparing \"this\" and \"THIS\": ");
	strcpy(strone, "this");
	strcpy(strtwo, "THIS");
	printf("%d\n", noCaseStrcmp(strone, strtwo));

	printf("comparing \"THIS\" and \"THIS\": ");
	strcpy(strone, "THIS");
	strcpy(strtwo, "THIS");
	printf("%d\n", noCaseStrcmp(strone, strtwo));

	printf("comparing \"this\" and \"this\": ");
	strcpy(strone, "this");
	strcpy(strtwo, "this");
	printf("%d\n", noCaseStrcmp(strone, strtwo));

	printf("comparing \"\" and \"this\": ");
	strone[0] = '\0';
	strcpy(strtwo, "this");
	printf("%d\n", noCaseStrcmp(strone, strtwo));

	return EXIT_SUCCESS;
}

	
	
