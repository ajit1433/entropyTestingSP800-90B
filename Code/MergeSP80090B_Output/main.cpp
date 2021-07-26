#include <stdio.h>
#include <string.h>

#define __ATECC508A__ 1
#define __URANDOM__ 1
#define __SHA2_256__ 1
#define __SHA3_256__ 1

#define END_FILE_NUM 1000

#define __WRITE_TO_FILE__ 1

void merge(const char *parentdir) {

    char infile[100] = {0};
    char outfile[100] = {0};
    char line[1000];

    memset(outfile, 0, 100);
    snprintf(outfile, 100, "../ExperimentData/Results/%s_combined_results.csv", parentdir);
    puts(outfile);

    FILE *outfp = fopen(outfile, "w+");
    if (outfp == NULL) {
        perror("error opening output file");
        return;
    }

    // add headers to the file
#if __WRITE_TO_FILE__
    char *headers;
    asprintf(&headers, "File Number, Most Common Value, Collision Test, Markov Test, Compression Test, T-Tuple Test, Longest Repeated Substring Test, Multi Most Common in Window (MultiMCW) Prediction Test, Lag Prediction Test, Multi Markov Model with Counting (MultiMMC) Prediction Test, LZ78Y Prediction Test, Final Entropy\n");
    fwrite((void *)headers, 1, strlen(headers), outfp);
#endif

    // start adding data from individual files
    for (int i = 1; i <= END_FILE_NUM; i++) {
        memset(infile, 0, 100);
        snprintf(infile, 100, "../ExperimentData/SP80090B_Output/%s/%05d.csv", parentdir, i);
        puts(infile);
        FILE *fp = fopen(infile, "r");
        if (fp == NULL) {
            perror("error opening file");
            goto _ERROR;
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

#if __WRITE_TO_FILE__
        fwrite((void *)line, 1, strlen(line), outfp);
#endif

        fclose(fp);
    }

    fclose(outfp);
_ERROR:
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