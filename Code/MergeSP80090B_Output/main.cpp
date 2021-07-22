#include <stdio.h>
#include <string.h>

#define __ATECC508A__ 1
#define __URANDOM__ 1
#define __SHA2_256__ 1
#define __SHA3_256__ 1

#define MAX_FILES 1000

#define LOG_TO_FILE 1

void merge(const char *parentdir) {

    char infile[100] = {0};
    char outfile[100] = {0};
    char line[1000];

    memset(outfile, 0, 100);
    snprintf(outfile, 100, "../Results/%s_combined_results.csv", parentdir);

    FILE *outfp = fopen(outfile, "w+");

    for (int i = 1; i <= MAX_FILES; i++) {
        memset(infile, 0, 100);
        snprintf(infile, 100, "../SP80090B_Output/%s/%05d.csv", parentdir, i);

        FILE *fp = fopen(infile, "r");
        if (fp == NULL) {
            perror("error opening file");
            return;
        }

        /* Get the number of bytes */
        fseek(fp, 0L, SEEK_END);
        int numbytes = ftell(fp);

        /* reset the file position indicator to
        the beginning of the file */
        fseek(fp, 0L, SEEK_SET);

        memset(line, 0, 1000);
        snprintf(line, 1000, "%05d, ", i);
        fread((void *)(line+7), 1, numbytes, fp);

#if LOG_TO_FILE
        fwrite((void *)line, 1, strlen(line), outfp);
#endif

        fclose(fp);
    }

    fclose(outfp);
}
int main() {
    printf("starting merging\n");

#if __ATECC508A__
    merge("atecc508a");
#endif

#if __URANDOM__
    merge("urandom");
#endif

#if __SHA2_256__
    merge("sha2_256");
#endif

#if __SHA3_256__
    merge("sha3_256");
#endif

    printf("merging complete\n");
    return 0;
}