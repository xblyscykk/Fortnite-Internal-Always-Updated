#pragma once
#include "custom_runtimes.h"
#include "custom_runtimes.cpp"
#include "safecall.h"
#include "spoof_call.h"
#include "FNTool.h"


int width;
int height;
FNTool* sdk_p = new FNTool();


BOOL valid_pointer1(DWORD64 address)
{
	if (!itsjustaispoof(IsBadWritePtr)((LPVOID)address, (UINT_PTR)8)) return TRUE;
	else return FALSE;
}

template<typename ReadT>
ReadT read1(DWORD_PTR address, const ReadT& def = ReadT())
{
	if (valid_pointer1(address)) {
		return *(ReadT*)(address);
	}
}



template<typename T>
struct TArray
{
public:
	T* Data;
	int ArrayNum;
	int ArrayMax;

public:
	TArray()
	{
		Data = nullptr;
		ArrayNum = ArrayMax = 0;
	}

public:
	int GetNum(void) const
	{
		return this->ArrayNum;
	}

	int GetMax(void) const
	{
		return this->ArrayMax;
	}

	const T& At(int index) const
	{
		return this->Data[index];
	}
};
struct FUniqueNetIdRepl
{
	unsigned char unreflected_1[0x1f]; // 0x1 (0x1f) 
	struct TArray<unsigned char> ReplicationBytes; // 0x20 (0x10)
};
struct FAsyncPhysicsTimestamp
{
	int ServerFrame; // 0x0 (0x4)
	int LocalFrame; // 0x4 (0x4)
};
class sdk {
public:

	inline uintptr_t GetGameBase() {

		return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
	}
	inline bool isValidPointer(uintptr_t address)
	{

		if (!(itsjustaispoof(IsBadWritePtr))((LPVOID)address, (UINT_PTR)8))
			return true;
		else
			return false;
	}

	inline uintptr_t PatternScan(uintptr_t pModuleBaseAddress, const char* sSignature, int pIndex, bool sRelativeAdr = false)
	{

		static auto patternToByte = [](const char* pattern) { auto bytes = std::vector<int>{}; const auto start = const_cast<char*>(pattern); const auto end = const_cast<char*>(pattern) + strlen(pattern); for (auto current = start; current < end; ++current) { if (*current == '?') { ++current; if (*current == '?') ++current; bytes.push_back(-1); } else bytes.push_back(strtoul((const char*)current, &current, 16)); } return bytes; };

		const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = patternToByte(sSignature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true; for (auto j = 0ul; j < s; ++j) { if (scanBytes[i + j] != d[j] && d[j] != -1) { found = false; break; } }
			if (found)
			{
				if (sRelativeAdr)
				{
					return ((uintptr_t)((UINT_PTR)(reinterpret_cast<uintptr_t>(&scanBytes[i])) + *(PINT)((UINT_PTR)(reinterpret_cast<uintptr_t>(&scanBytes[i])) + ((pIndex)-sizeof(INT))) + (pIndex)));
				}
				else
				{
					return reinterpret_cast<uintptr_t>(&scanBytes[i]);
				}
			}
		}

		return NULL;
	}
	inline bool ProcessEvent(uintptr_t address, void* fnobject, void* parms)
	{

		if (!isValidPointer(address)) return false;
		auto index = *reinterpret_cast<void***>(address); if (!index) return false;

		auto fProcessEvent = static_cast<void(*)(void* address, void* fnobject, void* parms)>(index[0x4D]); if (!fProcessEvent) return false; //changes sometimes (needs to be found in ida)
		fProcessEvent((void*)address, (void*)fnobject, (void*)parms);
		return true;
	}
	inline ue::FName Conv_StringToName(const wchar_t* String)
	{
		struct
		{
			ue::FString                                     inString;                                                 // (Parm, ZeroConstructor)
			ue::FName                                       ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;

		params.inString = String;

		ProcessEvent((uintptr_t)ue::cached::KismetStringLibrary, ue::cached::Conv_StringToName, &params);

		return params.ReturnValue;
	}
	inline float STATIC_Cos(float a)
	{

		struct
		{
			float                                              A;                                                        // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;

		params.A = a;

		ProcessEvent((uintptr_t)ue::cached::KismetMathLibrary, ue::cached::Cos, &params);

		return params.ReturnValue;
	}
	inline float STATIC_Sin(float a)
	{

		struct
		{
			float                                              A;                                                        // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;

		params.A = a;

		ProcessEvent((uintptr_t)ue::cached::KismetMathLibrary, ue::cached::Sin, &params);

		return params.ReturnValue;
	}

};
sdk* sdkUssage = new sdk();

namespace find
{
	inline void FnFree(uintptr_t address)
	{
		if (!sdkUssage->isValidPointer(address)) return;


		//auto function = reinterpret_cast<void(__fastcall*)(__int64)>(ue::cached::signatures::FreeFn);
		return; //function((__int64)address);
	}

	inline const wchar_t* GetNameByIndex(int idx)
	{
		std::wstring name = L"";

		if (!idx) return L"";

		auto function = reinterpret_cast<ue::FString * (__fastcall*)(int* index, ue::FString * res)>(ue::cached::signatures::GetNameByIndex);

		int index = idx;

		ue::FString fObjectName;
		function(&index, &fObjectName);

		if (!fObjectName.IsValid()) return L"";

		auto objectName = fObjectName.c_str();

		name = objectName;
		FnFree((uintptr_t)fObjectName.c_str());

		return name.c_str();
	}

	inline const wchar_t* GetObjectName(ue::classes::UObject* object)
	{

		std::wstring name = (L"");

		if (!object) return (L"");

		auto function = reinterpret_cast<ue::FString * (__fastcall*)(int* index, ue::FString * res)>(ue::cached::signatures::GetNameByIndex);

		int index = *(int*)(reinterpret_cast<uint64_t>(object) + 0x18);

		ue::FString fObjectName;
		function(&index, &fObjectName);

		if (!fObjectName.IsValid()) return (L"");

		auto objectName = fObjectName.c_str();

		name = objectName;
		FnFree((uintptr_t)fObjectName.c_str());

		return name.c_str();
	}
	void MsgBox1(const char* str, ...)
	{
		va_list vl;
		va_start(vl, str);
		char buff[1024];
		itsjustaispoof(vsprintf)(buff, str, vl);
		itsjustaispoof(MessageBoxA)(0, buff, e(""), MB_OK | MB_ICONQUESTION);
	}
	inline const wchar_t* GetFullObjectName(ue::classes::UObject* object)
	{
		std::wstring name = (L"");

		for (auto i = 0; object; object = object->Outer, i++)
		{
			if (!object) return (L"");

			auto function = reinterpret_cast<ue::FString * (__fastcall*)(int* index, ue::FString * res)>(ue::cached::signatures::GetNameByIndex);

			int index = *(int*)(reinterpret_cast<uint64_t>(object) + 0x18);

			ue::FString fObjectName;
			function(&index, &fObjectName);

			if (!fObjectName.IsValid()) break;

			auto objectName = fObjectName.c_str();

			name = objectName + std::wstring(i > 0 ? (L".") : (L"")) + name;

			FnFree((uintptr_t)fObjectName.c_str());

		}

		return name.c_str();
	}

	inline void NumChunks(ue::classes::TUObjectArray* ObjectArray, int* start, int* end)
	{
		int cStart = 0, cEnd = 0;

		if (!cEnd)
		{
			while (true)
			{
				if (ObjectArray->Objects[cStart] == nullptr)
				{
					cStart++;
				}
				else
				{
					break;
				}
			}

			cEnd = cStart;
			while (true)
			{
				if (ObjectArray->Objects[cEnd] == nullptr)
				{
					break;
				}
				cEnd++;
			}
		}

		*start = cStart;
		*end = cEnd;
	}

	inline ue::classes::UObject* GetObjByIndex(ue::classes::TUObjectArray* ObjectArray, int32_t index)
	{

		int cStart = 0, cEnd = 0;
		int chunkIndex, chunkSize = 0xFFFF, chunkPos;
		ue::classes::FUObjectItem* Object;

		NumChunks(ObjectArray, &cStart, &cEnd);

		chunkIndex = index / chunkSize;
		if (chunkSize * chunkIndex != 0 &&
			chunkSize * chunkIndex == index)
		{
			chunkIndex--;
		}

		chunkPos = cStart + chunkIndex;
		if (chunkPos < cEnd)
		{
			Object = ObjectArray->Objects[chunkPos] + (index - chunkSize * chunkIndex);
			if (!Object) { return nullptr; }

			return Object->Object;
		}

		return nullptr;
	}


	inline const wchar_t* fix_objectname(const wchar_t* objname)
	{
		wchar_t* _4 = (wchar_t*)objname;

		for (int i = 0; i < wcslen(_4); i++)
		{
			if (_4[i] == L'_')
			{
				if (_4[i + 1] == L'0' ||
					_4[i + 1] == L'1' ||
					_4[i + 1] == L'2' ||
					_4[i + 1] == L'3' ||
					_4[i + 1] == L'4' ||
					_4[i + 1] == L'5' ||
					_4[i + 1] == L'6' ||
					_4[i + 1] == L'7' ||
					_4[i + 1] == L'8' ||
					_4[i + 1] == L'9')
					_4[i] = L'$';

				for (int idx = 1; idx < 7; idx++) {

					if (_4[i + idx] == L'.') break;

					if (_4[i + idx] == L'0' ||
						_4[i + idx] == L'1' ||
						_4[i + idx] == L'2' ||
						_4[i + idx] == L'3' ||
						_4[i + idx] == L'4' ||
						_4[i + idx] == L'5' ||
						_4[i + idx] == L'6' ||
						_4[i + idx] == L'7' ||
						_4[i + idx] == L'8' ||
						_4[i + idx] == L'9') {
						_4[i + idx] = L'$';
					}
				}

			}
		}

		std::wstring str = _4;
		str.erase(std::remove(str.begin(), str.end(), '$'), str.end());
		return str.c_str();
	}
	void MsgBox11(const char* str, ...)
	{
		va_list vl;
		va_start(vl, str);
		char buff[1024];
		itsjustaispoof(vsprintf)(buff, str, vl);
		itsjustaispoof(MessageBoxA)(0, buff, e(""), MB_OK | MB_ICONQUESTION);
	}
	inline PVOID FindObject(const wchar_t* objectname)
	{

		for (auto index = 0x0; index < ue::classes::objects->ObjectCount; index++)
		{

			auto object = GetObjByIndex(ue::classes::objects->ObjectArray, index);

			const wchar_t* tmp_name = GetFullObjectName(object);

			const wchar_t* name = fix_objectname(tmp_name);


			if ((math::custom_wcsstr(name, objectname)))
			{
				return object;
			}
		}

		return 0;
	}


}
class UCanvas {
public:

	int32_t SizeX() {

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return *(int32_t*)((uintptr_t)this + 0x40); //wont change
	}

	int32_t SizeY() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return *(int32_t*)((uintptr_t)this + 0x44); //wont change
	}

	void K2_DrawLine(Vector2 ScreenPositionA, Vector2 ScreenPositionB, float Thickness, ue::FLinearColor RenderColor)
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		struct
		{
			Vector2                                   ScreenPositionA;                                          // (Parm, ZeroConstructor, IsPlainOldData)
			Vector2                                  ScreenPositionB;                                          // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              Thickness;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FLinearColor                                RenderColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.ScreenPositionA = { ScreenPositionA.x, ScreenPositionA.y };
		params.ScreenPositionB = { ScreenPositionB.x, ScreenPositionB.y };
		params.Thickness = Thickness;
		params.RenderColor = { (float)RenderColor.R, (float)RenderColor.G, (float)RenderColor.B, (float)RenderColor.A };

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::render::K2_DrawLine, &params);
	}

	void K2_DrawBox(Vector2 ScreenPosition, Vector2 ScreenSize, float Thickness, ue::FLinearColor RenderColor)
	{

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		struct
		{
			Vector2                                  ScreenPosition;                                           // (Parm, ZeroConstructor, IsPlainOldData)
			Vector2                                   ScreenSize;                                               // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              Thickness;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FLinearColor                                RenderColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.ScreenPosition = { ScreenPosition.x, ScreenPosition.y };
		params.ScreenSize = { ScreenSize.x, ScreenSize.y };
		params.Thickness = Thickness;
		params.RenderColor = { (float)RenderColor.R, (float)RenderColor.G, (float)RenderColor.B, (float)RenderColor.A };

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::render::K2_DrawBox, &params);
	}

	void K2_DrawText(Vector2 ScreenPosition, ue::FLinearColor RenderColor, ue::FString RenderText, bool bCentered = true, bool bOutlined = false)
	{

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		struct
		{
			uintptr_t RenderFont;                                               // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FString                                     RenderText;                                               // (Parm, ZeroConstructor)
			Vector2                                   ScreenPosition;                                           // (Parm, ZeroConstructor, IsPlainOldData)
			Vector2                                   Scale;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FLinearColor                                RenderColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              Kerning;                                                  // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FLinearColor                                ShadowColor;                                              // (Parm, ZeroConstructor, IsPlainOldData)
			Vector2                                   ShadowOffset;                                             // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bCentreX;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bCentreY;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               bOutlined;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FLinearColor                                OutlineColor;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.RenderFont = uintptr_t(ue::cached::objects::render::Font);
		params.RenderText = RenderText;
		params.ScreenPosition = { ScreenPosition.x, ScreenPosition.y };
		params.Scale = { 1.f, 1.f };
		params.RenderColor = { (float)RenderColor.R, (float)RenderColor.G, (float)RenderColor.B, (float)RenderColor.A };
		params.Kerning = false;
		params.ShadowColor = { 0.f, 0.f, 0.f, 0.f };
		params.ShadowOffset = { ScreenPosition.x + 1.5f, ScreenPosition.y + 1.5f };

		if (bCentered)
		{
			params.bCentreX = true;
			params.bCentreY = true;
		}
		else
		{
			params.bCentreX = false;
			params.bCentreY = false;
		}

		params.bOutlined = bOutlined;
		params.OutlineColor = { 0.0f, 0.0f, 0.0f, 1.f };

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::render::K2_DrawText, &params);
	}

	Vector2 K2_TextSize(ue::FString RenderText)
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return { 0, 0 };

		struct
		{
			uintptr_t RenderFont;                                               // (Parm, ZeroConstructor, IsPlainOldData)
			ue::FString                                     RenderText;                                               // (Parm, ZeroConstructor)
			Vector2                                   Scale;                                                    // (Parm, ZeroConstructor, IsPlainOldData)
			Vector2                                   ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;

		params.RenderFont = uintptr_t(ue::cached::objects::render::Font);
		params.RenderText = RenderText;
		params.Scale = { 0.6f, 0.6f };

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::render::K2_TextSize, &params);

		return { params.ReturnValue.x, params.ReturnValue.y };
	}
};
// Enum FortniteGame.EFortItemTier
enum class EFortItemTier : uint8_t
{
	No_Tier = 0,
	I = 1,
	II = 2,
	III = 3,
	IV = 4,
	V = 5,
	VI = 6,
	VII = 7,
	VIII = 8,
	IX = 9,
	X = 10,
	NumItemTierValues = 11,
	EFortItemTier_MAX = 12
};
class AFortItemDefinition {
public:

	ue::FText DisplayName() {

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return {};
		return *(ue::FText*)((uintptr_t)this + 0x90); //FortItemDefinition::DisplayName 0x90
	}
	EFortItemTier Tier() {

		return *(EFortItemTier*)((uintptr_t)this + 0x73); //FortItemDefinition::Tier 0x74
	}
};




class AWeapon {
public:

	AFortItemDefinition* WeaponData() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (AFortItemDefinition*)(*(uintptr_t*)(this + 0x430)); //FortWeapon::WeaponData 0x3F0
	}
	void RapidFire() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		float a = 0;
		float b = 0;
		a = *(float*)((uintptr_t)this + 0xb40); //FortWeapon::LastFireTime 0xAB0
		b = *(float*)((uintptr_t)this + 0xb44); //FortWeapon::LastFireTimeVerified 0xAB4
		*(float*)((uintptr_t)this + 0xb40) = a + b - 1000; //FortWeapon::LastFireTime 0xAB0
	}
	// Enum FortniteGame.EFortItemTier
	int GetBulletsPerClip()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortWeapon.GetBulletsPerClip"));


		struct
		{
			int                                                ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		return params.ReturnValue;
	}
	float GetProjectilePitchOffset()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortWeapon.GetProjectilePitchOffset"));


		struct
		{
			float                                                ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		return params.ReturnValue;
	}
	float GetProjectileSpeed()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortWeapon.GetProjectileSpeed"));


		int result;

		sdkUssage->ProcessEvent((uintptr_t)this, fn, &result);

		return result;
	}
	int GetMagazineAmmoCount()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortWeapon.GetMagazineAmmoCount"));

		int result;

		sdkUssage->ProcessEvent((uintptr_t)this, fn, &result);

		return result;
	}

};
class APlayerState {
public:

	uintptr_t TeamIndex() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return *(uintptr_t*)(this + 0x10B0); // FortPlayerStateAthena::TeamIndex 0x1098
	}
	int32_t SeasonLevelUIDisplay() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return *(int32_t*)(this + 0x1100); //FortPlayerStateAthena::SeasonLevelUIDisplay 0x10B0
	}
	ue::FString GetPlayerName() {

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return L"";
		ue::FString PlayerName;
		sdkUssage->ProcessEvent((uintptr_t)this, ue::cached::objects::actor::GetPlayerName, &PlayerName);
		if (!PlayerName.IsValid()) return L"";
		return PlayerName;
	}
	FUniqueNetIdRepl GetPlayerUniqueNetID() {
		if (sdkUssage->isValidPointer((uintptr_t)this)) {
			static ue::classes::UObject* fn = NULL;

			if (!fn)
				fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerState.GetPlayerUniqueNetID"));
			FUniqueNetIdRepl Platform;
			sdkUssage->ProcessEvent((uintptr_t)this, fn, &Platform);

			return Platform;
		}

	}


	ue::FString GetPlatform() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return L"";
		static ue::classes::UObject* fn = NULL;

		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerState.GetPlatform"));
		ue::FString Platform;
		sdkUssage->ProcessEvent((uintptr_t)this, fn, &Platform);
		if (!Platform.IsValid()) return L"";
		return Platform;
	}
	int GetKillScore()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerStateAthena.GetKillScore"));


		struct
		{
			int                                                ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		return params.ReturnValue;
	}
};


class ARootComponent {
public:

	Vector3 RelativeLocation() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return { 0, 0, 0 };

		return *(Vector3*)((uintptr_t)this + 0x128); //SceneComponent::RelativeLocation 0x128
	}
	Vector3 GetComponentVelocity() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return { 0, 0, 0 };

		return *(Vector3*)((uintptr_t)this + 0x170); //SceneComponent::RelativeLocation 0x128
	}
	void SetWorldScale3D(Vector3 NewScale)
	{

		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Function Engine.SceneComponent.SetWorldScale3D"));

		struct AFortPlayerPawn_OnEndWaterSprintBoost_Params
		{
			struct Vector3                                     NewScale;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.NewScale = NewScale;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}


	// ItemDefinition
	/*Vector3 RelativeRotation() {
		if (!Memory::IsValidPointer((uintptr_t)this)) return { 0, 0, 0 };
		return *(Vector3*)((uintptr_t)this + 0x128);
	}

	Vector3 GetComponentVelocity()
	{
		if (!Memory::IsValidPointer((uintptr_t)this)) return { 0, 0, 0 };
		StructsParams::USceneComponent_GetComponentVelocity_Params params;

		FN::ProcessEvent(uintptr_t(this), ObjectsAddresses::GetComponentVelocity, &params);

		return params.ReturnValue;
	}*/
};
class UFortVehicleFuelComponent {
public:

};
class APlayerController;
class AFortAthenaVehicle {
public:
	UFortVehicleFuelComponent* CachedFuelComponent() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (UFortVehicleFuelComponent*)(*(uintptr_t*)((uintptr_t)this + 0x10A0)); //FortAthenaVehicle::CachedFuelComponent 0x10A0
	}
	void SetHealthPercent(float Percent) {
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortAthenaVehicle.SetHealthPercent"));

		struct AFortPlayerPawn_bEnableDetection
		{
			float Percent;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.Percent = Percent;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}


	void SetOnFire(bool bNewValue) {
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortAthenaVehicle.SetOnFire"));

		struct AFortPlayerPawn_bEnableDetection
		{
			bool bNewValue;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.bNewValue = bNewValue;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}
};

class kek {
public:
	ue::FText* DisplayName() {
		if (!sdkUssage->isValidPointer(uintptr_t(this))) return 0;
		return reinterpret_cast<ue::FText*>(*(uintptr_t*)((uintptr_t)this + 0x90));
	}

	BYTE Tier() {
		if (!sdkUssage->isValidPointer(uintptr_t(this))) return false;
		return *(BYTE*)((uintptr_t)this + 0x74);
	}
};

class AFortPawn {
public:
	const wchar_t* ObjectName() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return (L"");
		return find::GetObjectName((ue::classes::UObject*)this);
	}



	kek* ItemDefinition() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (kek*)(*(uintptr_t*)((uintptr_t)this + (0x318 + 0x18)));
	}

	APlayerState* PlayerState() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (APlayerState*)(*(uintptr_t*)((uintptr_t)this + 0x2a8));
	}
	ARootComponent* RootComponent() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (ARootComponent*)(*(uintptr_t*)((uintptr_t)this + 0x190));
	}
	APlayerController* PlayerController() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (APlayerController*)(*(uintptr_t*)((uintptr_t)this + 0x30)); //wont change
	}
	void AimWhileJumping(bool action) {
		*(bool*)((uintptr_t)this + 0x4f40) = action; //FortPlayerPawnAthena::bADSWhileNotOnGround 0x4B25
	}
	BOOL IsA(PVOID parentClass) {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return false;
		ue::classes::UObject* object = (ue::classes::UObject*)this;
		for (auto super = object->Class; super; super = super->SuperClass) {
			if (super == parentClass) {
				return true;
			}
		}
		return false;
	}
	uintptr_t Mesh() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return *(uintptr_t*)((uintptr_t)this + 0x310); //Character::Mesh 0x310
	}

	Vector3 GetBone(int Id)
	{

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return { 0, 0, 0 };

		auto mesh = Mesh();
		if (!mesh) return { 0, 0, 0 };

		auto function = ((ue::FMatrix * (__fastcall*)(uintptr_t, ue::FMatrix*, int))(ue::cached::signatures::GetBoneMatrix));
		function(mesh, ue::myMatrix, Id);

		return { ue::myMatrix->M[3][0], ue::myMatrix->M[3][1], ue::myMatrix->M[3][2] };
	}
	AWeapon* CurrentWeapon() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (AWeapon*)(*(uintptr_t*)((uintptr_t)this + 0x908)); //FortPawn::CurrentWeapon 0x868
	}

	AFortAthenaVehicle* jeremyvehicle() {

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (AFortAthenaVehicle*)(*(uintptr_t*)((uintptr_t)this + 0x24c8));
	}




	struct AFortPlayerPawn_GetVehicleActor_Params
	{
		class AFortPawn* ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm)
	};




	void ApplyPawnHighlight(ue::FLinearColor InnerCol, ue::FLinearColor OuterCol)
	{
		//AFortPawn -> ApplyPawnHighlight

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return;

		struct APlayerPawn_Athena_C_ApplyPawnHighlight_Params
		{
			uintptr_t Source; // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
			ue::FPawnHighlight HitGlow; // (BlueprintVisible, BlueprintReadOnly, Parm, IsPlainOldData)
		} params;

		ue::FPawnHighlight HitGlow;

		HitGlow.FresnelBrightness_23_52B0F96447FF640F47DF2895B0602E92 = 0.f;
		HitGlow.FresnelExponent_24_B427CF0C441AA37ED49833BF7579DE6D = 0;
		HitGlow.Inner_21_4CC2801147EA190DE16F59B34F36853E = { (float)InnerCol.R, (float)InnerCol.G, (float)InnerCol.B, (float)InnerCol.A };//{ 1.f, 0.f, 0.f, 1.f };//{ (float)col.R, (float)col.G, (float)col.B, (float)col.A };
		HitGlow.Outer_22_5A1D7D0543D303E8B54B66A7F7BD2E2E = { (float)OuterCol.R, (float)OuterCol.G, (float)OuterCol.B, (float)OuterCol.A };//{ 0.f, 1.f, 0.f, 1.f };//{ (float)col.R, (float)col.G, (float)col.B, (float)col.A };
		HitGlow.Priority_28_E2E1B5344846E187B9C11B863A7F0698 = 0.f;
		HitGlow.UsesPulse_25_E29229F64E540F0617E4C4987AD77605 = 0.f;


		params.Source = uintptr_t(this);
		params.HitGlow = HitGlow;

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::ApplyPawnHighlight, &params);

	}
	VOID ScalePlayerUp()
	{
		//AFortPawn -> ApplyPawnHighlight

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return;
		struct
		{
		} params;
		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::ScalePlayerUp, &params);

	}
	VOID ScalePlayerDown()
	{
		//AFortPawn -> ApplyPawnHighlight

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return;
		struct
		{
		} params;
		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::ScalePlayerDown, &params);

	}
	VOID isHiddingInProp()
	{
		//AFortPawn -> ApplyPawnHighlight

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return;
		struct
		{
			bool                                               IsHidden;                                                 // (BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData)
		} params;
		params.IsHidden = true;
		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::isHiddingInProp, &params);

	}
	BOOL SetCharacterBodyVisibilityForPossession(bool bVisible)
	{
		//AFortPawn -> ApplyPawnHighlight

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return 0;
		struct
		{
			bool                                               bVisible;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;
		params.bVisible = bVisible;
		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::SetCharacterBodyVisibilityForPossession, &params);

	}
	struct FZiplinePawnState
	{
		struct AFortAthenaZiplineBase* Zipline; // 0x0 (0x8)
		struct AFortAthenaZiplineBase* PreviousZipline; // 0x8 (0x8)
		struct UPrimitiveComponent* InteractComponent; // 0x10 (0x8)
		bool bIsZiplining; // 0x18 (0x1)
		bool bJumped; // 0x19 (0x1)
		bool bReachedEnd; // 0x1a (0x1)
		unsigned char unreflected_1b[0x1]; // 0x1b (0x1) 
		int AuthoritativeValue; // 0x1c (0x4)
		struct FVector* SocketOffset; // 0x20 (0x18)
		float TimeZipliningBegan; // 0x38 (0x4)
		float TimeZipliningEndedFromJump; // 0x3c (0x4)
		struct FTimerHandle* RemoveZiplineFromIgnoreWhenMovingTimerHandle; // 0x40 (0x8)
	};
	void  SetCanSlide()
	{

		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerPawn.SetCanSlide"));

		struct AFortPlayerPawn_SetCanSlide
		{
			bool bCanSlide;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.bCanSlide = true;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}
	void ServerSendAimbotDetectionStatus()
	{

		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerPawn.ServerSendAimbotDetectionStatus"));

		struct AFortPlayerPawn_bEnableDetection
		{
			TArray<unsigned char> bEnableDetection;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;
		TArray<unsigned char> a;
		params.bEnableDetection = a;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}
	void ServerSetAimbotDetection()
	{

		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerPawn.ServerSetAimbotDetection"));

		struct AFortPlayerPawn_bEnableDetection
		{
			bool bEnableDetection;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;

		params.bEnableDetection = false;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}
	void ServerSendZiplineState(bool a)
	{

		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerPawn.ServerSendZiplineState"));

		struct AFortPlayerPawn_FZiplinePawnState_Params
		{
			struct FZiplinePawnState                                     InZiplineState;                                                 // (Parm, ZeroConstructor, IsPlainOldData)
		} params;


		params.InZiplineState.bIsZiplining = a;
		//	params.InZiplineState.AuthoritativeValue = 0;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}
	BOOL K2_SetActorLocation(const Vector3 NewLocation, bool bSweep, bool bTeleport)
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		struct
		{
			Vector3							NewLocation;
			bool							bSweep;
			int								SweepHitResult;
			bool							bTeleport;
			bool							ReturnValue;
		} params;

		params.NewLocation = NewLocation;
		params.bSweep = bSweep;
		params.bTeleport = bTeleport;

		sdkUssage->ProcessEvent((uintptr_t)this, ue::cached::objects::K2_SetActorLocation, &params);

		return params.ReturnValue;
	}



	void TeleportToMarker()
	{

		auto Uworld = (*(uintptr_t*)(ue::cached::signatures::GWorld));
		for (int li = 0; li < read1<DWORD>(Uworld + 0x170 + sizeof(PVOID)); ++li) {
			DWORD_PTR levels = read1<uintptr_t>(Uworld + 0x170);
			if (!levels) break;

			DWORD_PTR level = read1<uintptr_t>(levels + li * sizeof(PVOID));

			if (!level) std::cout << e("Failed LEVEL\n");

			auto lc = read1<DWORD>(level + 0x98 + sizeof(PVOID));

			DWORD_PTR AActorsLEVE = read1<uintptr_t>(level + 0x98);

			for (int ai = 0; ai < lc; ++ai)
			{
				auto Acotr = read1<uintptr_t>(AActorsLEVE + ai * sizeof(PVOID));
				//Get Name

				char* CurObjectName1 = sdk_p->fGetObjectName(Acotr);

				if (strstr(CurObjectName1, e("AthenaPlayerMarker")))
				{
					auto Rootcomp = read1<uint64_t>(Acotr + 0x190);
					auto Pos = read1<Vector3>(Rootcomp + 0x128);

					this->K2_SetActorLocation(Pos, false, true);

				}
			}
		}
	}


	BOOL K2_TeleportTo(const Vector3& DestLocation, const Vector3& DestRotation)
	{

		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		struct
		{
			Vector3										DestLocation;                                             // (Parm, ZeroConstructor, IsPlainOldData)
			Vector3										DestRotation;                                             // (Parm, ZeroConstructor, IsPlainOldData)
			bool										ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;

		params.DestLocation = DestLocation;
		params.DestRotation = DestRotation;

		sdkUssage->ProcessEvent((uintptr_t)this, ue::cached::objects::K2_TeleportTo, &params);

		return params.ReturnValue;
	}
};
class APlayerCameraManager {
public:
	Vector3 GetCameraLocation()
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return { 0,0,0 };

		struct APlayerCameraManager_GetCameraLocation_Params
		{
			Vector3                                    ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};
		APlayerCameraManager_GetCameraLocation_Params params;

		sdkUssage->ProcessEvent((uintptr_t)this, ue::cached::objects::camera::GetCameraLocation, &params);

		return params.ReturnValue;
	}
	Vector3 GetCameraRotation()
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return { 0,0,0 };


		struct APlayerCameraManager_GetCameraRotation_Params
		{
			Vector3                                    ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};
		APlayerCameraManager_GetCameraRotation_Params params;

		sdkUssage->ProcessEvent((uintptr_t)this, ue::cached::objects::camera::GetCameraRotation, &params);

		return params.ReturnValue;
	}
	float GetFOVAngle()
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		struct APlayerCameraManager_GetFOVAngle_Params
		{
			float                                              ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		};

		APlayerCameraManager_GetFOVAngle_Params params;

		sdkUssage->ProcessEvent((uintptr_t)this, ue::cached::objects::camera::GetFOVAngle, &params);

		return params.ReturnValue;
	}




#define M_PI	3.14159265358979323846264338327950288419716939937510
	Vector2 ProjectWorldLocationToScreen(int SizeX, int SizeY, Vector3 world_location)
	{
		Vector3 CameraLocation = GetCameraLocation();

		Vector3 CameraRotation = GetCameraRotation();

		float FOVAngle = GetFOVAngle();


		Vector2 output;
		float delta[3];
		float sp = 0, cp = 0, sy = 0, cy = 0, sr = 0, cr = 0;
		float axisx[3];
		float axisy[3];
		float axisz[3];
		float transformed[3];

		delta[0] = world_location.x - CameraLocation.x;
		delta[1] = world_location.y - CameraLocation.y;
		delta[2] = world_location.z - CameraLocation.z;

		sp = crt::sinf(CameraRotation.x * M_PI / 180);
		cp = crt::cosf(CameraRotation.x * M_PI / 180);
		sy = crt::sinf(CameraRotation.y * M_PI / 180);
		cy = crt::cosf(CameraRotation.y * M_PI / 180);
		sr = crt::sinf(CameraRotation.z * M_PI / 180);
		cr = crt::cosf(CameraRotation.z * M_PI / 180);

		axisx[0] = cp * cy;
		axisx[1] = cp * sy;
		axisx[2] = sp;

		axisy[0] = sr * sp * cy - cr * sy;
		axisy[1] = sr * sp * sy + cr * cy;
		axisy[2] = -sr * cp;

		axisz[0] = -(cr * sp * cy + sr * sy);
		axisz[1] = cy * sr - cr * sp * sy;
		axisz[2] = cr * cp;

		transformed[0] = delta[0] * axisy[0] + delta[1] * axisy[1] + delta[2] * axisy[2];
		transformed[1] = delta[0] * axisz[0] + delta[1] * axisz[1] + delta[2] * axisz[2];
		transformed[2] = delta[0] * axisx[0] + delta[1] * axisx[1] + delta[2] * axisx[2];

		if (transformed[2] < 1)
			transformed[2] = 1;

		if (transformed[2] / 100 > 0)
		{
			float tmpx = 0, tmpy = 0;

			tmpx = (float)(SizeX / 2);
			tmpy = (float)(SizeY / 2);

			output.x = tmpx + transformed[0] * tmpx / crt::tanf(FOVAngle * M_PI / 360) / transformed[2];
			output.y = tmpy - transformed[1] * tmpx / crt::tanf(FOVAngle * M_PI / 360) / transformed[2];

			return output;
		}

		return Vector2(0, 0);
	}
	Vector2 WorldToScreen(Vector3 WorldLocation)
	{

		return ProjectWorldLocationToScreen(width, height, WorldLocation);
	}
};
enum class EFortResourceType : uint8_t
{
	Wood = 0,
	Stone = 1,
	Metal = 2,
	Permanite = 3,
	GoldCurrency = 4,
	Ingredient = 5,
	None = 6,
	EFortResourceType_MAX = 7
};
template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};
class APlayerController {
public:
	AFortPawn* LocalPawn() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (AFortPawn*)(*(uintptr_t*)((uintptr_t)this + 0x330)); //PlayerController::AcknowledgedPawn 0x328
	}

	APlayerCameraManager* PlayerCameraManager() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (APlayerCameraManager*)(*(uintptr_t*)(this + 0x340)); //PlayerController::PlayerCameraManager 0x338
	}
	//Might not be right!!!
	/*Vector2 WorldToScreen(Vector3 WorldLocation)
	{
		if (!sdkUssage->isValidPointer(uintptr_t(this))) return Vector2(0, 0);

		Vector2 Output;
		uintptr_t ProjectWorldToScreen;

		ProjectWorldToScreen = (uintptr_t)GetModuleHandleA(0) + 0x42C7414;
		auto WorldToScreen = reinterpret_cast<bool(__fastcall*)(uintptr_t, Vector3, Vector2*, char)>(ProjectWorldToScreen);
		WorldToScreen((uintptr_t)this, WorldLocation, &Output, (char)0);

		return { Output.x, Output.y };
	}*/

	static bool IsVisible(APlayerController* PlayerController, AFortPawn* Pawn, Vector3* ViewPoint)
	{

		auto subIsVisible = reinterpret_cast<bool(__fastcall*)(APlayerController * PlayerController, AFortPawn * Pawn, Vector3 * ViewPoint)>(ue::cached::signatures::LineSightOfTo);
		return subIsVisible(PlayerController, Pawn, ViewPoint);

	}

	/*Vector2 WorldToScreen(Vector3 WorldLocation)
	{

		if (!sdkUssage->isValidPointer(uintptr_t(this))) return Vector2(0, 0);

		Vector2 Output;

		auto WorldToScreen = reinterpret_cast<bool(__fastcall*)(uintptr_t, Vector3, Vector2*, char)>(ue::cached::signatures::ProjectWorldToScreen);
		WorldToScreen((uintptr_t)this, WorldLocation, &Output, (char)0);

		return { Output.x, Output.y };
	}*/
	BOOL ClientSetRotation(const Vector3& NewRotation, bool bResetCamera)
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		struct AController_ClientSetRotation_Params
		{
			Vector3										NewRotation;                                              // (Parm, ZeroConstructor, IsPlainOldData)
			bool										bResetCamera;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		};

		AController_ClientSetRotation_Params params;
		params.NewRotation = NewRotation;
		params.bResetCamera = bResetCamera;

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::ClientSetRotation, &params);
		return TRUE;
	}
	void Fov(float newfov)
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		struct APlayerController_FOV_Params
		{
			float                                              NewFOV;                                                   // (Parm, ZeroConstructor, IsPlainOldData)
		};

		APlayerController_FOV_Params params;
		params.NewFOV = newfov;

		sdkUssage->ProcessEvent(uintptr_t(this), ue::cached::objects::actor::Fov, &params);
	}
	void ServerRespawnFromDBNO()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerRespawnFromDBNO"));

		struct
		{
			//	ue::FString s;
		} params;

		//	params.s = a;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	int getClientCapBandwidth()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ClientCapBandwidth"));

		struct
		{
			int Cap;
		} params;




		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		return params.Cap;
	}
	void ClientCapBandwidth(int cap)
	{
		//	ue::classes::UObject* fn = NULL;
		ue::classes::UObject* fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ClientCapBandwidth"));

		struct
		{
			int Cap;
		} params;

		params.Cap = cap;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	bool ServerSetSpectatorLocation(ue::FVector NewLoc, ue::FRotator NewRot)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerSetSpectatorLocation"));

		struct
		{
			ue::FVector                 NewLoc;
			ue::FRotator                NewRot;
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;

		params.NewLoc = NewLoc;
		params.NewRot = NewRot;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);


		return params.ReturnValue;
	}
	void AddRollInput(float a)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.AddRollInput"));

		struct
		{
			float a;
		} params;

		params.a = a;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	void AddPitchInput(float a)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.AddPitchInput"));

		struct
		{
			float a;
		} params;

		params.a = a;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	void AddYawInput(float a)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.AddYawInput"));

		struct
		{
			float a;
		} params;

		params.a = a;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}

	void ServerShortTimeout()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerShortTimeout"));

		struct
		{

		} params;



		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}

	void ServerExec(const wchar_t* name)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerExec"));

		struct
		{
			ue::FString                 S;
		} params;

		params.S = name;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	FAsyncPhysicsTimestamp getServerSendLatestAsyncPhysicsTimestamp()
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerSendLatestAsyncPhysicsTimestamp"));

		struct
		{
			FAsyncPhysicsTimestamp                 S;
		} params;



		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		return params.S;

	}
	void ServerSendLatestAsyncPhysicsTimestamp(int ServerFrame)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerSendLatestAsyncPhysicsTimestamp"));

		struct
		{
			FAsyncPhysicsTimestamp                 S;
		} params;

		params.S.ServerFrame = ServerFrame;
		params.S.LocalFrame = ServerFrame;
		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	void ServerChangeName(const wchar_t* name)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.ServerChangeName"));

		struct
		{
			ue::FString                 S;
		} params;

		params.S = name;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	void SetName(const wchar_t* name)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.SetName"));

		struct
		{
			ue::FString                 S;
		} params;

		params.S = name;

		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);

	}
	bool IsInputKeyDown(const struct ue::FKey& Key)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.IsInputKeyDown"));

		struct
		{
			struct ue::FKey                                        Key;                                                      // (Parm)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;
		params.Key = Key;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);


		return params.ReturnValue;
	}
	bool WasInputKeyJustPressed(const struct ue::FKey& Key)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"Engine.PlayerController.WasInputKeyJustPressed"));

		struct
		{
			struct ue::FKey                                        Key;                                                      // (Parm)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;
		params.Key = Key;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);


		return params.ReturnValue;
	}

	void ServerSetVehicleHealth(float NewHealth)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject(e(L"FortniteGame.FortPlayerControllerZone.ServerSetVehicleHealth"));

		struct
		{
			float                                              NewHealth;                                                // (Parm, ZeroConstructor, IsPlainOldData)
		} params;
		params.NewHealth = NewHealth;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
	}
	bool ClientStopForceFeedback(float* LocationX, float* LocationY)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject((L"Engine.PlayerController.ClientStopForceFeedback"));


		struct
		{
			float                                              LocationX;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
			float                                              LocationY;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		if (LocationX != nullptr)
			*LocationX = params.LocationX;
		if (LocationY != nullptr)
			*LocationY = params.LocationY;
		return params.ReturnValue;
	}

	bool GetMousePosition(float* LocationX, float* LocationY)
	{
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject((L"Engine.PlayerController.GetMousePosition"));


		struct
		{
			float                                              LocationX;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
			float                                              LocationY;                                                // (Parm, OutParm, ZeroConstructor, IsPlainOldData)
			bool                                               ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData)
		} params;


		sdkUssage->ProcessEvent(uintptr_t(this), fn, &params);
		if (LocationX != nullptr)
			*LocationX = params.LocationX;
		if (LocationY != nullptr)
			*LocationY = params.LocationY;
		return params.ReturnValue;
	}

	BOOL SetMouseLocation(const Vector2 NewLocation)
	{
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		static ue::classes::UObject* fn = NULL;
		if (!fn)
			fn = (ue::classes::UObject*)find::FindObject((L"Engine.PlayerController.SetMouseLocation"));
		struct
		{
			int X;
			int Y;
		} params;

		params.X = NewLocation.y;
		params.Y = NewLocation.x;


		sdkUssage->ProcessEvent((uintptr_t)this, fn, &params);
	}
	void EditModeDistance() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		*(float*)((uintptr_t)this + 0x19a8) = 20;
	}
	void TurboBuildInterval() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return;

		*(float*)((uintptr_t)this + 0x28a8) = 0000.1;
	}


};
class UViewportClient {
public:

};
class ULocalPlayer {
public:
	APlayerController* PlayerController() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (APlayerController*)(*(uintptr_t*)((uintptr_t)this + 0x30)); //wont change
	}
	UViewportClient* ViewportClient() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (UViewportClient*)(*(uintptr_t*)((uintptr_t)this + 0x78)); //wont change
	}

};
class ULocalPlayers {
public:
	ULocalPlayer* LocalPlayer() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (ULocalPlayer*)(*(uintptr_t*)((uintptr_t)this));
	}
};
class UGameInstance {
public:
	ULocalPlayers* LocalPlayers() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (ULocalPlayers*)(*(uintptr_t*)((uintptr_t)this + 0x38)); //wont change
	}
};
class AActors {
public:

	AFortPawn* GetCurrentActor(int i) {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (AFortPawn*)(*(uintptr_t*)(this + i * sizeof(uintptr_t)));
	}
};
class ULevel {
public:



	int ActorCount() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;

		return (int)(*(uintptr_t*)(this + 0xA0)); //wont change
	}
	ue::TArray<AFortPawn*> ActorArray() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return {};
		return *(ue::TArray<AFortPawn*>*)((uintptr_t)this + 0x98); //wont change
	}
	AActors* Actors() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (AActors*)(*(uintptr_t*)(this + 0x98)); //wont change
	}
};
class GWorld {
public:
	UGameInstance* OwningGameInstance() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (UGameInstance*)(*(uintptr_t*)((uintptr_t)this + 0x1b8)); //World::OwningGameInstance 0x1A8
	}
	ue::TArray<ULevel*> Levels() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return {};

		return *(ue::TArray<ULevel*>*)((uintptr_t)this + ue::cached::offsets::Levels);
	}
	ULevel* PersistentLevel() {
		if (!sdkUssage->isValidPointer((uintptr_t)this)) return 0;
		return (ULevel*)(*(uintptr_t*)(this + 0x30)); //wont change
	}
};
