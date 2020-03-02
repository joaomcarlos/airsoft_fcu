#ifndef GLOBAL_SOURCE
#define GLOBAL_SOURCE
#include "env.h"
#include "global.h"
#include "tasker.h"
Tasker tasker(TASKER_MAX_TASKS, false);
Tasker shooting_tasker(TASKER_MAX_PRIO_TASKS, false);
#endif