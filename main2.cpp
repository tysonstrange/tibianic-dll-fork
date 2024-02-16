
void LIB::InitializeMemory() {
    static int m_chunkLength = 28;

    /*char* */m_chunk = new char[m_chunkLength];
    ZeroMemory(m_chunk, m_chunkLength);

    AsmDword(0x44B978 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_MANA);
    AsmDword(0x44BA20 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_MANA);
    AsmDword(0x450A84 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_MANA);

    AsmDword(0x44B938 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_HEALTH);
    AsmDword(0x44B9D0 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_HEALTH);
    AsmDword(0x450A66 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_HEALTH);
    AsmDword(0x450DB8 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_HEALTH);

    AsmDword(0x44B924 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44B9C0 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44BBF2 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44BC12 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44BC32 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44BCD7 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44D6D0 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44D6F0 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44D720 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x44D88F + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x450A60 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x450DC5 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x45114B + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x4516FE + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x4518CE + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x4519B6 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x451A59 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x451C0D + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);
    AsmDword(0x451ECD + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_ID);

    AsmDword(0x4B6494 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_U);
    AsmDword(0x4B6516 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_U);
    AsmDword(0x4BDF92 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_U);
    AsmDword(0x4BE9E1 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_U);
    AsmDword(0x4BF6A6 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_U);

    AsmDword(0x4B08B7 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4B08DB + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4B08F0 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4B0910 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4B0951 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4B0960 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4B09A1 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4BB1CF + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4BC2DA + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4BE1CC + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4BEBAE + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4BEC2F + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);
    AsmDword(0x4BF6A0 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Z);

    AsmDword(0x4B08B1 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4B08D0 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4B0941 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4B0988 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4BC300 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4BD584 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4BD5A6 + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4BEBFB + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);
    AsmDword(0x4BF69A + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_Y);

    AsmDword(0x4B08AC + 1, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4B08C0 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4B0930 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4B096D + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4BC2EE + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4BD57E + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4BD5A0 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4BEBEF + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
    AsmDword(0x4BF694 + 2, (DWORD)m_chunk + CHUNK_OFFSET_PLAYER_X);
}

void LIB::InitHostLogin(std::string host, uint16_t port) {
    strcpy((char*)MemLoginServer1, host.c_str());
    strcpy((char*)MemLoginServer2, host.c_str());
    strcpy((char*)MemLoginServer3, host.c_str());
    strcpy((char*)MemLoginServer4, host.c_str());

    if (iequals(host, GLOBAL_HOST)) {
        strcpy((char*)MemTestLoginServer, GLOBAL_HOST_PROXY);
    }
    else {
        strcpy((char*)MemTestLoginServer, host.c_str());
    }

    *(uint16_t*)MemPortLoginServer1 = port;
    *(uint16_t*)MemPortLoginServer2 = port;
    *(uint16_t*)MemPortLoginServer3 = port;
    *(uint16_t*)MemPortLoginServer4 = port;
    *(uint16_t*)MemPortLoginServer5 = port;
}

void LIB::InitLocalHostLogin() {
    InitHostLogin(CAM_PLAYER_HOST, CAM_PLAYER_PORT);
}

void LIB::InitGlobalHostLogin() {
    /* Tibia.exe 7.72 port addr = CA1FB */
    //InitHostLogin(GLOBAL_HOST, GLOBAL_PORT);

    strcpy((char*)MemLoginServer1, GLOBAL_LOGIN1);
    strcpy((char*)MemLoginServer2, GLOBAL_LOGIN2);
    strcpy((char*)MemLoginServer3, GLOBAL_LOGIN3);
    strcpy((char*)MemLoginServer4, GLOBAL_LOGIN4);

    strcpy((char*)MemTestLoginServer, GLOBAL_HOST_PROXY);

    *(uint16_t*)MemPortLoginServer1 = GLOBAL_PORT;
    *(uint16_t*)MemPortLoginServer2 = GLOBAL_PORT;
    *(uint16_t*)MemPortLoginServer3 = GLOBAL_PORT;
    *(uint16_t*)MemPortLoginServer4 = GLOBAL_PORT;
    *(uint16_t*)MemPortLoginServer5 = GLOBAL_PORT;
}

void LIB::RequestSharedExperience() {
    g_dll.m_sharedExperience = !g_dll.m_sharedExperience;

    g_dll.m_networkMessage = new NetworkMessage(NetworkMessage::XTEA);
    g_dll.m_networkMessage->AddByte(0xA8);
    g_dll.m_networkMessage->AddByte(g_dll.m_sharedExperience);
    g_dll.SendXTEANetworkMessage(g_dll.m_networkMessage);
}

void LIB::RequestOutfitDialog() {
    g_dll.m_networkMessage = new NetworkMessage(NetworkMessage::XTEA);
    g_dll.m_networkMessage->AddByte(0xD2);
    g_dll.SendXTEANetworkMessage(g_dll.m_networkMessage);
}

void LIB::RequestQuestLine(uint16_t questId) {
    g_dll.m_networkMessage = new NetworkMessage(NetworkMessage::XTEA);
    g_dll.m_networkMessage->AddByte(0xF1);
    g_dll.m_networkMessage->AddU16(questId);
    g_dll.SendXTEANetworkMessage(g_dll.m_networkMessage);
}

void LIB::SendXTEANetworkMessage(NetworkMessage* message) {
    message->writeMessageLength();
    XTEA_encrypt(*message, (uint32_t*)XTEA_KEY_ADDRESS);

    char* m_messageBuffer = message->getBuffer();

    int transmitted = 0;
    int bytestosend = *(uint16_t*)m_messageBuffer + NetworkMessage::header_length;

    while (transmitted != bytestosend) {
        int ret = send(g_dll.m_socket, m_messageBuffer + transmitted, bytestosend - transmitted, 0);
        if (ret == -1) {
            break;
        }

        transmitted = transmitted + ret;
    }
    delete message;
}

void LIB::SendRSANetworkMessage(NetworkMessage* message) {

}

void LIB::SetExperienceText(bool Display, uint32_t Experience /*= 0*/) {
    if (!Display) {
        if (m_displayExperience) {
            SetWindowText(m_hWnd, (WINDOW_TITLE));
        }
    }
    else {
        uint32_t experiencePerMinute = (float)(Experience - m_beginExperience) / (float)((GetTickCount() - m_beginExpTime) / 1000.0f / 60.0f);

        std::stringstream m_title;
        m_title << WINDOW_TITLE << " - You are now gaining " << (experiencePerMinute * 60) << " experience points per hour";


        SetWindowText(m_hWnd, ((char*)m_title.str().c_str()));
    }
}

DWORD LIB::HookAsmCall(DWORD dwAddress, DWORD dwFunction, DWORD NOPs /* = 0 */) {
    DWORD dwOldProtect, dwNewProtect, dwOldCall, dwNewCall;
    HANDLE proc = GetCurrentProcess();

    VirtualProtectEx(proc, (LPVOID)(dwAddress), 5 + NOPs, PAGE_READWRITE, &dwOldProtect);

    dwNewCall = dwFunction - dwAddress - 5;
    dwOldCall = *(uint32_t*)(dwAddress + 1);

    *(uint8_t*)(dwAddress) = 0xE8;
    *(uint32_t*)(dwAddress + 1) = dwNewCall;
    // 0xE8 AABBCCDD - asm CALL func

    for (int i = 0; i < NOPs; i++) {
        *(uint8_t*)(dwAddress + 5 + i) = 0x90; // NOP
    }

    VirtualProtectEx(proc, (LPVOID)(dwAddress), 5 + NOPs, dwOldProtect, &dwNewProtect);

    return dwOldCall;
}

void LIB::AsmNop(DWORD dwAddress, DWORD dwNops) {
    DWORD dwOldProtect, dwNewProtect;
    HANDLE proc = GetCurrentProcess();

    VirtualProtectEx(proc, (LPVOID)(dwAddress), dwNops, PAGE_READWRITE, &dwOldProtect);

    for (int i = 0; i < dwNops; i++) {
        *(uint8_t*)(dwAddress + i) = 0x90; // NOP
    }

    VirtualProtectEx(proc, (LPVOID)(dwAddress), dwNops, dwOldProtect, &dwNewProtect);
}

void LIB::AsmDword(DWORD dwAddress, DWORD dwFunction) {
    DWORD dwOldProtect, dwNewProtect, dwOldCall, dwNewCall;
    HANDLE proc = GetCurrentProcess();
    VirtualProtectEx(proc, (LPVOID)(dwAddress), 4, PAGE_READWRITE, &dwOldProtect);
    dwNewCall = dwFunction;
    dwOldCall = *(uint32_t*)(dwAddress);
    *(uint32_t*)(dwAddress) = dwNewCall;
    VirtualProtectEx(proc, (LPVOID)(dwAddress), 4, dwOldProtect, &dwNewProtect);
    //return dwOldCall;
}

void LIB::InitializeHook() {
    DWORD dwOldProtect, dwNewProtect, funcAddress, origAddress;

    funcAddress = (DWORD)&HookedConnect;
    origAddress = (DWORD)((int*)IAT_CONNECT_FUNCTION_ADDRESS); //ws2_32 connect in Tibia.exe IAT
    VirtualProtect((LPVOID)origAddress, 4, PAGE_READWRITE, &dwOldProtect);
    memcpy((LPVOID)origAddress, &funcAddress, 4);
    VirtualProtect((LPVOID)origAddress, 4, dwOldProtect, &dwNewProtect);

    funcAddress = (DWORD)&HookedSend;
    origAddress = (DWORD)((int*)IAT_SEND_FUNCTION_ADDRESS); //ws2_32 send in Tibia.exe IAT
    VirtualProtect((LPVOID)origAddress, 4, PAGE_READWRITE, &dwOldProtect);
    memcpy((LPVOID)origAddress, &funcAddress, 4);
    VirtualProtect((LPVOID)origAddress, 4, dwOldProtect, &dwNewProtect);

    funcAddress = (DWORD)&HookedRecv;
    origAddress = (DWORD)((int*)IAT_RECV_FUNCTION_ADDRESS); //ws2_32 recv in Tibia.exe IAT
    VirtualProtect((LPVOID)origAddress, 4, PAGE_READWRITE, &dwOldProtect);
    memcpy((LPVOID)origAddress, &funcAddress, 4);
    VirtualProtect((LPVOID)origAddress, 4, dwOldProtect, &dwNewProtect);

    funcAddress = (DWORD)&HookedCreateWindowEx;
    origAddress = (DWORD)((int*)IAT_CREATEWINDOWEX_FUNCTION_ADDRESS); //USER32.CreateWindowExA
    VirtualProtect((LPVOID)origAddress, 4, PAGE_READWRITE, &dwOldProtect);
    memcpy((LPVOID)origAddress, &funcAddress, 4);
    VirtualProtect((LPVOID)origAddress, 4, dwOldProtect, &dwNewProtect);

    // fix the wrong func address:
    HookAsmCall(DRAW_SKIN_MENU_GAME_FUNCTION_ADDRESS, DRAW_SKIN_FUNCTION_ADDRESS);

    HookAsmCall(PING_CALL_ADDRESS, (DWORD)&HookedPing);

    HookAsmCall(PRINT_FPS_FUNCTION_ADDRESS, (DWORD)&HookedPrintFps);
    // 004494F9   . 83C4 24        ADD ESP,24
    // The asm will pop the stack from function arguments

    HookAsmCall(PRINT_CAPACITY_FUNCTION_ADDRESS, (DWORD)&HookedPrintCapacity);
    // 00428F2D  |. 83C4 24        ADD ESP,24
    // The asm will pop the stack from function arguments

    // Both functions have to be checked for stack validation!
    HookAsmCall(MINIMISE_INVENTORY_REPLACEMENT_ADDRESS, (DWORD)&HookedMinInventory);
    HookAsmCall(MAXIMIZE_INVENTORY_REPLACEMENT_ADDRESS, (DWORD)&HookedMaxInventory);

    // clear the code
    AsmNop(GUI_CLICK_EVENT_CODECAVE_CALL_ADDRESS, 6);

    // new code - check for stack validation!
    HookAsmCall(GUI_CLICK_EVENT_CODECAVE_CALL_ADDRESS, (DWORD)&HookedOnPushEvent);

    // new code - check for stack validation!
    HookAsmCall(0x004C8C6C, (DWORD)&HookedDrawCreatureName);

    // new code - check for stack validation!
    HookAsmCall(0x004A9B30, (DWORD)&HookedTimer);

    //HookAsmCall(0x42B8F5, (DWORD)&HookedMenuButton);
    HookAsmCall(SET_EXPERIENCE_CALL_ADDRESS, (DWORD)&HookedSetExperience);
    HookAsmCall(PUSH_LETTER_CALL_ADDRESS, (DWORD)&HookedPushLetter);

    //00449458   . 0F84 9E000000  JE Tibria.004494FC
    // printfps jump
    AsmNop(PRINT_FPS_JUMP_ADDRESS, 6);
}

void LIB::DeinitializeHook() {

}

bool LIB::CreateDACL(SECURITY_ATTRIBUTES* pSA) {
    const TCHAR* szSD = TEXT("D:")             // Discretionary ACL
        TEXT("(D;OICI;GA;;;BG)")    // Deny all access to built-in guests
        TEXT("(D;OICI;GA;;;AN)")    // Deny all access to anonymous logon
        TEXT("(D;OICI;GA;;;AU)")    // Deny all access to authenticated users
        TEXT("(D;OICI;GA;;;BA)");   // Deny full control to administrators
    //  return true;
    return ConvertStringSecurityDescriptorToSecurityDescriptor(szSD, 1, (PSECURITY_DESCRIPTOR*)&(pSA->lpSecurityDescriptor), NULL);
}

bool LIB::InitSecurityAttr() {
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;

    CreateDACL(&sa);
    if (!SetKernelObjectSecurity(m_currentProcess, DACL_SECURITY_INFORMATION, (PSECURITY_DESCRIPTOR)sa.lpSecurityDescriptor)) {
        m_error = std::string("Initializing security attributes failed!");
    }
}

extern "C" BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        g_dll.Initialize();
        break;
    }

    case DLL_PROCESS_DETACH: {
        g_dll.Deinitialize();
        break;
    }

    case DLL_THREAD_ATTACH: {
        break;
    }

    case DLL_THREAD_DETACH: {
        break;
    }

    default: break;
    }

    return TRUE;
}
