/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : usb_api.c
  �� �� ��   : ����
  ��    ��   : ���� 00195127
  ��������   : 2012��9��15��
  ����޸�   :
  ��������   : USBģ�����ӿ�ʵ��,�ӿڶ���μ��ӿ��ĵ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��15��
    ��    ��   : ���� 00195127
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
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
  2 ȫ�ֱ�������
*****************************************************************************/
PNP_CALLBACK_STACK* pEnableStack = NULL;
PNP_CALLBACK_STACK* pDisableStack = NULL;

static ACM_HANDLE_COMMAND_CB_T pAcmHandleCmdCB= NULL;

int g_usb_enum_complete = 0;


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
   ��NAS�Ľӿ�ʵ�֣�������׮ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : BSP_USB_RegUdiEnableCB
 ��������  : Э��ջ֪ͨUSB�ϲ�Ӧ�ô�UDI�豸�˿�
 �������  : USB_UDI_ENABLE_CB_T pFunc  : USBʹ�ܻص�����ָ��
 �������  : ��
 �� �� ֵ  : unsigned int: 0��ʾ�ɹ� 1��ʾʧ��,��VOS����ķ���ֵ����һ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��9��15��
    ��    ��   : ���� 00195127
    �޸�����   : ��V3R2��ֲ����

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
 �� �� ��  : BSP_USB_RegUdiDisableCB
 ��������  : Э��ջ֪ͨUSB�ϲ�Ӧ�ùر�UDI�豸�˿�
 �������  : USB_UDI_DISABLE_CB_T pFunc  : USBʹ�ܻص�����ָ��
 �������  : ��
 �� �� ֵ  : unsigned int: 0��ʾ�ɹ� 1��ʾʧ��,��VOS����ķ���ֵ����һ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��9��15��
    ��    ��   : ���� 00195127
    �޸�����   : ��V3R2��ֲ����

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
 �� �� ��  : BSP_USB_UdiEnableNotify
 ��������  : USBģ������ϲ�ע��ص�֪ͨ�ϲ�USB�Ѿ�OK�����Դ�UDI
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��9��15��
    ��    ��   : ���� 00195127
    �޸�����   : ��V3R2��ֲ����

*****************************************************************************/
void BSP_USB_UdiEnableNotify (void)
{
    PNP_CALLBACK_STACK* pEnableFunc = pEnableStack;
    /* ģ��USB����֪ͨ */
    while (pEnableFunc)
    {
        pEnableFunc->pCallback();
        pEnableFunc = pEnableFunc->pPre;
    }
    g_usb_enum_complete = 1;
}

/*****************************************************************************
 �� �� ��  : BSP_USB_UdiDisableNotify
 ��������  : USBģ������ϲ�ע��ص�֪ͨ�ϲ�USB�Ѿ��رգ���Ҫ�ر�UDI
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��9��15��
    ��    ��   : ���� 00195127
    �޸�����   : ��V3R2��ֲ����

*****************************************************************************/
void BSP_USB_UdiDisableNotify (void)
{
    PNP_CALLBACK_STACK* pDisableFunc = pDisableStack;
    /* ģ��USB����֪ͨ */
    while (pDisableFunc)
    {
        pDisableFunc->pCallback();
        pDisableFunc = pDisableFunc->pPre;
    }
    g_usb_enum_complete = 0;
}

/*****************************************************************************
 �� �� ��  : BSP_USB_RegUdiEnableCB
 ��������  : ����ע��acm����ͨ�������ӿڣ�ע��ӿ���gser_request_complete
             �б�����
 �������  : ACM_HANDLE_COMMAND_CB_T pFunc  : acm����ͨ�������ص�����ָ��
 �������  : ��
 �� �� ֵ  : unsigned int: 0��ʾ�ɹ� 1��ʾʧ��,��VOS����ķ���ֵ����һ��
 ���ú���  :
 ��������  :
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
 �� �� ��  : BSP_ACM_CmdHandle
 ��������  : ִ��acm����ͨ�������ص��ӿ�
 �������  : cmd        ����ͨ������
             interface  ��������Ľӿں�
             buf        ����������
             size       ���������ݴ�С

 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :gser_request_complete
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



/*�ṩ��NAS�Ľӿ�*/
EXPORT_SYMBOL(BSP_USB_RegUdiEnableCB);
EXPORT_SYMBOL(BSP_USB_RegUdiDisableCB);
EXPORT_SYMBOL(BSP_USB_UdiEnableNotify);
EXPORT_SYMBOL(BSP_USB_UdiDisableNotify);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
