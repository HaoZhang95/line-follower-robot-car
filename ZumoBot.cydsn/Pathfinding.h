/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef PATHFINDING_H_
#define PATHFINDING_H_
#include <stdint.h>

void Pathfinding(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack);
void PathfindingBasic(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack);
void PathfindingAdv(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack);
void PathfindingAdvP(uint16_t r1, uint16_t r3, uint16_t l1, uint16_t l3, uint8_t *checkStop, uint8_t *lastBlack);

#endif 
/* [] END OF FILE */
