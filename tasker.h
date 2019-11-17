/*
 * Tasker for Arduino - cooperative task scheduler with Javascript like API
 * Copyleft (c) 2015-2018  Petr Stehlik  petr@pstehlik.cz
 * Distributed under the GNU LGPL http://www.gnu.org/licenses/lgpl.txt
 */

#ifndef _tasker_h
#define _tasker_h

#include "Arduino.h"
#include "env.h"

//#ifndef TASKER_MAX_TASKS
//#define TASKER_MAX_TASKS 10 // max 254 entries, one occupies 14 bytes of RAM
//#endif

typedef void (*TaskCallback0)(void);
typedef void (*TaskCallback1)(int);

class Tasker
{
public:
    Tasker(int task_count, bool prioritized);

    bool setTimeout(TaskCallback0 func, unsigned long interval, byte prio = TASKER_MAX_TASKS);
    bool setTimeout(TaskCallback1 func, unsigned long interval, int param, byte prio = TASKER_MAX_TASKS);

    bool setInterval(TaskCallback0 func, unsigned long interval, byte prio = TASKER_MAX_TASKS);
    bool setInterval(TaskCallback1 func, unsigned long interval, int param, byte prio = TASKER_MAX_TASKS);

    bool setRepeated(TaskCallback0 func, unsigned long interval, unsigned int repeat, byte prio = TASKER_MAX_TASKS);
    bool setRepeated(TaskCallback1 func, unsigned long interval, unsigned int repeat, int param, byte prio = TASKER_MAX_TASKS);

    bool cancel(TaskCallback0 func);
    bool cancel(TaskCallback1 func, int param);

    bool clearTimeout(TaskCallback0 func) { cancel(func); };
    bool clearTimeout(TaskCallback1 func, int param) { cancel(func, param); };

    bool clearInterval(TaskCallback0 func) { cancel(func); };
    bool clearInterval(TaskCallback1 func, int param) { cancel(func, param); };

    unsigned long scheduledIn(TaskCallback0 func);
    unsigned long scheduledIn(TaskCallback1 func, int param);

    byte loop(void);

    bool isPrioritized() { return t_prioritized; }
    void setPrioritized(bool prioritized) { t_prioritized = prioritized; }

    int pendingTasks() { return t_count; }

private:
    struct TASK
    {
        TaskCallback1 call;
        int param;
        unsigned long interval;
        unsigned long lastRun;
        unsigned int repeat;
    };

    int findTask(TaskCallback0 func);
    int findTask(TaskCallback1 func, int param);
    bool addTask(TaskCallback1 func, unsigned long interval, unsigned int repeat, int param, byte prio);
    bool removeTask(int t_idx);

    TASK *tasks;
    byte t_count;
    byte t_task_count;
    bool t_prioritized;
    static const int NO_PARAMETER = -1;
};

#endif // _tasker_h