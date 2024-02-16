
HWND WINAPI HookedCreateWindowEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    HWND m_hWnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    // If there is an error saved we need to show it now
    if (!g_dll.m_error.empty()) {
        Tibia::FatalError(g_dll.m_error.c_str());
    }

    // Main window should be hooked
    if (g_dll.m_screenWidth == 0 || g_dll.m_screenHeight == 0) {
        g_dll.m_screenWidth = nWidth;
        g_dll.m_screenHeight = nHeight;

        g_dll.m_hWnd = m_hWnd;

        g_dll.m_wndProc = (WNDPROC)GetWindowLongPtr(m_hWnd, GWL_WNDPROC);
        SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR)HookedMessageDispatcher);
    }

    return m_hWnd;
}

int APIENTRY HookedConnect(SOCKET s, const struct sockaddr* name, int namelen) {
    // Dispatch the event
    g_dll.onConnectEvent(name);

    // Return the real function
    return connect(s, name, namelen);
}

int APIENTRY HookedSend(SOCKET s, char* buff, int len, int flags) {
    CAM* cam = g_dll.getCAM();

    if (!Tibia::IsOnline()) {
        switch (buff[NetworkMessage::header_length]) {
        case 0x0A: { // Enter game packet
            *(uint16_t*)(buff + NetworkMessage::header_length + 3) = PROTOCOL_VERSION;
            g_dll.m_online = true;
            g_dll.m_buffer = NULL;
            break;
        }

        case 0x01: { // Login packet
            *(uint16_t*)(buff + NetworkMessage::header_length + 3) = PROTOCOL_VERSION;
            g_dll.m_online = false;
            g_dll.m_buffer = NULL;
            break;
        }

        default: break;
        }
    }

    if (!cam->isPlayerEnabled()) {
        if (Tibia::IsOnline()) {
            if (g_dll.m_buffer == NULL) {
                g_dll.m_buffer = buff;
                g_dll.m_validBuffer = true;
            }

            // The buffer does not match so it's a cheat
            if (!(buff == g_dll.m_buffer)) {
                g_dll.m_validBuffer = false;
            }
        }

        if (g_dll.m_socket != s) {
            g_dll.m_socket = s;
        }
    }

    return send(s, buff, len, flags);
}

int APIENTRY HookedRecv(SOCKET s, char* buff, int len, int flags) {
    int ret = recv(s, buff, len, flags);

    CAM* cam = g_dll.getCAM();
    if (!cam->isPlayerEnabled()) {
        if (Tibia::IsOnline()) {
            if (ret == 0) { // Connection in game has been closed - dispatch an event
                g_dll.onLogoutEvent();
            }
        }

        if (ret > 0) {
            if (g_dll.m_online) {
                if (cam->isRecorderEnabled()) {
                    /* Connection related variables */
                    static char connectionBuffer[NETWORKMESSAGE_MAXSIZE << 2];
                    static int bufferLength = 0;
                    static int bufferPosition = 0;
                    static uint16_t& packetLength = *(uint16_t*)connectionBuffer;

                    /* Lets copy Tibia buffer to ours */
                    memcpy(&connectionBuffer[bufferLength], buff, ret);
                    bufferLength = bufferLength + ret;

                    /* Lets make sure we have at least one packet */
                    if (bufferLength >= NetworkMessage::header_length && bufferLength >= (packetLength + NetworkMessage::header_length)) {
                        for (int bufferLocation = bufferPosition; bufferLocation < bufferLength;) {
                            uint16_t& packetLength = *(uint16_t*)&connectionBuffer[bufferLocation];

                            /* There might be some packet which wasn't yet fully acquired */
                            if ((packetLength + NetworkMessage::header_length) > (bufferLength - bufferLocation)) {
                                bufferPosition = bufferLocation;
                                return ret;
                            }

                            NetworkMessage message((uint8_t*)&connectionBuffer[bufferLocation]);
                            if (!XTEA_decrypt(message, (uint32_t*)XTEA_KEY_ADDRESS)) {
                                Tibia::FatalError("CAM Recorder: Failed to decrypt XTEA!");
                            }

                            /* Create new NetworkMessage class for the recorder */
                            NetworkMessage* msg = new NetworkMessage((uint8_t*)(message.getBuffer() + NetworkMessage::header_length));

                            Recorder* recorder = cam->getRecorder();
                            recorder->addPacket(msg);

                            bufferLocation = bufferLocation + packetLength + NetworkMessage::header_length;
                        }

                        bufferLength = 0;
                        bufferPosition = 0;
                    }
                }
            }
        }
    }

    return ret;
}

void Render(int nSurface) {
    // Local variables for any use
    static char messageBuffer[1024];

    if (g_dll.m_gameMenu) {
        static int height = 70;

        static int x = 60;
        static int y = 12 + 30;

        Painter::drawSkin(nSurface, x + 3, y, 108, height, 60, 0, 0); //background for buttons

        Painter::drawSkin(nSurface, x, y, 4, height, 56, 0, 0); // left line
        Painter::drawSkin(nSurface, x + 112 - 1, y, 6, height + 1, 57, 0, 0); // right line
        Painter::drawSkin(nSurface, x + 5, y - 5, 108, 5, 58, 0, 0); // top line  
        Painter::drawSkin(nSurface, x + 3, y + height, 108, 6, 59, 0, 0); // botom line

        Painter::drawSkin(nSurface, x, y + height, 5, 6, 54, 0, 0); // bottom/left corner
        Painter::drawSkin(nSurface, x + 2 + 112 - 3, y + height, 6, 6, 55, 0, 0); // bottom/right corner
        Painter::drawSkin(nSurface, x, y - 5, 5, 5, 52, 0, 0); // upper/left corner
        Painter::drawSkin(nSurface, x + 112 - 1, y - 5, 6, 5, 53, 0, 0); // upper/right corner

        Painter::drawText(nSurface, x + 116 / 2, y, 2, 255, 255, 255, (char*)"CAM options:", 1);

        Buttons* buttons = g_dll.getButtons();
        buttons->draw(nSurface);
    }

    if (Tibia::IsOnline()) {
        CAM* cam = g_dll.getCAM();
        Player* player = cam->getPlayer();
        Pinger* pinger = g_dll.getPinger();

        if (pinger && pinger->running()) {
            int ping = pinger->getPing();

            std::string m_host = GLOBAL_HOST;

            // Perhaps the host is not global one?
            if (iequals(g_dll.m_pingHost, CAM_PLAYER_HOST)) {
                m_host = std::string("Local Host");
            }

            std::string m_status;
            uint8_t red, green, blue;

            getPingInfo(ping, m_status, red, green, blue);

            sprintf(messageBuffer, "Ping to %s: ", m_host.c_str());
            Painter::drawText(nSurface, 4, 18, FONT_NORMAL_OUTLINED, 199, 199, 199, messageBuffer, 0);

            int textWidth = Tibia::TextMetric(FONT_NORMAL_OUTLINED, messageBuffer, strlen(messageBuffer));

            sprintf(messageBuffer, "%d ms - %s", ping, m_status.c_str());
            Painter::drawText(nSurface, 4 + textWidth, 18, FONT_NORMAL_OUTLINED, red, green, blue, messageBuffer, 0);
        }

        if (cam->isPlayerEnabled()) { // CAM player is enabled
            if (iequals(g_dll.m_pingHost, CAM_PLAYER_HOST)) { // Lets make sure we are connected to local host since CAM player can be enabled in game
                Tibia::GUI* gui = Tibia::GetGUIPointer();
                Tibia::GUIHolder* bar = gui->m_resize;

                sprintf(messageBuffer, "CAM Player speed: %.04lf", player->getSpeed());
                Painter::drawText(nSurface, bar->m_offsetx + 4, bar->m_offsety - 16, FONT_NORMAL_OUTLINED, 80, 255, 0, messageBuffer, 0);

                sprintf(messageBuffer, "Time: %s of %s", MilisecondsToStr(player->getNextPacket()).c_str(), MilisecondsToStr(player->getTimeTotal()).c_str());
                Painter::drawText(nSurface, bar->m_offsetx + 4, bar->m_offsety - 30, FONT_NORMAL_OUTLINED, 95, 247, 247, messageBuffer, 0);

                sprintf(messageBuffer, "Playing file: %s", player->getFileName().c_str());
                Painter::drawText(nSurface, bar->m_offsetx + bar->m_width - 4, bar->m_offsety - 16, FONT_NORMAL_OUTLINED, 80, 255, 0, messageBuffer, 2);
            }
        }
        else if (cam->isRecorderEnabled()) {
            Tibia::GUI* gui = Tibia::GetGUIPointer();
            Tibia::GUIHolder* bar = gui->m_resize;

            uint64_t time = Timer::tickCount() % 900;
            if (time <= 300) {
                sprintf(messageBuffer, "[*] Recording.");
            }
            else if (time <= 600) {
                sprintf(messageBuffer, "[*] Recording..");
            }
            else {
                sprintf(messageBuffer, "[*] Recording...");
            }

            Painter::drawText(nSurface, bar->m_offsetx + 4, bar->m_offsety - 16, FONT_NORMAL_OUTLINED, 255, 45, 40, messageBuffer, 0);
        }
    }

    g_gui->draw(nSurface);
}

void HookedTimer() {
    // Dispatch idle message to Tibia
    Tibia::Idle();

    // First fame initialization
    if (!g_dll.m_firstFrame) {
        g_dll.FirstTick();
    }

    // Tick whole GUI (each tick per 100-120 ms)
    g_gui->tick(Tibia::GetPlayerInfo(PLAYER_INFO_TICKS));

    // Tick auto trade, the dialog is in the background
    if (g_gui->getDialog() != g_dll.m_tradeDialog) {
        g_dll.m_tradeDialog->tick(Tibia::GetPlayerInfo(PLAYER_INFO_TICKS));
    }
}

void HookedDrawCreatureName(int nSurface, int nX, int nY, int nFont, int nRed, int nGreen, int nBlue, char* lpText, int nAlign) {
    SAVE_STACK;
    SAVE_REGISTERS;

    // Buffer for various usage
    static char messageBuffer[1024];

    Creature_t* creature = (Creature_t*)(lpText - 4);
    if (g_dll.m_manaBar && creature->id == Tibia::GetPlayerInfo(PLAYER_INFO_ID)) {
        // Print creature name with vertical displacement
        Painter::drawText(nSurface, nX, nY - 5, nFont, nRed, nGreen, nBlue, lpText, nAlign);

        // Calculate percentage of health and mana
        uint32_t healthPercent;
        uint32_t manaPercent;

        if (healthPercent = Tibia::GetPlayerInfo(PLAYER_INFO_MAX_HEALTH)) {
            healthPercent = std::min((uint32_t)100, 1 + 100 * Tibia::GetPlayerInfo(PLAYER_INFO_HEALTH) / healthPercent);
        }
        else {
            healthPercent = 100;
        }

        if (manaPercent = Tibia::GetPlayerInfo(PLAYER_INFO_MAX_MANA)) {
            manaPercent = std::min((uint32_t)100, 1 + 100 * Tibia::GetPlayerInfo(PLAYER_INFO_MANA) / manaPercent);
        }
        else {
            manaPercent = 100;
        }

        Color_t healthColor = getHealthPercentColor(healthPercent);
        Color_t manaColor(0, 87, 238);

        // Choose mana color
        if (manaPercent >= 80) {
            manaColor.green = 102;
            manaColor.blue = 253;
        }
        else if (manaPercent >= 60) {
            manaColor.green = 97;
            manaColor.blue = 248;
        }
        else if (manaPercent >= 40) {
            manaColor.green = 92;
            manaColor.blue = 243;
        }

        // Calculate the displacement
        int xOffset = getTotalMetric(nFont, lpText);

        // Adjust colors according to lighting
        if (healthColor.green != nGreen || healthColor.blue != nBlue) {
            manaColor.green = std::max(0, manaColor.green - (healthColor.green - nGreen));
            manaColor.blue = std::max(0, manaColor.blue - (healthColor.blue - nBlue));
        }

        // Draw new health bar
        Painter::drawRectangle(nSurface, nX + (xOffset >> 1) - 14, nY + 7, 27, 4, 0, 0, 0);
        Painter::drawRectangle(nSurface, nX + (xOffset >> 1) - 14 + 1, nY + 7 + 1, 25 * healthPercent / 100, 2, nRed, nGreen, nBlue);

        // Mana bar over old health bar
        Painter::drawRectangle(nSurface, nX + (xOffset >> 1) - 14, nY + 12, 27, 4, 0, 0, 0);
        Painter::drawRectangle(nSurface, nX + (xOffset >> 1) - 14 + 1, nY + 12 + 1, 25 * manaPercent / 100, 2, manaColor.red, manaColor.green, manaColor.blue);
    }
    else {
        Painter::drawText(nSurface, nX, nY, nFont, nRed, nGreen, nBlue, lpText, nAlign);
    }

    LOAD_REGISTERS;
    LOAD_STACK;
}
