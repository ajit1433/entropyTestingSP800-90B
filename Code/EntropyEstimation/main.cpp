#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdlib>
#include <sys/stat.h>
#include "SP800-90B_EntropyAssessment/cpp/non_iid_main.h"

#define DATA_STORE_PATH "../ExperimentData/DataStore"
#define SP80090B_OUTPUT_PATH "../ExperimentData/SP80090B_Output"

int main(int argc, char **argv) {
    printf("Starting...\n");

    struct dirent *de;

    DIR *dr = opendir(DATA_STORE_PATH);

    if (dr == NULL) {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL) {
        // only process directories
        if (de->d_type == 0x04) {

            if (strcmp(de->d_name, ".") == 0) continue;
            if (strcmp(de->d_name, "..") == 0) continue;

            if (argc == 2)
                if (strcmp(de->d_name, argv[1]) != 0) continue;

            // create directory
            char *full_path = NULL;
            asprintf(&full_path, "%s/%s", SP80090B_OUTPUT_PATH, de->d_name);

            int status = mkdir(full_path, S_IRWXU | S_IRWXG | S_IRWXO);
            if (status == 0) {
                printf("%s created\n", full_path);
                driver_entropy_estimation(de->d_name, false);
            } else if (status == -1 && errno == EEXIST) {
                printf("%s already exists. continuing...\n", full_path);
                // driver_entropy_estimation(de->d_name, false);
            }
            else
                printf("error in creating %s. error code: %02x. continuing...\n", full_path, errno);

            free(full_path);
        }
    }
    closedir(dr);
    printf("Complete...\n");
    return 0;
}