/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : pwrctrl_sleeptst.h
  �� �� ��   : ����
  ��    ��   : ������ 56193
  ��������   : 2012��7��25��
  ����޸�   :
  ��������   : pwrctrl_sleeptst.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��25��
    ��    ��   : ������ 56193
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __PWRCTRL_SLEEPTST_H__
#define __PWRCTRL_SLEEPTST_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

#if 1  /*after acpu softtimer ready, should be deleted*/
#define PWRCTRL_ACPU_TST_TIMER_ID                   (8)         /*timer 8*/
#define PWRCTRL_ACPU_TST_TIMER_INT_ID               (52)        /*timer 8*/

#define PWCTRL_ACPU_TST_TIMER_CTRL                  (0xe2)
#endif







/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
#ifdef WIN32
typedef struct irqaction
{
    u32_t voteEn;                   /*stub*/
} irqaction;

#endif

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

extern s32_t pwrctrl_test_time_int(void_t);
extern s32_t pwrctrl_test_time_set(u32_t time);












#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of bsp_pwc_sleeptst.h */