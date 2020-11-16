// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "vector.h"
#include "ESP.h"
#include "dllmain.h"
#include "Entity.h"

static struct openglFormatMatrix
{
	float v[16];

	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }
};

glm::mat4 dumbToFancyMatrix(openglFormatMatrix dumbMatrix) {
	glm::mat4 mat;
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = dumbMatrix[count];
		}
	}
	return mat;
}

// Holy grail offset
int playerBaseAddressOffset = 0x10f4f4; //gathered via ac_client.exe+AAFB - 8B 0D F4F45000        - mov ecx,[ac_client.exe+10F4F4] { (0271A138) }
int entityListAddressOffset = 0x10F4F8; // ac_client.exe + 5AA2D - 8B 0D F8F45000 - mov ecx, [ac_client.exe + 10F4F8]{ (02834838) }
int numPlayerEntitiesAddressOffset = 0x10F500; // ac_client.exe+25719 - 89 0D 00F55000 - mov [ac_client.exe+10F500],ecx { (12) }

// OpenGL specific offsets
int viewMatrixOffset = 0x101Ae8;

// Entity object variable offsets, kept for reference mostly as we have a ReClass entity to do the legwork

int yawOffset = 0x40;
int pitchOffset = 0x44;
int healthOffset = 0xf8;
int armourOffset = 0xfc;
int akimboOffset = 0x10c;
int grenadeOffset = 0x158;
int nameOffset = 0x225;
int teamOffset = 0x32c;
int weaponsListOffset = 0x348;
int currWeaponPtrOffset = 0x374;
int shootingBooleanOffset = 0x224;
int playerAmmoListOffset = 0x013c;

//Location on the heap where the verbs reside (x VERBED y)
int verbStringBaseAddressOffset = 0xfcee8; // Address on the heap of all of the verbs which are printed when a player is killed
// canpickup() func address offset
int canPickupFuncAddressOffset = 0x58c2b; // ac_client.exe + 58C2B - E8 80DBFEFF - call ac_client.exe + 467B0{ call to canpickup() }
// pickup() func address offset
int pickupFuncAddressOffset = 0x466b0; // ac_client.exe + 38B30 - E8 7BDB0000 - call ac_client.exe + 466B0{ Call to pickup()}
// getEntAtCrosshair() func address offset
int getEntAtCrosshairOffset = 0x607C0; // ac_client.exe+AD9D - E8 1E5A0500 - call ac_client.exe+607C0
// selectWeapon() func address offset
int selectWeaponOffset = 0x5F619; // ac_client.exe + 5F619 - 81 F9 C8000000 - cmp ecx, 000000C8{ 200 }


template<typename T>
T readMemory(HANDLE hProcess, LPVOID adr)
{
	T val;
	DWORD oldProtect;
	VirtualProtectEx(hProcess, adr, sizeof(T), PAGE_READWRITE, &oldProtect);
	ReadProcessMemory(hProcess, adr, &val, sizeof(T), NULL);
	VirtualProtectEx(hProcess, adr, sizeof(T), oldProtect, NULL);
	return val;
}

template<typename T>
void writeMemory(HANDLE hProcess, LPVOID adr, T val)
{
	DWORD oldProtect;
	VirtualProtectEx(hProcess, adr, sizeof(T), PAGE_READWRITE, &oldProtect);
	WriteProcessMemory(hProcess, adr, &val, sizeof(T), NULL);
	VirtualProtectEx(hProcess, adr, sizeof(T), oldProtect, NULL);
}

void writeToGameConsole(std::string message) {
	typedef void(__cdecl* _contoutf)(const char* string, ...);
	_contoutf contoutf = (_contoutf)0x46b060;

	contoutf(message.c_str());
}

int getEntAtCrosshair(int baseAddress) {
	typedef int(__cdecl* _getEntAtCrosshairf)();
	_getEntAtCrosshairf getEntAtCrosshairf = (_getEntAtCrosshairf)(baseAddress + getEntAtCrosshairOffset);
	return getEntAtCrosshairf();
}

void selectWeapon(entity* playerPtr, int weapon) {
	// Get the current list of weapon pointers
	playerPtr->currWeaponPtr = playerPtr->weaponPtrs[weapon];

}

//int WIP!!
void  pickup(int baseAddress, int itemType)
{
	typedef void(__fastcall* _pickup)(int itemType);

	int playerEntAddr = playerBaseAddressOffset + baseAddress + 0xf4;

	_pickup pickupF = (_pickup)(baseAddress + pickupFuncAddressOffset);

	// Get EAX setup ready with our player entity..
	__asm {
		mov eax, playerEntAddr
		mov edx, 5
	}
	pickupF(itemType);

	typedef void(__cdecl* _contoutf)(const char* string, ...);
	_contoutf contoutf = (_contoutf)0x46b060;

	contoutf((char*)"Hello");
}

void createConsole()
{
	FILE* fDummy;

	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		Beep(10000, 2000);
		return;
	}

	// std::cout, std::clog, std::cerr, std::cin

	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFile(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();;
}

void printMenu() {
	writeToGameConsole("===================");
	writeToGameConsole("Numpad 1 full ammo / Numpad 2 full health");
	writeToGameConsole("Numpad 3 for the help menu / Numpad 4 silly verbs");
	writeToGameConsole("Numpad 5 toggle aimbot / Numpad 6 toggle triggerbot");
	writeToGameConsole("Numpad 7 toggle ESP / Numpad 8 cycle weapons");
	writeToGameConsole("===================");
}



void aimbot(HANDLE currentProcess, int baseAddress, entity* player, bool targetAll) {
	// first up, find out how many entity pointers we'll read.
	int numEnts = readMemory<int>(currentProcess, (void*)(baseAddress + numPlayerEntitiesAddressOffset));
	if (numEnts == 0) {
		return;
	}
	// Get the entity list pointer so that we can dereference it later
	int entityListPtr = readMemory<int>(currentProcess, (void*)(baseAddress + entityListAddressOffset));

	
	Vec3 playerVec = { player->xPosHead, player->yPosHead, player->zPosHead};
	float closestDistance = 9999999.0f; // The distance to the closest enemy, initialised to some chonky number
	Vec3 closestEnemyVec;
	for (int i = 1; i < numEnts; i++) { // First pointer is always 0, because reasons, so skip it with i = 1
		entity* enemy = readMemory<entity*>(currentProcess, (void*)(entityListPtr + (i * 4)));
		
		if (enemy->health <= 0) { // don't bother with calculations if they're dead.
			continue;
		}
		
		//Get the distance to the player.
		float distance = sqrt(pow(player->xPosHead - enemy->xPosHead, 2) + pow(player->yPosHead- enemy->yPosHead, 2) + pow(player->zPosHead- enemy->zPosHead, 2));
		if (distance < closestDistance && enemy->health > 0) {
			if (!targetAll && enemy->team == player->team) {
				continue;
			}
			// This is a viable candidate, check if we can see the enemy
			closestDistance = distance;
			closestEnemyVec = { enemy->xPosHead, enemy->yPosHead, enemy->zPosHead };

		}
		// debug output - 
		//std::cout << "Enemy Name: " << enemyName << " X: " << enemyX << " Y: " << enemyY << " Z: " << enemyZ << " Health: " << enemyHealth << " Distance: " << distance << " Team: " << enemyTeam << std::endl;

	}
	if (closestDistance == 9999999.0f) {
		// There's no enemy entities to shoot, exit without changing the pitch/yaw
		return;
	}
	// We have a nearest enemy pointer, now we can point at it.
	// Maths gratuitously ripped from the Internet because I am BAD.
	Vec3 distanceVector = closestEnemyVec - playerVec;
	distanceVector.Normalize();
	float yaw = -atan2(distanceVector.x, distanceVector.y) / 3.141592653589F * 180 + 180;
	float pitch = atan2(distanceVector.z, sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y)) / 3.141592653589F * 180;
	// we should have the correct angles now, so put them in place of our existing pitch and yaw
	player->pitch = pitch;
	player->yaw = yaw;
}

void triggerbot(HANDLE currentProcess, int baseAddress, entity* player, bool targetAll) {
	int entLookedAt = getEntAtCrosshair(baseAddress);
	
	if (entLookedAt == 0x0) { // not looking at anything, nullptr
		// Stop shooting
		player->isShooting = 0;
		return;
	}

	entity enemy = readMemory<entity>(currentProcess, (void*)(entLookedAt));

	if (enemy.health <= 0) { // don't bother with calculations if they're dead.
		return;
	}

	// Debug COUTs
	std::cout << "Looking at a player entity at 0x" << std::hex << entLookedAt << std::dec;
	std::cout << " name - " << enemy.name << " team - " << enemy.team << std::endl;

	// Bail out if we're looking at a teammate
	if (player->team == enemy.team && !targetAll) {
		return;
	}

	std::cout << "It's an enemy, start shooting" << std::endl;
	player->isShooting = 1;

}

void renderESP(HANDLE currentProcess, entity* player, int baseAddress, ESP friendESP, ESP foeESP) {
	// Must be recalculated every frame
	openglFormatMatrix dumbMatrix = readMemory<openglFormatMatrix>(currentProcess, (void*)(baseAddress + viewMatrixOffset));

	//Convert the dumb matrix to a fancy one
	glm::mat4 viewMatrix = dumbToFancyMatrix(dumbMatrix);

	// first up, find out how many entity pointers we'll read.
	int numEnts = readMemory<int>(currentProcess, (void*)(baseAddress + numPlayerEntitiesAddressOffset));
	if (numEnts == 0) {
		return;
	}
	// Get the entity list pointer so that we can dereference it later
	int entityListPtr = readMemory<int>(currentProcess, (void*)(baseAddress + entityListAddressOffset));


	for (int i = 1; i < numEnts; i++) { // First pointer is always 0, because reasons, so skip it with i = 1
		entity* enemy = readMemory<entity*>(currentProcess, (void*)(entityListPtr + (i * 4)));

		if (enemy->health <= 0) { // don't bother with rendering ESP if they're dead.
			continue;
		}

		glm::vec3 enemyVec = { enemy->xPosHead, enemy->yPosHead, enemy->zPosHead };
		float distance = sqrt(pow(player->xPosHead - enemyVec.x, 2) + pow(player->yPosHead - enemyVec.y, 2) + pow(player->zPosHead - enemyVec.z, 2));
		if (enemy->team == player->team) {
			friendESP.renderESPBox(enemyVec, dumbMatrix.v, distance);
		}
		else {
			foeESP.renderESPBox(enemyVec, dumbMatrix.v, distance);
		}

	}
}

DWORD WINAPI haxThread(HMODULE hmodule) {
	createConsole();
	std::cout << "We're injected! numpad keys for cheats. NUM3 for menu" << std::endl;

	// module base address
	DWORD baseAddress = (DWORD)GetModuleHandleW(L"ac_client.exe");
	std::cout << "Base address - " << baseAddress << std::endl;
	HANDLE currentProcess = GetCurrentProcess();
	int aimbotMode = 0;
	int triggerbotMode = 0;
	int espMode = 0;
	int currWeapon = 0;
	printMenu();
	ESP friendESP(ESP::TEAM::FRIEND);
	ESP foeESP(ESP::TEAM::ENEMY);

	while (1)
	{
		entity* playerEnt = readMemory<entity*>(currentProcess, (void*)(baseAddress + playerBaseAddressOffset));
		if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
			std::cout << "Detaching injected DLL." << std::endl;
			break;
		}
		if (GetAsyncKeyState(VK_NUMPAD1) & 1) { // Full ammo
			// give self 999 ammo
			// Ammo pointer chain 
			// pointer chain = [[[[base+109B74] + 374] + 14] + 0]
			/*int ammoPointer = readMemory<int>(currentProcess, (void*)(baseAddress + 0x109b74));
			ammoPointer = readMemory<int>(currentProcess, (void*)(ammoPointer + 0x374));
			ammoPointer = readMemory<int>(currentProcess, (void*)(ammoPointer + 0x14));
			writeMemory<int>(currentProcess, (void*)ammoPointer, 9999);*/
			//writeMemory<int>(currentProcess, (void*)(playerEnt + grenadeOffset), 9999);
			for (int i = 0; i < 10; i++) {
				playerEnt->ammo[i] = 9999;
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1) { // full health
			// give self 999 health 
			// get the player entity
			playerEnt->health = 9999;
			playerEnt->armour = 9999;
			playerEnt->akimbo = 1;
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1) { // print the cheat menu
			printMenu();
		}

		if (GetAsyncKeyState(VK_NUMPAD4) & 1) { // fun verbs :D
			//there are 18 verbs, each 16 bytes wide
			char chars[18][16] = { "bl00ped\0\0\0\0\0\0\0\0","sl00ped\0\0\0\0\0\0\0\0","fl00pered\0\0\0\0\0\0",
				"brrred\0\0\0\0\0\0\0\0\0","bopped\0\0\0\0\0\0\0\0","bammed\0\0\0\0\0\0\0\0\0",
				"SMASHED\0\0\0\0\0\0\0\0","SLAPPED\0\0\0\0\0\0\0\0","gatted\0\0\0\0\0\0\0\0\0",
				"clapped\0\0\0\0\0\0\0\0","wrecked\0\0\0\0\0\0\0\0","eviscerated\0\0\0\0",
				"un-alived\0\0\0\0\0\0","deleted\0\0\0\0\0\0\0\0","crumpled\0\0\0\0\0\0\0",
				"erased\0\0\0\0\0\0\0\0\0","crushed\0\0\0\0\0\0\0\0","haxx0red\0\0\0\0\0\0\0" };
			for (int i = 0; i < 18; i++) {
				WriteProcessMemory(currentProcess, (void*)(baseAddress + verbStringBaseAddressOffset + (i * 16)), chars[i], 16, NULL);
			}
			writeToGameConsole("Verbs were sillified");
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1) { // aimbot!
			aimbotMode++;
			if (aimbotMode > 2) {
				aimbotMode = 0;
			}
			if (aimbotMode == 0) {
				std::cout << "Aimbot is off." << std::endl;
				writeToGameConsole("Aimbot OFF");
			}
			else if (aimbotMode == 1) {
				std::cout << "Aimbot is set to target-all (including teammates!)" << std::endl;
				writeToGameConsole("Aimbot TARGET ALL");
			}
			else if (aimbotMode == 2) {
				std::cout << "Aimbot is set to target-foes (excluding teammates!)" << std::endl;
				writeToGameConsole("Aimbot TARGET FOES");
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD6) & 1) { // triggerbot!
			triggerbotMode++;
			if (triggerbotMode > 2) {
				triggerbotMode = 0;
			}
			if (triggerbotMode == 0) {
				std::cout << "Triggerbot is off." << std::endl;
				writeToGameConsole("Triggerbot OFF");
			}
			else if (triggerbotMode == 1) {
				std::cout << "Triggerbot is set to target-all (including teammates!)" << std::endl;
				writeToGameConsole("Triggerbot TARGET ALL");
			}
			else if (triggerbotMode == 2) {
				std::cout << "Triggerbot is set to target-foes (excluding teammates!)" << std::endl;
				writeToGameConsole("Triggerbot TARGET FOES");
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD7) & 1) { // toggle ESP
			espMode == 0 ? espMode = 1 : espMode = 0;
		}

		if (GetAsyncKeyState(VK_NUMPAD8) & 1) { // cycle weapons
			++currWeapon;
			if (currWeapon > 8) {
				currWeapon = 0;
			}

			selectWeapon(playerEnt, currWeapon);
		}

		if (aimbotMode > 0)
		{
			aimbot(currentProcess, baseAddress, playerEnt, aimbotMode == 1);
		}

		if (triggerbotMode > 0)
		{
			triggerbot(currentProcess, baseAddress, playerEnt, triggerbotMode == 1);
		}


		if (espMode > 0) {
			renderESP(currentProcess, playerEnt, baseAddress, friendESP, foeESP);
		}
	}
	FreeConsole();
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)haxThread, hModule, 0, nullptr));
		std::cout << "Process Attach" << std::endl;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}