#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#define END_FILE_NUM 1000

#define __WRITE_TO_FILE__ 1
#define RESULTS_OUTPUT_PATH "../ExperimentData/Results"
#define SP80090B_OUTPUT_PATH "../ExperimentData/SP80090B_Output"

void merge(const char *parentdir) {

    char infile[100] = {0};
    char outfile[100] = {0};
    char line[1000];

    memset(outfile, 0, 100);
    snprintf(outfile, 100, "%s/%s_combined_results.csv", RESULTS_OUTPUT_PATH, parentdir);
    puts(outfile);

    FILE *outfp = fopen(outfile, "w+");
    if (outfp == NULL) {
        perror("error opening output file");
        goto _ERROR;
    }

    // add headers to the file
#if __WRITE_TO_FILE__
    char *headers;
    // asprintf(&headers, "File Number,Most Common Value,Collision Test,Markov Test,Compression Test,T-Tuple Test,Longest Repeated Substring Test,Multi Most Common in Window (MultiMCW) Prediction Test,Lag Prediction Test,Multi Markov Model with Counting (MultiMMC) Prediction Test,LZ78Y Prediction Test,Per Bit Entropy,Final Entropy\n");
     asprintf(&headers, "File Number,6.3.1,6.3.2,6.3.3,6.3.4,6.3.5,6.3.6,6.3.7,6.3.8,6.3.9,6.3.10,PBME,FE,\n");

    fwrite((void *)headers, 1, strlen(headers), outfp);
#endif

    // start adding data from individual files
    for (int i = 1; i <= END_FILE_NUM; i++) {
        memset(infile, 0, 100);
        snprintf(infile, 100, "%s/%s/%05d.csv", SP80090B_OUTPUT_PATH, parentdir, i);
        puts(infile);
        FILE *fp = fopen(infile, "r");
        if (fp == NULL) {
            printf("%s", infile);
            perror("error opening file");
            continue;
        }

        /* Get the number of bytes */
        fseek(fp, 0L, SEEK_END);
        int numbytes = ftell(fp);

        /* reset the file position indicator to
        the beginning of the file */
        fseek(fp, 0L, SEEK_SET);

        memset(line, 0, 1000);
        snprintf(line, 1000, "%05d,", i);
        fread((void *)(line+6), 1, numbytes, fp);

#if __WRITE_TO_FILE__
        fwrite((void *)line, 1, strlen(line), outfp);
#endif

        fclose(fp);
    }

    fclose(outfp);
    return;
_ERROR:
    fclose(outfp);
    return;
}

int main() {
    printf("starting merging\n");
    struct dirent *de;

    DIR *dr = opendir(SP80090B_OUTPUT_PATH);

    if (dr == NULL) {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL) {
        // only process directories
        if (de->d_type == 0x04) {

            if (strcmp(de->d_name, ".") == 0) continue;
            if (strcmp(de->d_name, "..") == 0) continue;

            // create directory
            char *full_path = NULL;
            asprintf(&full_path, "%s/%s", SP80090B_OUTPUT_PATH, de->d_name);

            // check if older data exists
            char *dest_file;
            asprintf(&dest_file, "%s/%s_combined_results.csv", RESULTS_OUTPUT_PATH, de->d_name);
            int res = access(dest_file, R_OK | F_OK);
            if (res != -1) {
                printf("Result for %s already exists.\n\tTo re-compute results, first delete %s then re-run the program.\n", full_path, dest_file);
                continue;
            }
            merge(de->d_name);

            free(full_path);
            free(dest_file);
        }
    }
    closedir(dr);
    printf("merging complete\n");
    return 0;
}