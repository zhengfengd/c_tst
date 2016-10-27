#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main( int argc, char **argv )
{
        char *ret;
        if(setlocale(LC_CTYPE, "") == NULL)
        //if((ret = setlocale(LC_ALL, "zh_CN.UTF-8")) == NULL)
        { /*ÉèÖÃÎª±¾µØ»·¾³±äÁ¿¶¨ÒåµÄlocale*/
                fprintf(stderr, "can't set the locale\n");
        } else
        {
                fprintf(stdout, "ret is %s\n", ret);
        }
        wchar_t hi[]=L"ÄãÊÇË­£¿";
        wprintf(L"%ls\n", hi);
        wprintf("%wc\n", 0x7ed3);

        return 0;

}

