#!/bin/bash

# LTW-Turbo 完整 APK 构建脚本
# 整合 LTW 库构建和 Capacitor APK 构建流程

set -e  # 遇到错误立即退出

echo "🚀 开始构建 LTW-Turbo 完整 APK..."

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 项目根目录
PROJECT_ROOT=$(pwd)
LTW_BUILD_DIR="$PROJECT_ROOT/ltw/build/outputs/aar"
CAPACITOR_APP_DIR="$PROJECT_ROOT/capacitor-android/app"
JNILIBS_DIR="$CAPACITOR_APP_DIR/src/main/jniLibs"

# 步骤1：构建 LTW 库
echo -e "${YELLOW}📦 步骤1：构建 LTW 库...${NC}"
if ! ./gradlew :ltw:assembleRelease; then
    echo -e "${RED}❌ LTW 库构建失败${NC}"
    exit 1
fi
echo -e "${GREEN}✅ LTW 库构建成功${NC}"

# 步骤2：检查 AAR 文件
AAR_FILE="$LTW_BUILD_DIR/ltw-release.aar"
if [ ! -f "$AAR_FILE" ]; then
    echo -e "${RED}❌ 找不到 AAR 文件：$AAR_FILE${NC}"
    exit 1
fi

# 步骤3：创建临时目录并解压 AAR
TEMP_DIR=$(mktemp -d)
echo -e "${YELLOW}📂 步骤2：解压 AAR 文件...${NC}"
cd "$TEMP_DIR"
unzip -q "$AAR_FILE"

# 步骤4：复制 SO 文件到 jniLibs
echo -e "${YELLOW}📋 步骤3：复制 SO 文件到 jniLibs...${NC}"
cd "$PROJECT_ROOT"

# 确保目标目录存在
mkdir -p "$JNILIBS_DIR"/{armeabi-v7a,arm64-v8a,x86_64}

# 复制各架构的 SO 文件
for arch in armeabi-v7a arm64-v8a x86_64; do
    if [ -f "$TEMP_DIR/jni/$arch/libltw_turbo.so" ]; then
        cp "$TEMP_DIR/jni/$arch/libltw_turbo.so" "$JNILIBS_DIR/$arch/"
        echo -e "${GREEN}  ✅ 复制 $arch/libltw_turbo.so${NC}"
    else
        echo -e "${YELLOW}  ⚠️  警告：未找到 $arch/libltw_turbo.so${NC}"
    fi
done

# 清理临时目录
rm -rf "$TEMP_DIR"

# 步骤5：同步 Capacitor
echo -e "${YELLOW}🔄 步骤4：同步 Capacitor...${NC}"
cd "$PROJECT_ROOT"
if ! npx cap sync android; then
    echo -e "${RED}❌ Capacitor 同步失败${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Capacitor 同步成功${NC}"

# 步骤6：构建 APK
echo -e "${YELLOW}🏗️  步骤5：构建 APK...${NC}"
cd "$PROJECT_ROOT/capacitor-android"
if ! ./gradlew assembleDebug; then
    echo -e "${RED}❌ APK 构建失败${NC}"
    exit 1
fi

# 步骤7：检查生成的 APK
APK_FILE="$PROJECT_ROOT/capacitor-android/app/build/outputs/apk/debug/app-debug.apk"
if [ -f "$APK_FILE" ]; then
    APK_SIZE=$(du -h "$APK_FILE" | cut -f1)
    echo -e "${GREEN}🎉 构建成功！${NC}"
    echo -e "${GREEN}📱 APK 文件：$APK_FILE${NC}"
    echo -e "${GREEN}📏 文件大小：$APK_SIZE${NC}"
    
    # 验证 APK 中的 SO 文件
    echo -e "${YELLOW}🔍 验证 APK 内容...${NC}"
    if unzip -l "$APK_FILE" | grep -q "libltw_turbo.so"; then
        echo -e "${GREEN}✅ APK 中包含 libltw_turbo.so${NC}"
    else
        echo -e "${RED}❌ 警告：APK 中未找到 libltw_turbo.so${NC}"
    fi
else
    echo -e "${RED}❌ 找不到生成的 APK 文件${NC}"
    exit 1
fi

echo -e "${GREEN}🎊 LTW-Turbo APK 构建完成！${NC}"