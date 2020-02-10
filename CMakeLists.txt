cmake_minimum_required (VERSION 2.6)
project (ygos)

include_directories ("${PROJECT_SOURCE_DIR}/app/ygsh"
"${PROJECT_SOURCE_DIR}/src/include/"
)

add_definitions(-g -w -Wall -DDEBUG_ON -DSTD_OUT -DLINUX -DGCC_BUILD)

add_executable(ygos 
	src/fs/deviceNode/fs_inode.c 
	src/fs/procfs/fs_procfs_uptime.c 
	src/fs/procfs/fs_proc.c 
	src/fs/fatfs/fs_fat32.c 
	src/fs/fatfs/fs_utils.c
	src/fs/fatfs/fs_utils_arm.c 
	src/fs/fs_mount.c 
	src/fs/fs_driver.c 
	src/fs/fs_syscall.c

	src/mm/malloc.c

	src/driver/ramlog.c 

	app/ygsh/cmd_cat.c  
	app/ygsh/cmd_echo.c 
	app/platform/linux/main.c 

	src/arch/linux/core.c 
	)
