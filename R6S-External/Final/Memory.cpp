#include <Windows.h>
#include <Psapi.h>
#include "Memory.h"
#include "Globals.h"
#include "Offsets.h"

Memory::Memory() {};

BOOL Memory::SetBaseAddress() {
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(Global::GameHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(Global::GameHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				std::wstring wstrModName = szModName;
				std::wstring wstrModContain = L"RainbowSix.exe";
				if (wstrModName.find(wstrModContain) != std::string::npos)
				{
					Global::BaseAddress = hMods[i];
					return true;
				}
			}
		}
	}
	return false;
}

DWORD_PTR Memory::GetBaseAddress() {
	return (DWORD_PTR)Global::BaseAddress;
}

template<typename T> T Memory::RPM(SIZE_T address) {
	//The buffer for data that is going to be read from memory
	T buffer;

	//The actual RPM
	ReadProcessMemory(Global::GameHandle, (LPCVOID)address, &buffer, sizeof(T), NULL);

	//Return our buffer
	return buffer;
}

std::string Memory::RPMString(SIZE_T address) {
	//Make a char array of 20 bytes
	char name[20];

	//The actual RPM
	ReadProcessMemory(Global::GameHandle, (LPCVOID)address, &name, sizeof(name), NULL);

	//Add each char to our string
	//While also checking for a null terminator to end the string
	std::string nameString;
	for (int i = 0; i < sizeof(name); i++) {
		if (name[i] == 0)
			break;
		else
			nameString += name[i];
	}

	return nameString;
}

template<typename T> void Memory::WPM(SIZE_T address, T buffer) {
	//A couple checks to try and avoid crashing
	//These don't actually make sense, feel free to remove redundent ones
	if (address == 0 || (LPVOID)address == nullptr || address == NULL) {
		return;
	}

	WriteProcessMemory(Global::GameHandle, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}

void Memory::UpdateAddresses() {
	//Game manager pointer from games base address + the GameManager offset
	pGameManager = RPM<DWORD_PTR>(GetBaseAddress() + ADDRESS_GAMEMANAGER);
	//Entity list pointer from the GameManager + EntityList offset
	pEntityList = RPM<DWORD_PTR>(pGameManager + OFFSET_GAMEMANAGER_ENTITYLIST);

	//Renderer pointer from games base address + Renderer offset
	pRender = RPM<DWORD_PTR>(GetBaseAddress() + ADDRESS_GAMERENDERER);
	//Game Renderer pointer from Renderer + GameRenderer offset
	pGameRender = RPM<DWORD_PTR>(pRender + OFFSET_GAMERENDERER_DEREF);
	//EngineLink pointer from GameRenderer + EngineLink offset
	pEngineLink = RPM<DWORD_PTR>(pGameRender + OFFSET_GAMERENDERER_ENGINELINK);
	//Engine pointer from EngineLink + Engine offset
	pEngine = RPM<DWORD_PTR>(pEngineLink + OFFSET_ENGINELINK_ENGINE);
	//Camera pointer from Engine + Camera offset
	pCamera = RPM<DWORD_PTR>(pEngine + OFFSET_ENGINE_CAMERA);
}

DWORD_PTR Memory::GetEntity(int i) {
	DWORD_PTR entityBase = RPM<DWORD_PTR>(pEntityList + (i * OFFSET_ENTITY));
	return RPM<DWORD_PTR>(entityBase + OFFSET_ENTITY_REF);
}

DWORD_PTR Memory::GetLocalEntity() {
	//Loop through the first 12
	for (int i = 0; i < 12; i++) {
		//get current entity
		DWORD_PTR entity = GetEntity(i);
		//get that entity's name
		std::string entityName = GetEntityPlayerName(entity);

		//check it against our local name
		if (strcmp(entityName.c_str(), Global::LocalName.c_str()) == 0) {
			return entity;
		}
	}

	//return the first entity if we didn't find anything
	return GetEntity(0);
}

DWORD Memory::GetEntityHealth(DWORD_PTR entity) {
	//Entity info pointer from the Entity
	DWORD_PTR EntityInfo = RPM<DWORD_PTR>(entity + OFFSET_ENTITY_ENTITYINFO);
	//Main component pointer from the entity info
	DWORD_PTR MainComponent = RPM<DWORD_PTR>(EntityInfo + OFFSET_ENTITYINFO_MAINCOMPONENT);
	//Child component pointer form the main component
	DWORD_PTR ChildComponent = RPM<DWORD_PTR>(MainComponent + OFFSET_MAINCOMPONENT_CHILDCOMPONENT);

	//Finally health from the child component
	return RPM<DWORD>(ChildComponent + OFFSET_CHILDCOMPONENT_HEALTH);
}

Vector3 Memory::GetEntityFeetPosition(DWORD_PTR entity) {
	//We get the feet position straight from the entity
	return RPM<Vector3>(entity + OFFSET_ENTITY_FEET);
}

Vector3 Memory::GetEntityHeadPosition(DWORD_PTR entity) {
	//We get the head position straight from the entity
	return RPM<Vector3>(entity + OFFSET_ENTITY_HEAD);
}

Vector3 Memory::GetEntityNeckPosition(DWORD_PTR entity) {
	//We get the head position straight from the entity
	return RPM<Vector3>(entity + OFFSET_ENTITY_NECK);
}

std::string Memory::GetEntityPlayerName(DWORD_PTR entity) {
	DWORD_PTR playerInfo = RPM<DWORD_PTR>(entity + OFFSET_ENTITY_PLAYERINFO);
	DWORD_PTR playerInfoD = RPM<DWORD_PTR>(playerInfo + OFFSET_ENTITY_PLAYERINFO_DEREF);

	return RPMString(RPM<DWORD_PTR>(playerInfoD + OFFSET_PLAYERINFO_NAME) + 0x0);
}

BYTE Memory::GetEntityTeamId(DWORD_PTR entity) {
	//Team id comes from player info
	DWORD_PTR playerInfo = RPM<DWORD_PTR>(entity + OFFSET_ENTITY_PLAYERINFO);
	//We have to derefrnce it as 0x0
	DWORD_PTR playerInfoD = RPM<DWORD_PTR>(playerInfo + OFFSET_ENTITY_PLAYERINFO_DEREF);

	return RPM<BYTE>(playerInfoD + OFFSET_PLAYERINFO_TEAMID);
}

PlayerInfo Memory::GetAllEntityInfo(DWORD_PTR entity) {
	PlayerInfo p;

	p.Health = GetEntityHealth(entity);
	p.Name = GetEntityPlayerName(entity);
	p.Position = GetEntityFeetPosition(entity);
	p.w2s = WorldToScreen(p.Position);
	p.w2sHead = WorldToScreen(GetEntityHeadPosition(entity));
	p.TeamId = GetEntityTeamId(entity);

	return p;
}

Vector3 Memory::GetViewTranslation() {
	//View translation comes straight from the camera
	return RPM<Vector3>(pCamera + OFFSET_CAMERA_VIEWTRANSLATION);
}

Vector3 Memory::GetViewRight() {
	//View right comes directly from the camera
	return RPM<Vector3>(pCamera + OFFSET_CAMERA_VIEWRIGHT);
}

Vector3 Memory::GetViewUp() {
	//View up comes directly from the camera
	return RPM<Vector3>(pCamera + OFFSET_CAMERA_VIEWUP);
}

Vector3 Memory::GetViewForward() {
	//View forward comes directly from the camera
	return RPM<Vector3>(pCamera + OFFSET_CAMERA_VIEFORWARD);
}

float Memory::GetFOVX() {
	//FOV comes directly from the camera
	return RPM<float>(pCamera + OFFSET_CAMERA_VIEWFOVX);
}

float Memory::GetFOVY() {
	//FOV comes directly from the camera
	return RPM<float>(pCamera + OFFSET_CAMERA_VIEWFOVY);
}

Vector3 Memory::WorldToScreen(Vector3 position) {
	Vector3 temp = position - GetViewTranslation();

	float x = temp.Dot(GetViewRight());
	float y = temp.Dot(GetViewUp());
	float z = temp.Dot(GetViewForward() * -1);

	return Vector3((displayWidth / 2) * (1 + x / GetFOVX() / z), (displayHeight / 2) * (1 - y / GetFOVY() / z), z);
}