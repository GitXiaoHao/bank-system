
#include "include/bank.h"

int main() {
    pthread_t th1;
    pthread_create(&th1, NULL, graph_theme, NULL);
    pthread_join(th1, NULL);
    return 0;
}
