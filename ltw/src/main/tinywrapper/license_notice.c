/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：版权声明嵌入。
 *
 * 往最终二进制里放置一条持久可见的 LGPL 版权与构建时间字符串。
 */
// Persistent notice that gets added into the final binary
const volatile char* LICENSE_NOTICE = "Copyright (c) 2025 artDev, SerpentSpirale, CADIndie. For use under LGPL-3.0. "
                                      "Build date: "__DATE__" "__TIME__;
