//
// Created by maks on 12.07.2025.
//

#ifndef GL4ES_WRAPPER_ENV_H
#define GL4ES_WRAPPER_ENV_H

#include <stdbool.h>
#include <stddef.h>

bool env_istrue(const char* name);
bool env_istrue_d(const char* name, bool _default);
size_t detect_device_memory_mb(void);

#endif //GL4ES_WRAPPER_ENV_H
