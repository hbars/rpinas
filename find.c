
#include "common.h"
#include "func.h"

char *strmatch_regex(const char *chk_string, const char *pattern)
{
int ret;
regex_t preg;
regmatch_t pmatch[10];
static char result[BUFSIZ];
int i;
int len;

ret = regcomp(&preg, pattern, REG_ICASE | REG_EXTENDED);
if (ret != 0)
	return NULL;
ret = regexec(&preg, chk_string, 10, pmatch, REG_NOTBOL);
regfree(&preg);
if (ret == REG_NOMATCH) return NULL;
for (i = 0; pmatch[i].rm_so != -1; i++)
    {
      len = pmatch[i].rm_eo - pmatch[i].rm_so;
      memcpy(result, chk_string + pmatch[i].rm_so, len);
      result[len] = 0;
    }

if (pmatch[0].rm_so != -1)
	return (result);
    else
	return NULL;
}
