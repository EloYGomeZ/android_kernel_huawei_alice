/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : usb_api.c
  版 本 号   : 初稿
  作    者   : 夏青 00195127
  生成日期   : 2012年9月15日
  最近修改   :
  功能描述   : USB模块对外接口实现,接口定义参见接口文档
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月15日
    作    者   : 夏青 00195127
    修改内容   : 创建文件

******************************************************************************/

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include  <linux/slab.h>
#include  <linux/kernel.h>
#include  <linux/module.h>
#include  <drv_usb_if.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
PNP_CALLBACK_STACK* pEnableStack = NULL;
PNP_CALLBACK_STACK* pDisableStack = NULL;

static ACM_HANDLE_COMMAND_CB_T pAcmHandleCmdCB= NULL;

int g_usb_enum_complete = 0;


/*****************************************************************************
  3 函数实现
*****************************************************************************/

/*****************************************************************************
   与NAS的接口实现，包含打桩实现
*****************************************************************************/

/*****************************************************************************
 函 数 名  : BSP_USB_RegUdiEnableCB
 功能描述  : 协议栈通知USB上层应用打开UDI设备端口
 输入参数  : USB_UDI_ENABLE_CB_T pFunc  : USB使能回调函数指针
 输出参数  : 无
 返 回 值  : unsigned int: 0表示成功 1表示失败,与VOS定义的返回值类型一致
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月15日
    作    者   : 夏青 00195127
    修改内容   : 自V3R2移植函数

*****************************************************************************/
unsigned int BSP_USB_RegUdiEnableCB(USB_UDI_ENABLE_CB_T pFunc)
{
    PNP_CALLBACK_STACK* tmp = pEnableStack;

    USB_API_DBG("enter\n");

    if (NULL == pFunc)
    {
        USB_API_DBG("pFunc NULL\n");
        return 1;
    }

    pEnableStack = (PNP_CALLBACK_STACK*) kmalloc(sizeof(PNP_CALLBACK_STACK), M_ZERO);

    pEnableStack->pPre = tmp;
    pEnableStack->pCallback = pFunc;

    if(g_usb_enum_complete)
    {
        pEnableStack->pCallback();
    }

    return 0;
}

/*****************************************************************************
 函 数 名  : BSP_USB_RegUdiDisableCB
 功能描述  : 协议栈通知USB上层应用关闭UDI设备端口
 输入参数  : USB_UDI_DISABLE_CB_T pFunc  : USB使能回调函数指针
 输出参数  : 无
 返 回 值  : unsigned int: 0表示成功 1表示失败,与VOS定义的返回值类型一致
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月15日
    作    者   : 夏青 00195127
    修改内容   : 自V3R2移植函数

*****************************************************************************/
unsigned int BSP_USB_RegUdiDisableCB(USB_UDI_DISABLE_CB_T pFunc)
{
    PNP_CALLBACK_STACK* tmp = pDisableStack;

    USB_API_DBG("enter\n");

    if (NULL == pFunc)
    {
        USB_API_DBG("pFunc NULL\n");
        return 1;
    }

    pDisableStack = (PNP_CALLBACK_STACK*) kmalloc(sizeof(PNP_CALLBACK_STACK), M_ZERO);

    pDisableStack->pPre = tmp;
    pDisableStack->pCallback = pFunc;

    return 0;
}

/*****************************************************************************
 函 数 名  : BSP_USB_UdiEnableNotify
 功能描述  : USB模块调用上层注册回调通知上层USB已经OK，可以打开UDI
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月15日
    作    者   : 夏青 00195127
    修改内容   : 自V3R2移植函数

*****************************************************************************/
void BSP_USB_UdiEnableNotify (void)
{
    PNP_CALLBACK_STACK* pEnableFunc = pEnableStack;
    /* 模拟USB插入通知 */
    while (pEnableFunc)
    {
        pEnableFunc->pCallback();
        pEnableFunc = pEnableFunc->pPre;
    }
    g_usb_enum_complete = 1;
}

/*****************************************************************************
 函 数 名  : BSP_USB_UdiDisableNotify
 功能描述  : USB模块调用上层注册回调通知上层USB已经关闭，需要关闭UDI
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月15日
    作    者   : 夏青 00195127
    修改内容   : 自V3R2移植函数

*****************************************************************************/
void BSP_USB_UdiDisableNotify (void)
{
    PNP_CALLBACK_STACK* pDisableFunc = pDisableStack;
    /* 模拟USB插入通知 */
    while (pDisableFunc)
    {
        pDisableFunc->pCallback();
        pDisableFunc = pDisableFunc->pPre;
    }
    g_usb_enum_complete = 0;
}

/*****************************************************************************
 函 数 名  : BSP_USB_RegUdiEnableCB
 功能描述  : 用于注册acm控制通道命令处理接口，注册接口在gser_request_complete
             中北调用
 输入参数  : ACM_HANDLE_COMMAND_CB_T pFunc  : acm控制通道命令处理回调函数指针
 输出参数  : 无
 返 回 值  : unsigned int: 0表示成功 1表示失败,与VOS定义的返回值类型一致
 调用函数  :
 被调函数  :
*****************************************************************************/
unsigned int BSP_ACM_RegCmdHandle(ACM_HANDLE_COMMAND_CB_T pFunc)
{
    if (NULL == pFunc)
    {
        USB_API_DBG("pFunc NULL\n");
        return 1;
    }

    pAcmHandleCmdCB = pFunc;

    return 0;
}

/*****************************************************************************
 函 数 名  : BSP_ACM_CmdHandle
 功能描述  : 执行acm控制通道命令处理回调接口
 输入参数  : cmd        控制通道命令
             interface  处理命令的接口号
             buf        待处理数据
             size       待处理数据大小

 输出参数  : 无
 返 回 值  : int
 调用函数  :
 被调函数  :gser_request_complete
*****************************************************************************/
int BSP_ACM_CmdHandle(int cmd, int tty_idx, char *buf, int size)
{
    int ret = 0;
    if (NULL == pAcmHandleCmdCB)
    {
        USB_API_DBG("pFunc NULL\n");
        return -1;
    }

    ret = pAcmHandleCmdCB(cmd, tty_idx, buf, size);
    return ret;
}



/*提供给NAS的接口*/
EXPORT_SYMBOL(BSP_USB_RegUdiEnableCB);
EXPORT_SYMBOL(BSP_USB_RegUdiDisableCB);
EXPORT_SYMBOL(BSP_USB_UdiEnableNotify);
EXPORT_SYMBOL(BSP_USB_UdiDisableNotify);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
