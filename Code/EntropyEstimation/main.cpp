#include "SP800-90B_EntropyAssessment/cpp/non_iid_main.h"

#define __ATECC508A__ 1
#define __URANDOM__ 1
#define __SHA2_256__ 1
#define __SHA3_256__ 1

int main() {
    printf("Starting...\n");

#if __SHA3_256__
    driver_entropy_estimation("sha3_256", false);
#endif

#if __SHA2_256__
    driver_entropy_estimation("sha2_256", false);
#endif

#if __URANDOM__
    driver_entropy_estimation("urandom", false);
#endif

#if __ATECC508A__
    driver_entropy_estimation("atecc508a", false);
#endif
    printf("Complete...\n");
}