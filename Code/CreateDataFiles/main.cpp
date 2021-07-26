#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

#define __ATECC508A__ 1
#define __URANDOM__ 1
#define __SHA2_256__ 1
#define __SHA3_256__ 1

#define ONETIME_BYTES_READ 1000
#define MAX_BYTES_IN_ONE_FILE 1000000
#define MAX_FILES 1000

int generator(const char *infile) {
    int random_source_fd, random_destination_fd;
    uint8_t buf[ONETIME_BYTES_READ] = {0};
    int total_count = 0;
    ssize_t bytes_read;

    char *source_file_path;
    asprintf(&source_file_path, "../ExperimentData/RawDataStore/%s_full.bin", infile);
    random_source_fd = open(source_file_path, O_RDONLY);
    if (random_source_fd == -1) perror("source file opening failed");

    for (int i = 0; i < MAX_FILES; i++) {
        char *dest_file;

        asprintf(&dest_file, "../ExperimentData/DataStore/%s/%05d.bin", infile, i + 1);
        puts(dest_file);

        random_destination_fd = open(dest_file, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        if (random_destination_fd == -1) perror("destination file opening/creation failed");

        for (int j = 0; j < MAX_BYTES_IN_ONE_FILE / ONETIME_BYTES_READ; j++) {
            bytes_read = read(random_source_fd, (void *) buf, ONETIME_BYTES_READ);

            write(random_destination_fd, buf, bytes_read);
            total_count += bytes_read;
        }
        bytes_read = read(random_source_fd, (void *) buf, MAX_BYTES_IN_ONE_FILE - total_count);

        write(random_destination_fd, buf, MAX_BYTES_IN_ONE_FILE - total_count);

        total_count += bytes_read;

        if (total_count < MAX_BYTES_IN_ONE_FILE) {
            printf("not sufficient data in random data file: %s_full.bin", infile);
            return -1;
        }

        close(random_destination_fd);
        total_count = 0;
        free(dest_file);
    }
    close(random_source_fd);
    printf("Completed for %s_full.bin\n", infile);
}

int main() {
    printf("starting data generator\n");

#if __ATECC508A__
    generator("atecc508a");
#endif // __ATECC508A__


#if __URANDOM__
    generator("urandom");
#endif // __URANDOM__


#if __SHA2_256__
    generator("sha2_256");
#endif // __SHA356_DERIVED__

#if __SHA3_256__
    generator("sha3_256");
#endif  // __SHA3_356_DERIVED__


    printf("Data generation completed\n");
    return 0;
}