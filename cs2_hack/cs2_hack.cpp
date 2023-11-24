#include <iostream>
#include "offsets.hpp"
#include "client.dll.hpp"
#include <Windows.h>
#include "ProcessManager.hpp"

using namespace client_dll;
using namespace CBasePlayerWeaponVData;
using namespace C_BaseEntity;
using namespace CEffectData;
using namespace CCSPlayerController;
using namespace C_CSPlayerPawnBase;
using namespace CCSPlayerController;
using namespace C_BasePlayerPawn;

struct vec3
{
    float x, y, z;
};

struct CurrProcess
{
    DWORD64 localPlayer;
    DWORD64 processID;
    uintptr_t moduleBase;
    BYTE flag;
    DWORD64 jump;
    DWORD64 localPawn;
    uintptr_t entityList;
    DWORD64 listEntry;
    DWORD64 playerPawn;
    int localTeam;
    float tDelay;
}proc;

struct GameOffsets
{
    DWORD localPawn = dwLocalPlayerPawn;
    DWORD playerPawn = m_hPlayerPawn;
    DWORD fJump = dwForceJump;
    DWORD flags = C_BaseEntity::m_fFlags;
    DWORD locPlayerController = dwLocalPlayerController;
    DWORD entList = dwEntityList;
    DWORD health = m_iHealth;
    DWORD flashDur = m_flFlashDuration;
    DWORD crosshairID = m_iIDEntIndex;
    DWORD team = m_iTeamNum;
    DWORD forceAttack = dwForceAttack;
    DWORD currWeapon = m_pClippingWeapon;
    DWORD vecOrigin = m_vOldOrigin;
    DWORD isScoped = m_bIsScoped;

}offset;


void writeCheatstoConsole(bool bhop, bool antiflash, bool bTrigger)
{
    system("CLS");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    std::cout << "[NUM1] Bhop: ";

    if (bhop)
    {
        SetConsoleTextAttribute(hConsole, 10);

        std::cout << "ON" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 12);

        std::cout << "OFF" << std::endl;
    }

    SetConsoleTextAttribute(hConsole, 7);

    std::cout << "[NUM2] Anti Flash: ";

    if (antiflash)
    {
        SetConsoleTextAttribute(hConsole, 10);

        std::cout << "ON" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 12);

        std::cout << "OFF" << std::endl;
    }

    SetConsoleTextAttribute(hConsole, 7);

    std::cout << "[NUM3] TriggerBot: ";

    if (bTrigger)
    {
        SetConsoleTextAttribute(hConsole, 10);

        std::cout << "ON" << std::endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 12);

        std::cout << "OFF" << std::endl;
    }


    SetConsoleTextAttribute(hConsole, 7);
}




void setTBDelay(float dist, std::string currWeapon)
{
    float delay = 0;

    if (currWeapon == "m4a1") // M4A1-S
    {
        delay = 3.0;
    }
    else if (currWeapon == "m4a4") // M4A4
    {
        delay = 3.3;
    }
    else if (currWeapon == "mp5sd") // MP5-SD
    {
        delay = 3.0;
    }
    else if (currWeapon == "aug") // AUG
    {
        delay = 3.5;
    }
    else if (currWeapon == "famas") // FAMAS
    {
        delay = 3.3;
    }
    else if (currWeapon == "elite") // DUAL BERETTAS
    {
        delay = 3.0;
    }
    else if (currWeapon == "fiveseven") // Five Seven
    {
        delay = 3.0;
    }
    else if (currWeapon == "p90") // P90
    {
        delay = 0.5;
    }
    else if (currWeapon == "mac10") // Mac-10
    {
        delay = 0.5;
    }
    else if (currWeapon == "ump45") // UMP-45
    {
        delay = 2.5;
    }
    else if (currWeapon == "bizon") // PP-bizon
    {
        delay = 0.5;
    }
    else if (currWeapon == "mp7") // mp7
    {
        delay = 2.5;
    }
    else if (currWeapon == "mp9") // mp9
    {
        delay = 1.5;
    }
    else if (currWeapon == "ak47") // ak47
    {
        delay = 3.3;
    }
    else if (currWeapon == "ssg08") // ssg08
    {
        delay = 0.2;
    }
    else if (currWeapon == "hkp2000") // usp-s
    {
        delay = 4.5;
    }
    else if (currWeapon == "glock") // glock-18
    {
        delay = 1.5;
    }
    else if (currWeapon == "galilar") // Galil-AR
    {
        delay = 3.0;
    }
    else if (currWeapon == "sg556") // SSG-553
    {
        delay = 3.5;
    }
    else if (currWeapon == "awp") // AWP
    {
        delay = 0.2;
    }
    else if (currWeapon == "deagle") // Desert Eagle
    {
        delay = 12.5;
    }
    else
    {
        delay = 3.0;
    }

    proc.tDelay = delay * dist;
}

std::string getCurrweapon()
{
    DWORD64 CurrWeaponAddress = 0;
    char Buffer[40]{};

    CurrWeaponAddress = ProcessMgr.TraceAddress(proc.localPlayer + offset.currWeapon, { 0x10, 0x20, 0x0 });

    if (CurrWeaponAddress == 0)
    {
        return "ERROR";
    }

    ProcessMgr.ReadMemory(CurrWeaponAddress, Buffer, 40);

    std::string WeaponName = std::string(Buffer);

    std::size_t index = WeaponName.find("_");

    if (index == std::string::npos || WeaponName.empty())
    {
        return "ERROR";
    }
    else
    {
        return (WeaponName.substr(index + 1, WeaponName.size() - index - 1));
    }
    return "ERROR";
}

float getDistance(DWORD64 ent)
{
    vec3 localPlayerLoc, enemyLoc;

    ProcessMgr.ReadMemory<vec3>(proc.localPlayer + offset.vecOrigin, localPlayerLoc);
    ProcessMgr.ReadMemory<vec3>(ent + offset.vecOrigin, enemyLoc);

    return (sqrt(pow(localPlayerLoc.x - enemyLoc.x, 2) + pow(localPlayerLoc.y - enemyLoc.y, 2) + pow(localPlayerLoc.z - enemyLoc.z, 2)) * 0.0254);
}

void shoot()
{
    int shoot = 65537, stopTheViolence = 256;

    Sleep(proc.tDelay);
    ProcessMgr.WriteMemory<int>(proc.moduleBase + offset.forceAttack, shoot);

    Sleep(1);
    ProcessMgr.WriteMemory<int>(proc.moduleBase + offset.forceAttack, stopTheViolence);

    std::string currWeapon = getCurrweapon();
}

bool checkTBot()
{
    DWORD crosshair;
    ProcessMgr.ReadMemory<DWORD>(proc.localPlayer + offset.crosshairID, crosshair);

    if (crosshair != -1)
    {
        DWORD64 ListEntry = 0;
        ListEntry = ProcessMgr.TraceAddress(proc.entityList, { 0x8 * (crosshair >> 9) + 0x10, 0x0 });

        if (ListEntry == 0)
        {
            return false;
        }

        DWORD64 ent;
        ProcessMgr.ReadMemory<DWORD64>(proc.listEntry + 0x78 * (crosshair & 0x1FF), ent);
        int enTeam, enHealth;

        ProcessMgr.ReadMemory<int>(ent + offset.team, enTeam);
        ProcessMgr.ReadMemory<int>(ent + offset.health, enHealth);
        if (enTeam != proc.localTeam && enHealth > 0)
        {
            std::string currWeapon = getCurrweapon();

            if (currWeapon != "ERROR")
            {
                setTBDelay(getDistance(ent), currWeapon);

                if (currWeapon == "awp" || currWeapon == "ssg08")
                {
                    bool isScoped;
                    ProcessMgr.ReadMemory<bool>(proc.localPlayer + offset.isScoped, isScoped);
                    return isScoped;
                }

                return true;
            }
        }
    }

    return false;
}

void handleTBot()
{
    if (checkTBot())
    {
        shoot();
    }
}

int main()
{
    proc.processID = ProcessMgr.GetProcessID("cs2.exe");
    //proc.moduleBase = GetProcessModuleHandle(proc.processID, "client.dll");
    proc.moduleBase = reinterpret_cast<DWORD64>(ProcessMgr.GetProcessModuleHandle(proc.processID, "client.dll"));

    ProcessMgr.ProcessID = proc.processID;
    ProcessMgr.ModuleAddress = proc.moduleBase;
    ProcessMgr.hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD, TRUE, proc.processID);

    do
    {
        ProcessMgr.ReadMemory<DWORD64>(proc.moduleBase + offset.locPlayerController, proc.localPlayer);
    } while (proc.localPlayer == NULL);

    ProcessMgr.ReadMemory<DWORD64>(proc.moduleBase + offset.entList, proc.entityList);

    ProcessMgr.ReadMemory<DWORD64>(proc.localPlayer + offset.playerPawn, proc.localPawn);

    ProcessMgr.ReadMemory<DWORD64>(proc.entityList + 0x8 * ((proc.localPawn & 0x7FFF) >> 9) + 16, proc.listEntry);

    ProcessMgr.ReadMemory<DWORD64>(proc.listEntry + 120 * (proc.localPawn & 0x1FF), proc.localPlayer);


    double sleepTime = 1;

    bool bBhop = false, bAntiFlash = false, bTriggerBot = false;
    writeCheatstoConsole(bBhop, bAntiFlash, bTriggerBot);


    while (true)
    {
        ProcessMgr.ReadMemory<int>(proc.localPlayer + offset.team, proc.localTeam);

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bBhop = !bBhop;

            writeCheatstoConsole(bBhop, bAntiFlash, bTriggerBot);
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAntiFlash = !bAntiFlash;

            writeCheatstoConsole(bBhop, bAntiFlash, bTriggerBot);
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bTriggerBot = !bTriggerBot;

            writeCheatstoConsole(bBhop, bAntiFlash, bTriggerBot);
        }

        if (bBhop)
        {
            ProcessMgr.ReadMemory<BYTE>(proc.localPlayer + offset.flags, proc.flag);
            proc.jump = proc.moduleBase + offset.fJump;
            if (GetAsyncKeyState(VK_SPACE))
            {
                DWORD val1 = 256, val2 = 65537;
                if (proc.flag & (1 << 0))
                {
                    ProcessMgr.WriteMemory<DWORD>(proc.jump, val2);
                    Sleep(2);
                    ProcessMgr.WriteMemory<DWORD>(proc.jump, val1);
                    Sleep(2);
                }
            }
        }

        if (bAntiFlash)
        {
            float flashDur = 0, flD;
            //ProcessMgr.ReadMemory<float>(proc.localPlayer + offset.flashDur, flD);

            ProcessMgr.WriteMemory<float>(proc.localPlayer + offset.flashDur, flashDur);

            //std::cout << flD << std::endl;
        }

        if (bTriggerBot)
        {
            handleTBot();
        }
    }

    return 0;
}

