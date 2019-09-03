cmake_minimum_required (VERSION 2.6)
project (ygos)

include_directories ("${PROJECT_SOURCE_DIR}/app/ygsh"
"${PROJECT_SOURCE_DIR}"
"${PROJECT_SOURCE_DIR}/src/driver"
"${PROJECT_SOURCE_DIR}/src/fs"
"${PROJECT_SOURCE_DIR}/src"
"${PROJECT_SOURCE_DIR}/src/deviceNode"
"${PROJECT_SOURCE_DIR}/src/fs/fatfs"
"${PROJECT_SOURCE_DIR}/src/procfs"

)

add_definitions(-g -w -Wall -DDEBUG_ON -DSTD_OUT -DLINUX)

add_executable(ygos 
	src/fs/deviceNode/fs_inode.c 
	src/fs/procfs/fs_procfs_uptime.c 
	src/fs/procfs/fs_proc.c 
	src/fs/fatfs/fs_fat32.c 
	src/fs/fatfs/fs_utils.c
	src/fs/fatfs/fs_utils_arm.c 
	src/fs/fs_mount.c 
	src/driver/ramlog.c 
	app/ygsh/cmd_cat.c  
	app/ygsh/cmd_echo.c 
	app/platform/linux/main.c 
	src/arch/linux/core.c 
	src/fs/fs_driver.c 
	src/fs/fs_syscall.c)
