# 中断
## 中断向量表的方法
> 本os兼容2种使用方法

## 前512字节
	.section "RESET", text
	.align	512
	jr32	__start ; RESET

	.align	16
	syncp
	jr32	_Dummy ; SYSERR

	.align	16
	jr32	_Dummy

	.align	16
	jr32	_Dummy ; FETRAP

##  从下512字节开始
    .section "EIINTTBL", const
	.align	512
	.offset (0x22*4)
	.dw	#_INTRLIN30UR0
	.offset (0x38*4)
	.dw	#_INTADCA0ERR
	.offset (0x51*4)
	.dw	#_INTTAUJ0I1
	.offset (0x54*4)
	.dw	#_ei_interrupt_common
	.offset (0x013B*4)
	.dw	#_INTETNB0DATA


# 设置中断向量表基地址
> 中断向量表设置在段EIINTTBL中，通道号*4： 通道范围是0~357
> Exception handler address read position = INTBP register + channel number × 4 bytes

```
.section "EIINTTBL", const
	.align	512

```

```
mov	#__sEIINTTBL, r6
jarl	_set_table_reference_method, lp ; set table reference method

	.align	2
_set_table_reference_method:
	ldsr	r6, 4, 1		; set INTBP
```

# 中断基础知识
## FEINT
OSTM0, OSTM1,OSTM2,OSTM3等16个中断可以作为FEINT中断
INTOSTM0 can operate as an EIINT or FEINT interrupt, but using it both ways at the same time is
prohibited

## 中断屏蔽寄存器
0xffffb0a8  INTC2.OSTM 设置0x4f

## 优先级
优先级越低越高,是从0到15
EIINT0到EIINT511和FPI的优先级取决于寄存器设置
1000H to 11FFH (channels 0 to 511) are selected according to the channel.

## 中断
>所有异常和中断都是关闭中断的IDbit位，想要实现中断
FEINT 优先级:3 
EIINT 优先级:4
FEINT> EIINT> TRAP

## 设置中断优先级
bit3-bit0: 设置中断优先级
void R_INTC_SetPriority(volatile uint16_t *IC_ptr, uint8_t Priority)
{
    Priority &= 0x07;
    *IC_ptr &= ~0x0007;
    *IC_ptr |= Priority;
}

## 中断向量表
bit6: 设置当前中断选择直接地址还是中断向量表
void R_INTC_SetTableBit(volatile uint16_t *IC_ptr)
{
    *IC_ptr |= 0x40;
}

## 中断请求标志位
bit12: 使能中断请求
void R_INTC_SetRequestFlag(volatile uint16_t *IC_ptr)
{
    *IC_ptr |= 0x1000;
}
