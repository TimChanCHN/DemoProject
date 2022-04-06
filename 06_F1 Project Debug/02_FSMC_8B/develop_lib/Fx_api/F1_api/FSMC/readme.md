# Readme
1. 当外设是16bit设备时，STM32的FSMC会对内部地址(HADDR[x:0])右移一位输出到外部设备(FSMC_A)上,因此STM32的FSMC_Ax与外部设备的Ax一一对应,FSMC控制器会根据HADDR[0]的状态选择从外部设备读到的字节的高/低位
2. 2440等ARM芯片并不存在该特点，其内存控制器不会根据外部设备的位宽进行移位，需要硬件进行适配
3. STM32-->1个地址1个字节(8bit),16bit外设-->1个地址2个字节(16bit)