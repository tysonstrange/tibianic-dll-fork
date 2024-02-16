
extern "C" void _cdecl contextEvent(uint32_t eventId) {
    switch (eventId) {
    case 0x1000: {
        return g_dll.RequestSharedExperience();
    }

    case 0x1001: {
        return g_dll.RequestOutfitDialog();
    }
    }
}

extern "C" void HookedOnPushEvent();
asm("_HookedOnPushEvent:      \n\
  .intel_syntax noprefix      \n\
  CMP ECX, 0x1000              \n\
  JL default                  \n\
  CMP ECX, 0x1500              \n\
  JG  default                  \n\
  CALL _pushRegisters          \n\
  PUSH ECX                    \n\
  CALL _contextEvent          \n\
  ADD ESP, 4                  \n\
  CALL _popRegisters          \n\
  MOV EAX, 0                  \n\
  JMP return                  \n\
  default:                    \n\
  PUSH ECX                    \n\
  MOV ECX, EAX                \n\
  CALL DWORD PTR DS:[EDX+0x70]\n\
  return:                      \n\
  RET                          \n\
  .att_syntax;                \n\
");

/* Called only when equipment window is minimized */

extern "C" void _stdcall MinimizeInventory() {
    // Inventory is not visible
    g_dll.m_inventory = false;

    // Disable inventory buttons
    g_dll.m_inventoryButtons->disable();

    // Enable arrow slot
    g_dll.m_arrowSlot->activate();
}

extern "C" void HookedMinInventory();
asm("_HookedMinInventory:      \n\
  .intel_syntax noprefix      \n\
  POP _g_lastTmp              \n\
  CALL _MinimizeInventory@0    \n\
  MOV EAX, 0x0042B4D0          \n\
  CALL EAX                    \n\
  PUSH _g_lastTmp              \n\
  RET                          \n\
  .att_syntax;                \n\
");

/* Called only when equipment window is maximized */
extern "C" void _stdcall MaximizeInventory() {
    // Inventory is visible
    g_dll.m_inventory = true;

    // Enable inventory buttons  
    g_dll.m_inventoryButtons->enable();

    // Disable arrow slot
    g_dll.m_arrowSlot->deactivate();
}

extern "C" void HookedMaxInventory();
asm("_HookedMaxInventory:      \n\
  .intel_syntax noprefix      \n\
  POP _g_lastTmp              \n\
  CALL _MaximizeInventory@0    \n\
  MOV EAX, 0x0049BEE0          \n\
  CALL EAX                    \n\
  PUSH _g_lastTmp              \n\
  RET                          \n\
  .att_syntax;                \n\
");

void HookedPrintCapacity(int nSurface, int nX, int nY, int nFont, int nRed, int nGreen, int nBlue, int nAlign, char* lpText) {
    SAVE_STACK;
    SAVE_REGISTERS;

    // Normal function
    Painter::drawText(nSurface, nX, nY, nFont, nRed, nGreen, nBlue, lpText, nAlign);

    if (g_dll.m_inventory) {
        Buttons* inventoryButtons = g_dll.m_inventoryButtons;

        inventoryButtons->setPosition(nX + 25, nY);
        inventoryButtons->draw(nSurface);
    }
    else {
        ArrowSlot* slot = g_dll.m_arrowSlot;

        slot->setPosition(nX + 89, nY - 7);
        slot->draw(nSurface);
    }

    LOAD_STACK;
    LOAD_REGISTERS;
}

void HookedPrintFps(int nSurface, int nX, int nY, int nFont, int nRed, int nGreen, int nBlue, char* lpText, int nAlign) {
    SAVE_STACK;
    SAVE_REGISTERS;

    if (g_dll.m_gameFps) {
        Painter::drawText(nSurface, nX, nY, nFont, nRed, nGreen, nBlue, lpText, nAlign);
    }

    Render(nSurface);

    LOAD_STACK;
    LOAD_REGISTERS;
}

/* We have some variables on the stack so we need the function to clean it up */
/*_stdcall */void HookedPing() {
    // Update experience text
    g_dll.SetExperienceText(g_dll.m_displayExperience, *(uint32_t*)PLAYER_EXP_ADDRESS);

    // Prepare ping network message
    g_dll.m_networkMessage = new NetworkMessage(NetworkMessage::XTEA);
    g_dll.m_networkMessage->AddByte(0x1E);

    // Send information about validations
    if (!g_dll.m_validHost) {
        g_dll.m_networkMessage->AddU32(1);
    }
    else if (!g_dll.m_validBuffer) {
        g_dll.m_networkMessage->AddU32(2);
    }
    else if (!g_dll.m_validCursor) {
        g_dll.m_networkMessage->AddU32(3);
    }
    else {
        g_dll.m_networkMessage->AddU32(4);
    }

    g_dll.SendXTEANetworkMessage(g_dll.m_networkMessage);
}

/* In this case we are creating a variable on the stack so it needs to be cleaned later on */
/*_cdecl */void _stdcall HookedSetExperience(int Experience, int Level, int U3) {
    Tibia::SetExperience(Experience, Level, U3);

    if (!g_dll.m_firstExperience) {
        if (g_dll.m_optionsDialog) {
            g_dll.m_optionsDialog->setExph(g_dll.m_displayExperience);
        }

        // We have recieved first experience
        g_dll.m_firstExperience = true;
    }
    else {
        g_dll.SetExperienceText(g_dll.m_displayExperience, Experience);
    }
}

/*_cdecl */void _stdcall HookedPushLetter(int Letter) {
    if (Tibia::IsOnline()) {
        if (g_dll.m_wsad) {
            if (Letter == 'A' || Letter == 'a') {
                return;
            }

            if (Letter == 'W' || Letter == 'w') {
                return;
            }

            if (Letter == 'S' || Letter == 's') {
                return;
            }

            if (Letter == 'D' || Letter == 'd') {
                return;
            }

            if (Letter == 'Q' || Letter == 'q') {
                return;
            }

            if (Letter == 'E' || Letter == 'e') {
                return;
            }

            if (Letter == 'Z' || Letter == 'z') {
                return;
            }

            if (Letter == 'C' || Letter == 'c') {
                return;
            }
        }
    }

    // Dialog should block everything
    if (g_gui->hasDialog()) {
        return;
    }

    Tibia::PushLetter(Letter);
}

void flushCamRecording(bool logout) {
    CAM* cam = g_dll.getCAM();

    cam->disableRecorder();
    g_dll.m_gameRecordButton->setActive(false);

    Recorder* recorder = cam->getRecorder();
    std::string filename = recorder->flush();

    std::string str = std::string("Your movie has been saved as ") + filename;
    if (logout) {
        str = str + std::string("\r\nPlease check cam/ directory located in Tibria main directory.");
        ShowMessage(str);
    }
    else {
        str = str + std::string("\nPlease check cam/ directory located in Tibria main directory.");
        Tibia::GUIDialog* dialog = Tibia::CreateTextDialog("Information", str.c_str());
        Tibia::SetCurrentDialog(dialog);
    }
}

void gameRecord(Button* button, void* lParam) {
    CAM* cam = g_dll.getCAM();

    if (button->active()) { // enabled
        if (Tibia::IsOnline()) {
            // Create dialog
            Tibia::GUIDialog* dialog = Tibia::CreateTextDialog("Warning", "In order to record a movie you have to\nstart recording before logging into game.");
            Tibia::SetCurrentDialog(dialog);

            button->setActive(false);
        }
        else {
            cam->enableRecorder();
        }
    }
    else {
        if (!cam->getRecorder()->empty()) {
            flushCamRecording(false);
        }
        else { // lets disable it
            cam->disableRecorder();
        }
    }
}

void gamePlayer(Button* button, void* lParam) {
    CAM* cam = g_dll.getCAM();
    Player* player = cam->getPlayer();

    if (button->active()) {
        if (Tibia::IsOnline()) {
            Tibia::GUIDialog* dialog = Tibia::CreateTextDialog("Warning", "You can't start CAM player while being online.\nPlease logout and start the CAM player then.");
            Tibia::SetCurrentDialog(dialog);

            // Lets set the button to false
            button->setActive(false);
        }
        else {
            // Lets start the cam player
            g_dll.InitLocalHostLogin();
            if (!player->start()) {
                // Create dialog with warning message
                Tibia::GUIDialog* dialog = Tibia::CreateTextDialog("Warning", "CAM Player was unable to start.");
                Tibia::SetCurrentDialog(dialog);

                // Since it failed to start there is no point of allowing local login
                g_dll.InitGlobalHostLogin();

                // Lets set the button to false
                button->setActive(false);
            }
            else { // Everything went fine
                cam->enablePlayer();

                Tibia::SetCurrentDialog(NULL);
            }
        }
    }
    else {
        // disable
        g_dll.InitGlobalHostLogin();
        player->stop();
        cam->disablePlayer();
    }
}

void gameMenu(Button* button, void* lParam) {
    GUIOptionsWindow* options = g_dll.m_optionsDialog;

    g_gui->setDialog(options);
}

void gameNews(Button* button, void* lParam) {
    GUINewsWindow* news = g_dll.m_newsDialog;
    news->fetch();

    g_gui->setDialog(news);
}

void gameQuests(Button* button, void* lParam) {
    g_dll.m_networkMessage = new NetworkMessage(NetworkMessage::XTEA);
    g_dll.m_networkMessage->AddByte(0xF0);
    g_dll.SendXTEANetworkMessage(g_dll.m_networkMessage);
}

void gameMana(Button* button, void* lParam) {
    g_dll.m_optionsDialog->setMana(button->active());
}

void gamePing(Button* button, void* lParam) {
    g_dll.m_optionsDialog->setPing(button->active());
}

void gameAutoTrade(Button* button, void* lParam) {
    g_gui->setDialog(g_dll.m_tradeDialog);
}

void gameOptions(Button* button, void* lParam) {
    Tibia::SetCurrentDialog(Tibia::CreateOptionsDialog());
}

void gameHelp(Button* button, void* lParam) {
    static uint32_t ret = 0;
    Tibia::SetCurrentDialog(Tibia::CreateHelpDialog(&ret));
}

wchar_t* convertCharArrayToLPCWSTR(char* charArray)
{
    wchar_t* wString = new wchar_t[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, MAX_PATH);
    return wString;
}

/* Library functions */

LIB::LIB() {
    // Set variables to default values
    m_gameMenu = false;
    m_gameFps = false;
    m_online = false;
    m_inventory = false;
    m_firstFrame = false;
    m_firstExperience = false;
    m_wsad = false;
    m_escToggle = false;
    m_manaBar = false;
    m_displayExperience = false;
    m_sharedExperience = false;

    m_validHost = true;
    m_validBuffer = true;
    m_validCursor = true;

    m_beginExperience = 0;
    m_beginExpTime = 0;
    m_screenHeight = 0;
    m_screenWidth = 0;

    m_networkMessage = NULL;
    m_buffer = NULL;
    m_chunk = NULL;
    pinger = NULL;

    // The default host the pinger will ping
    m_pingHost = GLOBAL_HOST;

    // We need the main module name which is the executable name - it could be changed by the player so we need to know what to update if it's not Tibria.exe
    char pFile[MAX_PATH];
    if (!GetModuleFileName(GetModuleHandle(NULL), (pFile), MAX_PATH)) {
        m_error = std::string("Tibria client was unable to get current module!");
    }

    m_moduleName = pFile;
    m_moduleName = m_moduleName.substr(m_moduleName.find_last_of('\\') + 1);

    g_rsa = new RSA();
    g_gui = new GUI();

    // These windows are used as dialogs, they shouldn't be pushed to main container of GUIWindows class
    m_newsDialog = new GUINewsWindow();
    m_optionsDialog = new GUIOptionsWindow();
    m_questsDialog = new GUIQuestsLog();
    m_questDialog = new GUIQuestLine();
    m_tradeDialog = new GUITradeWindow();
    m_outfitDialog = new GUIOutfitDialog();

    // Lets create a CAM instance
    cam = new CAM();

    // Main GUI button container - it will take care of main buttons visible in the Tibria menu
    buttons = new Buttons(75, 58);

    // Inventory buttons
    m_inventoryButtons = new Buttons(0, 0);

    // Create new arrow slot
    m_arrowSlot = new ArrowSlot(0, 0, 97, 2148, 12);

    // Push button so they can be interacted with
    g_gui->pushElement(buttons);
    g_gui->pushElement(m_inventoryButtons);
    g_gui->pushElement(m_arrowSlot);

    // Lets create main buttons visible in the menu
    m_gameRecordButton = buttons->pushButton("CAM Record", gameRecord, NULL, 0, 0, true);
    buttons->pushButton("CAM Player", gamePlayer, NULL, 0, 30, true);

    // Lets create quests button in inventory
    m_gameQuestsButton = m_inventoryButtons->pushButton("Quests", gameQuests, NULL, 0, -56, false, true);

    // Options button
    m_inventoryButtons->pushButton("Options", gameOptions, NULL, 0, -32, false, true);

    // Help button
    m_inventoryButtons->pushButton("Help", gameHelp, NULL, 0, -8, false, true);

    // Basic, fast access buttons located in inventory window
    m_inventoryOptsButton = m_inventoryButtons->pushButton("T", gameMenu, NULL, -41 + 17 * 0, -139, false, true);
    m_inventoryOptsButton->setGraphics(24, 16, 16);

    m_inventoryNewsButton = m_inventoryButtons->pushButton("N", gameNews, NULL, -41 + 17 * 1, -139, false, true);
    m_inventoryNewsButton->setGraphics(24, 16, 16);

    m_inventoryManaButton = m_inventoryButtons->pushButton("M", gameMana, NULL, -41 + 17 * 2, -139, true, true);
    m_inventoryManaButton->setGraphics(24, 16, 16);

    m_inventoryPingButton = m_inventoryButtons->pushButton("P", gamePing, NULL, -41 + 17 * 3, -139, true, true);
    m_inventoryPingButton->setGraphics(24, 16, 16);

    m_inventoryWsadButton = m_inventoryButtons->pushButton("A", gameAutoTrade, NULL, -41 + 17 * 4, -139, false, true);
    m_inventoryWsadButton->setGraphics(24, 16, 16);

    // Disable inventory buttons
    m_inventoryButtons->disable();
}

LIB::~LIB() {
    // TODO
}

void LIB::Initialize() {
    m_currentProcess = GetCurrentProcess();
    m_currentProcessId = GetCurrentProcessId();

    const char* p("14299623962416399520070177382898895550795403345466153217470516082934737582776038882967213386204600674145392845853859217990626450972452084065728686565928113");
    const char* q("7630979195970404721891201847792002125535401292779123937207447574596692788513647179235335529307251350570728407373705564708871762033017096809910315212884101");
    const char* d("46730330223584118622160180015036832148732986808519344675210555262940258739805766860224610646919605860206328024326703361630109888417839241959507572247284807035235569619173792292786907845791904955103601652822519121908367187885509270025388641700821735345222087940578381210879116823013776808975766851829020659073");

    g_rsa->setKey(p, q, d);

    if (!crc32(std::string(GAME_DIR) + std::string(MAIN_LIBRARY))) {
        m_error = std::string("Tibria client was unable to load Tibria.dll properly!");
    }

    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        m_error = std::string("Tibria client was unable to initialize winsock!");
    }

    // Lets check if map directory is available, if not - lets create it
    if (!CreateDirectory((MAP_DIR), NULL)) {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_ALREADY_EXISTS) {
            m_error = std::string(std::string("Tibria was unable to create ") + std::string(MAP_DIR) + std::string(" directory!"));
        }
    }

    // Lets check if cam directory is available, if not - lets create it.
    if (!CreateDirectory((CAM_DIR), NULL)) {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_ALREADY_EXISTS) {
            m_error = std::string(std::string("Tibria was unable to create") + std::string(CAM_DIR) + std::string(" directory!"));
        }
    }

    InitGlobalHostLogin();
    InitSecurityAttr();
    InitializeHook();
    InitializeMemory();
}

void LIB::Deinitialize() {
    DeinitializeHook();
    WSACleanup();
}

bool LIB::onAsciiCharacter(WORD character) {
    if (g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_ASCII, character)) {
        return true;
    }

    return false;
}

void LIB::checkCursorPos() {
    static POINT prevRealPosition;
    static POINT lastRealPosition;
    static POINT prevCallPosition;
    static POINT lastCallPosition;

    POINT currentPosition;
    GetCursorPos(&currentPosition);
    if (ScreenToClient(m_hWnd, &currentPosition)) {
        prevRealPosition = lastRealPosition;
        lastRealPosition = currentPosition;
        prevCallPosition = lastCallPosition;
        lastCallPosition = g_lastMousePosition;

        static int sameCursorPosition = 0;
        if (prevRealPosition.x == lastRealPosition.x && prevRealPosition.y == lastRealPosition.y) {
            sameCursorPosition++;
        }
        else {
            sameCursorPosition = 0;
        }

        if (sameCursorPosition > 6 && (lastCallPosition.x != lastRealPosition.x && lastCallPosition.y != lastRealPosition.y) && (prevCallPosition.x != lastCallPosition.x && prevCallPosition.y != lastCallPosition.y)) {
            // we didn't move our cursor, but yet the position received by callback changes
            m_validCursor = false;
        }
    }
}

void LIB::FirstTick() {
    /**
      This function will be called only once after the first tick
    */

    // Lets create update window
    m_updateDialog = new GUIUpdateWindow();

    // Lets make it as dialog
    g_gui->setDialog(m_updateDialog);

    // Load configuration file for the advanced options
    m_optionsDialog->loadConfig();

    // Everything has been initialized, this function won't be called anymore
    m_firstFrame = true;
}

void LIB::onConnectEvent(const struct sockaddr* address) {
    sockaddr_in* addr_in = (sockaddr_in*)address;

    if (pinger) {
        m_pingHost = ipbintostr(addr_in->sin_addr.s_addr);
        pinger->setHost(g_dll.m_pingHost);
    }
    else {
        m_pingHost = ipbintostr(addr_in->sin_addr.s_addr);
    }

    // Reset the first experience
    m_firstExperience = false;
}

void LIB::onLogoutEvent() {
    m_online = false;
    m_buffer = NULL;

    /* Dialogs actions */
    m_tradeDialog->stop();

    if (cam->isRecorderEnabled()) {
        flushCamRecording(true);
    }
}
