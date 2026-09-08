/**
 * LTW 共享配置读取（借鉴 MobileGlues 的“应用写文件、渲染库读文件”思路，
 * 但实现独立：只读我们自己的 /sdcard/LTW-Turbo/config.json 扁平 JSON）。
 *
 * 优先级：环境变量 > 共享配置文件 > 内置默认值。
 */
#ifndef LTW_TURBO_CONFIG_H
#define LTW_TURBO_CONFIG_H

#include <stdbool.h>

// 初始化/刷新配置（同一进程只读一次；失败时后续读取直接返回默认值）。
bool ltw_config_init(void);

bool ltw_config_get_bool(const char* key, bool def);
int ltw_config_get_int(const char* key, int def);
const char* ltw_config_get_string(const char* key, const char* def);

void ltw_config_cleanup(void);

#endif //LTW_TURBO_CONFIG_H
