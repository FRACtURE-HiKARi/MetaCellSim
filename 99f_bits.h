/*this header file contains bit label definition*/

#pragma once

#define PositionLeft    0b1000 
#define PositionTop     0b0100
#define PositionRight   0b0010
#define PositionBottom  0b0001

#define Status_Green    0b0001
#define Status_Burning  0b0010
#define Status_Burnt    0b0100

/* L    Left   */
/* R    Right  */
/* T    Top    */
/* B    Bottom */

/* LT   T   RT */
/* L         R */
/* LB   B   RB */

#define Test_LB         0x01
#define Test_L          0x02
#define Test_LT         0x04
#define Test_T          0x08
#define Test_RT         0x10
#define Test_R          0x20
#define Test_RB         0x40
#define Test_B          0x80

#define POS_L  0
#define POS_T  1
#define POS_R  2
#define POS_B  3
#define POS_LT 4
#define POS_RT 5
#define POS_RB 6
#define POS_LB 7