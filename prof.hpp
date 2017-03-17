#include <time.h>
#include <string.h>
#include <stdlib.h>

#ifdef __PROF__

#define PROF_FUNC_NUM 10


#define PROF_GLOBAL_INIT(...) \
    Prof __g_prof; \
    const char *__g_prof_names[PROF_FUNC_NUM] = {__VA_ARGS__}

#define PROF_LOCAL_INIT \
    ProfTimer __prof_timers[PROF_FUNC_NUM]

#define PROF_START(func_num) ProfStart(__prof_timers, func_num)
#define PROF_FINISH(func_num) ProfFinish(__prof_timers, func_num)

#define PROF(func_num, ...) do{\
    PROF_START(func_num);\
    __VA_ARGS__;\
    PROF_FINISH(func_num);\
}while(0)

#define PROF_PRINT(s, e) ProfPrint(s, e)

struct ProfStat{
    unsigned long long elapsed_time; 
    unsigned long long cnt; 
};

struct Prof{
    ProfStat time_array[PROF_FUNC_NUM];
};

struct ProfTimer{
    timespec start; 
    timespec end; 
};

extern Prof __g_prof;
extern const char *__g_prof_names[PROF_FUNC_NUM];

inline void ProfStart(ProfTimer *timers, int func_num){
    clock_gettime(CLOCK_MONOTONIC, &timers[func_num].start);
}

inline void ProfFinish(ProfTimer *timers, int func_num){
    unsigned long long timedelay = 0;
    ProfStat *ps = &__g_prof.time_array[func_num];
    ProfTimer *tm = &timers[func_num];

    clock_gettime(CLOCK_MONOTONIC, &tm->end);
    timedelay = 1000000000ULL * (tm->end.tv_sec - tm->start.tv_sec) +
                tm->end.tv_nsec - tm->start.tv_nsec;
    __sync_fetch_and_add(&ps->elapsed_time, timedelay);
    __sync_fetch_and_add(&ps->cnt, 1);
}

inline void ProfPrint(int start, int end){
    fprintf(stderr,
            "======================PROFILE RESULT=======================\n");
    fprintf(stderr, " %15s %15s %10s %15s\n",
            "Function Name", "Total Time(us)", "Count", "Avg. Time(us)");

    ProfStat *ps = __g_prof.time_array;
    for (int i = start; i <= end; i++){
        fprintf(stderr, " %15s %15.3lf %10llu %15.3lf\n",
                __g_prof_names[i], ps[i].elapsed_time/1000.0,
                ps[i].cnt, ps[i].elapsed_time / 1000.0 / ps[i].cnt);
    }
    fprintf(stderr,
            "===========================================================\n");
}

#else /* #ifndef __PROF__ */
#define PROF_GLOBAL_INIT(...)
#define PROF_LOCAL_INIT
#define PROF_START(func_num)
#define PROF_FINISH(func_num)
#define PROF_PRINT(s, e) 
#define PROF(func_num, ...) __VA_ARGS__

#define ProfStart(...) 
#define ProfFinish(...)
#endif /* __NPROF__ */
