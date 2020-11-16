// Created with ReClass.NET 1.2 by KN4CK3R
#include <iostream>
#include <Windows.h>
class entity
{
public:
	int32_t vfTable; //0x0000
	float xPosHead; //0x0004
	float yPosHead; //0x0008
	float zPosHead; //0x000C
	float xVel; //0x0010
	float yVel; //0x0014
	float zVel; //0x0018
	char pad_001C[24]; //0x001C
	float xPosFeet; //0x0034
	float yPosFeet; //0x0038
	float zPosFeet; //0x003C
	float pitch; //0x0040
	float yaw; //0x0044
	char pad_0048[176]; //0x0048
	int32_t health; //0x00F8
	int32_t armour; //0x00FC
	char pad_0100[12]; //0x0100
	int32_t akimbo; //0x010C
	char pad_0110[44]; //0x0110
	int32_t ammo[10]; //0x013C
	char pad_0164[16]; //0x0164
	int32_t timeUntilNextShot; //0x0174
	char pad_0178[172]; //0x0178
	bool isShooting; //0x0224
	char name[16]; //0x0225
	char pad_0235[244]; //0x0235
	bool N0000034E; //0x0329
	char pad_032A[2]; //0x032A
	uint8_t team; //0x032C
	char pad_032D[24]; //0x032D
	uint16_t N00000355; //0x0345
	bool N00000B56; //0x0347
	uint32_t* weaponPtrs[10]; //0x0348
	uint32_t* N00000356; //0x0370
	uint32_t* currWeaponPtr; //0x0374
	uint32_t* N00000358; //0x0378
	uint32_t* N00000359; //0x037C
	uint32_t* N0000035A; //0x0380
	char pad_0384[1220]; //0x0384
}; //Size: 0x0848