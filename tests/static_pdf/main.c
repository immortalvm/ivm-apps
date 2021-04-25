
#include "ierrors.h"
#include "iapi.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

// Static PDF data:
// size filename
// 79160025 a_tale_of_two_cities_pdfa.h
//   525372 guide_pdfa.h
// 16371888 nou_2019-11_pdfa.h
//#include "guide_pdfa.h"
//#define PDF_SIZE source_pdf_guide_pdfa_pdf_len
//#define PDF_DATA source_pdf_guide_pdfa_pdf

#include "nou_2019-11_pdfa.h"
#define PDF_SIZE source_pdf_nou_2019_11_pdfa_pdf_len
#define PDF_DATA source_pdf_nou_2019_11_pdfa_pdf

unsigned int PDF_POS = 0;

static int read_stdin(void *caller_handle, char *buf, int len) {
    if (PDF_POS == PDF_SIZE) {
        printf("reading EOF\n");
        return 0;
    }
    if (PDF_POS > PDF_SIZE) {
        printf("reading error\n");
        return -1;
    }
    printf("reading %d\n", len);
    
    size_t l = PDF_POS + len > PDF_SIZE ? PDF_SIZE - PDF_POS : len;
    memcpy(buf, PDF_DATA+PDF_POS, l);
    PDF_POS += l;
    return l;
}

int stdout_fn(void *caller_handle, const char *str, int len)
{
    printf("%s", str);
}

int stderr_fn(void *caller_handle, const char *str, int len)
{
    printf("%s", str);
}

// PDF library instance
void *minst = NULL;

int main(int argc, char *argv[])
{
    int code, code1;
    char * gsargv[7];
    int gsargc;
    int ac = 0;
    gsargv[ac++] = ""; // Ignored by API
    //gsargv[ac++] = "-dNOPAUSE";
    //gsargv[ac++] = "-dBATCH";
    gsargv[ac++] = "-dSAFER";
    gsargv[ac++] = "-dDEBUG"; // Must link with gs debug
    gsargv[ac++] = "-sDEVICE=ivm64";
    gsargv[ac++] = "-sOutputFile=-";
    gsargv[ac++] = "-";
    //gsargv[ac++] = "--debug";
    gsargc=ac;

    printf("Creating instance\n");
    code = gsapi_new_instance(&minst, NULL);
    if (code < 0) {
        printf("Error creating instance\n");
        return 1;
    }
    
    printf("Set encoding\n");
    if (gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_UTF8) == 0) {
        printf("Set stdio\n");
        if (gsapi_set_stdio(minst,read_stdin, stdout_fn, stderr_fn) == 0) {
            printf("Init\n");
            if ( gsapi_init_with_args(minst, gsargc, gsargv) == 0) {
                printf("Exit\n");
                code = gsapi_exit(minst);
            }
        }
    }


    gsapi_delete_instance(minst);

    if ((code == 0) || (code == gs_error_Quit))
        return 0;
    return 1;
}

