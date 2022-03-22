/**
 * @file tools.h
 * @author 信念D力量 (zsf.cn.90@gmail.com)
 * @brief 工具函数与宏
 * @version 0.1
 * @date 2022-03-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __TOOLS_H
#define __TOOLS_H

/**
 * @brief BCD码：用4位二进制数来表示1位十进制数
 * 例：0001 0011 表示 13，1000 0101 表示 83，0001 1010 不合法
 */
#define HEX2BCD(v)  ((v) % 10 + (v) / 10 * 16)
#define BCD2HEX(v)  ((v) % 16 + (v) / 16 * 10)


#endif /* __TOOLS_H */
