#简介

本单板是基于米尔科技的MYS-6UY2-256N256D-50-I-IND

MYS-6ULX-IOT 是米尔科技推出的基于NXP公司 I.MX6ULL 系列处理器的面向物联网
市场推出的单板计算机。I.MX6ULL 是一个高功效、高性价比的应用处理器系列，采用单个
ARM Cortex-A7 内核，运行速度高达 528 MHz。该 系列的每个处理器提供多种存储器接口，
其中包括 16 位 LPDDR2、DDR3、DDR3L、NAND Flash、NOR Flash、eMMC、Quad SPI
和各种其他接口，用于连接外围设备

<img src="./doc/mys-6ul.png" width = "300" height = "120" alt="" />

## imx-6ull的LED管脚
低电平点亮LED, 高电平关闭LED,
<img src="./doc/mys-6ull-led.png" width = "650" height = "120" alt="" />


# 构建过程
烧写固件到sd卡
```
$ make 
$ ./download /dev/sdb 
```

<img src="./doc/mys-ygos.png" width = "480" height = "240" alt="" />

