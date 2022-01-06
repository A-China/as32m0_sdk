#ifndef __AS32M0_LVD_H__
#define __AS32M0_LVD_H__

#ifdef	__cplusplus
extern "C" {	/* allow C++ to use these headers */
#endif	/* __cplusplus */

#include "as32m0.h"

/*
 * Description:
 * Bit shifts and widths for  CR 
 */
#define bsLVD_CR_EN              0
#define bwLVD_CR_EN                  1
#define bsLVD_CR_MODE            1
#define bwLVD_CR_MODE                1
#define bsLVD_CR_LV              2
#define bwLVD_CR_LV                  1
#define bsLVD_CR_PE              3
#define bwLVD_CR_PE                  1
#define bsLVD_CR_NE              4
#define bwLVD_CR_NE                  1



/*
 * Description:
 * Bit shifts and widths for  ST 
 */
#define bsLVD_ST_STATUS         0
#define bwLVD_ST_STATUS             1


/*
 * Description:
 * Bit shifts and widths for  SET
 */
#define bsLVD_SET_BYP           0
#define bwLVD_SET_BYP               1
#define bsLVD_SET_SET           4
#define bwLVD_SET_SET               0x7


/*
 * Description:
 * Bit shifts and widths for FLT
 */
#define bsLVD_FLT_EN           0
#define bwLVD_FLT_EN               1
#define bsLVD_FLT_LMT          16
#define bwLVD_FLT_LMT              0xFFFF

typedef enum {
   HIGH_TRIGGER    = 0,
   LOW_TRIGGER     = 1,
   POSEDGE_TRIGGER = 2,
   NEGEDGE_TRIGGER = 3,
}LVD_TRIGGER_MODE;

typedef enum {
    INT_MODE    = 0,
    RESET_MODE  = 1,
}LVD_MODE;

typedef enum{
    ONT_POINT_EIGHT = 0,
    TWO_POINT_ZERO  = 1,
    TWO_POINT_FOUR  = 2,
    TWO_POINT_SIX   = 3,
    THREE_POINT_ZERO = 4,
    THREE_POINT_SIX  = 5,
    THREE_POINT_NINE = 6,
    FOUR_POINT_TWO   = 7,
}LVD_VDT;


typedef struct lvd_info{
    uint32_t         filter_en;
    uint32_t         filter_lmt;
    uint32_t         byp_vdt_en;
    LVD_VDT          LVD_VDT;
    LVD_MODE         LVD_MODE;
    LVD_TRIGGER_MODE LVD_TRIGGER_MODE;
}LVD_INFO,*LVD_INFO_PTR;


void LVD_Init(LVD_INFO_PTR lvd_info_ptr);
void LVD_Int_Clr(void);
void LVD_Disable(void);



#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif	/* __cplusplus */


#endif

