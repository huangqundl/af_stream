/*
 ******************************************************************************
 版权所有 (C), 2001-2011, 华为技术有限公司
 ******************************************************************************
  文 件 名   : egnapitype.h
  版 本 号   : 初稿
  作    者   : EGN_TEAM
  生成日期   : 2008年7月18日
  最近修改   :
  功能描述   : EGN基本类型定义
  函数列表   :
  修改历史   :
  1.日    期   : 2008年7月18日
    作    者   : EGN_TEAM
    修改内容   : 创建文件

******************************************************************************/
/**@file  egnapitype.h
  *    EGN基本类型定义
*******************************************************/

#ifndef __EGN_API_TYPE_H__
#define __EGN_API_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * @defgroup common  公共的系统服务和系统管理API
 */

/** 定义函数声明中的帮助符号， 编译时无效 */
#ifndef IN
#define IN      /*   input parameter */
#endif

#ifndef OUT
#define OUT     /*   output parameter */
#endif

#ifndef INOUT
#define INOUT   /*   input and output parameter */
#endif

/* 常用基本类型定义 */
typedef signed char    EGN_INT8;
typedef unsigned char  EGN_UINT8;
typedef char           EGN_CHAR;
typedef unsigned char  EGN_UCHAR;

typedef signed short   EGN_INT16;
typedef unsigned short EGN_UINT16;

typedef signed int     EGN_INT32;
typedef unsigned int   EGN_UINT32;
typedef EGN_UINT32     EGN_BOOL;

typedef signed long    EGN_LONG;
typedef unsigned long  EGN_ULONG;

#define EGN_VOID       void    /*   void */
#define EGN_ZERO       0       /*   The number 0 */

/** 返回错误码 */
#define EGN_RET_ERR(_phase, _ret) return((_ret)|(((__LINE__) >> 1) << 12)|__FILE_ID__|(_phase))

/** 返回非错误码 */
#define EGN_RET_VAL(_code) return(_code)

/** 设置返回值 */
#define EGN_RET_SET(_ret, _phase) ((_ret)|(((__LINE__) >> 1) << 12)|__FILE_ID__|(_phase))

/** 没有返回值 */
#define EGN_RET_VOID return

/** 返回错误码 */
#define EGN_RET_ERR_EXT(_ret)  EGN_RET_ERR(0, (_ret))

/** 取得错误码 */
#define EGN_RET_GETVAL(_ret) ((_ret) & 0xFF)

/* 常用基本宏定义 */
#define EGN_EMPTY_ARRAY    {0}                  /* 空数组 */
#define EGN_NULL_PTR       (EGN_VOID *)0x0L     /* 空指针的值 */
#define EGN_INVALID_UINT8  0xFF                 /* 用于判断 无符号字符类型的上限值 */
#define EGN_INVALID_UINT16 0xFFFF               /* 用于判断 无符号短整型的上限值 */
#define EGN_MAX_UINT32     0xFFFFFFFF           /* 用于判断 无符号整型的上限值 */
#define EGN_INVALID_UINT32 EGN_MAX_UINT32       /* 用于判断 无符号整型的上限值 */

#define EGN_EN_INVALID     (-1)                 /* 用于判断枚举的开始值 */
#define EGN_EN_BUTT        0x7FFFFFFFU          /* 用于判断枚举的结束值 */

#define EGN_FALSE          0                    /* 布尔值:为假 */
#define EGN_TRUE           1                    /* 布尔值:为真 */

/** MemCp句柄 */
typedef  EGN_VOID*    EgnMemCpHdl;

#define EGN_CONST const     /* 常量说明 */

#define EGN_OPER_SET 0      /* 统计操作 复位 */
#define EGN_OPER_ADD 1      /* 统计操作 加 */
#define EGN_OPER_DEL 2      /* 统计操作 减 */

#define EGN_DYNAMIC  0       /* 动态内存的类型 */
#define EGN_STATIC   1       /* 静态内存的类型 */

#define EGN_CMP_EQUAL   0    /* 比较结果:相等 */
#define EGN_CMP_LOWER (-1)   /* 比较结果:小于 */
#define EGN_CMP_BIGGER  1    /* 比较结果:大于 */

#define EGN_UNIT_NONE    0   /* 无单位 */
#define EGN_UNIT_TIME_S  1   /* 时间单位秒 */
#define EGN_UNIT_TIME_M  2   /* 时间单位分 */
#define EGN_UNIT_TIME_MS 3   /* 时间单位毫秒 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_API_TYPE_H__ */

