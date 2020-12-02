#include <sys/time.h>

using namespace std;

inline long long GetCurrentTimeMillis() {
    timeval t;
    gettimeofday(&t, NULL);

    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
}
