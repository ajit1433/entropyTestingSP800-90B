#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cerrno>
#include <cstring>

#define ONETIME_BYTES_READ 100000
#define MAX_BYTES_IN_ONE_FILE 1000000
#define MAX_FILES 1000

#define DATA_STORE_PATH "../ExperimentData/DataStore"
#define RAW_DATA_STORE_PATH "../ExperimentData/RawDataStore"

int __min(int a, int b) {
    if (a <= b) return a;
    else return b;
}

int generator(const char *infile) {
    printf("Generating files from %s\n", infile);

    int random_source_fd, random_destination_fd;
    char *dest_file;
    uint8_t buf[ONETIME_BYTES_READ] = {0};
    int total_count = 0;
    ssize_t bytes_read, bytes_written;

    char *source_file_path;
    asprintf(&source_file_path, "%s/%s.bin", RAW_DATA_STORE_PATH, infile);
    random_source_fd = open(source_file_path, O_RDONLY);
    if (random_source_fd == -1) {
        perror("source file opening failed");
        goto _ERROR;
    }

    for (int i = 0; i < MAX_FILES; i++) {

        asprintf(&dest_file, "%s/%s/%05d.bin", DATA_STORE_PATH, infile, i + 1);
        puts(dest_file);

        random_destination_fd = open(dest_file, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (random_destination_fd == -1) {
            perror("destination file opening/creation failed");
            goto _ERROR;
        }

        for (int j = 0; j < MAX_BYTES_IN_ONE_FILE / ONETIME_BYTES_READ; j++) {
            bytes_read = read(random_source_fd, (void *) buf, ONETIME_BYTES_READ);
//            printf("bytes_read: %d\n", bytes_read);
            if (bytes_read == -1 || bytes_read < ONETIME_BYTES_READ) {
                printf("error in reading from file: not enough data in file\n");
                goto _ERROR;
            }

            bytes_written = write(random_destination_fd, buf, bytes_read);
            if (bytes_written == -1 || bytes_written < bytes_read) {
                perror("error in writing to file");
                goto _ERROR;
            }

            total_count += bytes_read;
        }
        bytes_read = read(random_source_fd, (void *) buf, MAX_BYTES_IN_ONE_FILE - total_count);
        if (bytes_read == -1) {
            printf("error in reading from file: not enough data in file\n");
            goto _ERROR;
        }

        bytes_written = write(random_destination_fd, buf, __min(MAX_BYTES_IN_ONE_FILE - total_count, bytes_read));
        if (bytes_written == -1 || bytes_written < bytes_read) {
            perror("error in writing to file");
            goto _ERROR;
        }
        total_count += bytes_read;

        if (total_count < MAX_BYTES_IN_ONE_FILE) {
            printf("not sufficient data in random data file: %s.bin", infile);
            goto _ERROR;
        }

        close(random_destination_fd);
        total_count = 0;
        free(dest_file);
    }
    close(random_source_fd);
    printf("Completed for %s.bin\n", infile);
    return 0;
_ERROR:
    printf("Completed for %s.bin\n", infile);
    if (random_source_fd != -1) close(random_source_fd);
    if (random_destination_fd != -1) close(random_destination_fd);
    return -1;
}

void filename_without_ext(char *filename) {
    int filename_len = strlen(filename);
    filename[filename_len - 4] = 0;
    filename[filename_len - 3] = 0;
    filename[filename_len - 2] = 0;
    filename[filename_len - 1] = 0;
}

int main(int argc, char **argv) {
    printf("starting data generator\n");

    struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(RAW_DATA_STORE_PATH);

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    // int flag = 0;
    while ((de = readdir(dr)) != NULL) {
        // only process real files
        if (de->d_type == 0x08) {

            if (argc == 2)
                if (strcmp(de->d_name, argv[1]) != 0) continue;

            // create directory
            char *full_path = NULL;
            asprintf(&full_path, "%s/%s", DATA_STORE_PATH, de->d_name);

            // remove extension from full path
            filename_without_ext(full_path);

            // filename without ext
            char *filename_wo_ext;
            asprintf(&filename_wo_ext, "%.*s", strlen(de->d_name) - 4, de->d_name);

            int status = mkdir(full_path, S_IRWXU | S_IRWXG | S_IRWXO);
            if (status == 0) {
                printf("%s created\n", full_path);
                generator(filename_wo_ext);
            } else if (status == -1 && errno == EEXIST) {
                printf("%s already exists. continuing...\n", full_path);
                // generator(filename_wo_ext);
            }
            else
                printf("error in creating %s. error code: %02x. continuing...\n", full_path, errno);

            free(full_path);
        }
    }
    closedir(dr);
    printf("Data generation completed\n");
    return 0;
}