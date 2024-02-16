#include "main.h"
#include "Sddl.h"

/* It is important for the library to be created on DS segment */
LIB g_dll;

// Global objects
RSA* g_rsa;
GUI* g_gui;

// Last caller of the event function
uint32_t g_lastCaller;

// Stack related variables
uint32_t g_lastEsp;
uint32_t g_lastEbp;

// Registers related variables
uint32_t g_lastTmp;
uint32_t g_lastEax;
uint32_t g_lastEbx;
uint32_t g_lastEcx;
uint32_t g_lastEdx;
uint32_t g_lastEsi;
uint32_t g_lastEdi;

// Event related variables
int g_lastEventId;
int g_lastPacketId;

// Input related variables
POINT g_lastMousePosition;
bool g_lastAlt;
bool g_lastControl;

/* Asembler functions */
extern "C" void _cdecl pushRegisters();
asm("_pushRegisters:     \n\
  .intel_syntax noprefix \n\
  pop _g_lastTmp         \n\
  push eax               \n\
  push ebx               \n\
  push ecx               \n\
  push edx               \n\
  push edi               \n\
  push esi               \n\
  push ebp               \n\
  push esp               \n\
  push _g_lastTmp        \n\
  ret                    \n\
  .att_syntax;           \n\
");

extern "C" void _cdecl popRegisters();
asm("_popRegisters:      \n\
  .intel_syntax noprefix \n\
  pop _g_lastTmp         \n\
  pop esp                \n\
  pop ebp                \n\
  pop esi                \n\
  pop edi                \n\
  pop edx                \n\
  pop ecx                \n\
  pop ebx                \n\
  pop eax                \n\
  push _g_lastTmp        \n\
  ret                    \n\
  .att_syntax;           \n\
");

/* Global functions */

LRESULT CALLBACK HookedMessageDispatcher(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  ScopedCriticalSection section(7);
  
  // Mouse common variables
  g_lastMousePosition.x = GET_X_LPARAM(lParam);
  g_lastMousePosition.y = GET_Y_LPARAM(lParam);
  
  // Keyboard common variables
  g_lastAlt = GetKeyState(VK_MENU) & KEY_DOWN;
  g_lastControl = GetKeyState(VK_CONTROL) & KEY_DOWN;
  
  switch(uMsg){
    case WM_MOUSEWHEEL: {
      if(!ScreenToClient(hWnd, &g_lastMousePosition)){
        break;
      }
      
      if(!g_gui->onMouseEvent(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? EVENT_SCROLL_UP : EVENT_SCROLL_DOWN, g_lastMousePosition.x, g_lastMousePosition.y, wParam & MK_LBUTTON, wParam & MK_RBUTTON)){
        if(!g_gui->hasDialog()){
          return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
        }
      }
        
      break;
    }
  
    case WM_MOUSEMOVE: {
      if(!g_gui->onMouseEvent(EVENT_MOVEMENT, g_lastMousePosition.x, g_lastMousePosition.y, wParam & MK_LBUTTON, wParam & MK_RBUTTON)){
        if(!g_gui->hasDialog()){
          return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
        }
      }
      
      break;
    }
      
    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONDOWN: {
       // check whether the cursor is in the same position as reported mouse position
      g_dll.checkCursorPos();
      
      if(!g_gui->onMouseEvent(EVENT_BUTTON, g_lastMousePosition.x, g_lastMousePosition.y, true, wParam & MK_RBUTTON)){
        if(!g_gui->hasDialog()){
          return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
        }
      }
        
      break;
    }
      
    case WM_LBUTTONUP: {
       // check whether the cursor is in the same position as reported mouse position
      g_dll.checkCursorPos();
      
      if(!g_gui->onMouseEvent(EVENT_BUTTON, g_lastMousePosition.x, g_lastMousePosition.y, false, wParam & MK_RBUTTON)){
        if(!g_gui->hasDialog()){
          return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
        }
      }
      
      break;
    }
    
    case WM_KEYUP: {
      return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
    }
      
    case WM_KEYDOWN: {
      switch(wParam){
        case VK_F8: {
          if(g_lastAlt){ // Enable fps
            g_dll.m_optionsDialog->setNfps(!g_dll.m_optionsDialog->getNfps());
            
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_F9: {
          if(g_lastAlt){ // Enable main menu
            g_dll.m_optionsDialog->setInfo(!g_dll.m_optionsDialog->getInfo());    
            
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_A: { // A key
          if(g_lastControl){ // Select all
            if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_SELECTALL, 0)){
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
          }
          
          break;
        }
        
        case VK_X: { // X key
          if(g_lastControl){ // Cut
            if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_CUT, 0)){
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
          }
          
          break;
        }
        
        case VK_C: { // C key
          if(g_lastControl){ // Copy
            if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_COPY, 0)){
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
          }
          
          break;
        }
        
        case VK_V: { // V key
          if(g_lastControl){ // Paste
            if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_PASTE, 0)){
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
          }
          
          break;
        }
        
        case VK_LEFT: { // Arrow key left
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_LEFT, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_RIGHT: { // Arrow key right
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_RIGHT, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_UP: { // Arrow key up
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_UP, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_DOWN: { // Arrow key down
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_DOWN, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_TAB: { // Tab key
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_TAB, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_RETURN: { // Enter key
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_ENTER, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_ESCAPE: { // Escape key
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_ESCAPE, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
          
          break;
        }
        
        case VK_BACK: { // Backspace key
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_BACK, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
      
          break;
        }
        
        case VK_DELETE: { // Delete key
          if(g_gui->onKeyboardEvent(EVENT_KEY_DOWN, KEYBOARD_STATE_DELETE, 0)){
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
      
          break;
        }
        
        default: break;
      }

      if(g_gui->hasDialog()){
        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);
        WORD code;
        
        if(ToAscii(wParam, 0, keyboardState, &code, 0) == 1){
          if(code >= 32){
            if(g_dll.onAsciiCharacter(code)){
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
          }
        }
        
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
      }
      
      if(Tibia::IsOnline()){
        if(g_dll.m_escToggle){
          if(wParam == VK_ESCAPE){
            g_dll.m_optionsDialog->setWsad(!g_dll.m_wsad);
            
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
          }
        }
          
        CAM* cam = g_dll.getCAM();
        Player* player = cam->getPlayer();
          
        if(cam->isPlayerEnabled()){
          switch(wParam){
            case VK_LEFT: {
              player->decreaseSpeed();
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
              
            case VK_RIGHT: {
              player->increaseSpeed();
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
              
            case VK_DOWN: {
              player->setSpeed(1.0);
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
              
            case VK_UP: {
              player->setSpeed(256.0);
              return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
              
            default: break;
          }
        }
          
        if(g_dll.m_wsad){
          if(!(GetKeyState(VK_CONTROL) & KEY_DOWN) && !(GetKeyState(VK_SHIFT) & KEY_DOWN)){
            switch(wParam){
              case LETTER_W: {
                wParam = VK_UP;
                break;
              }
                
              case LETTER_A: {
                wParam = VK_LEFT;
                break;
              }
                
              case LETTER_S: {
                wParam = VK_DOWN;
                break;
              }
                
              case LETTER_D: {
                wParam = VK_RIGHT;
                break;
              }
                
              case LETTER_Q: {
                wParam = VK_HOME;
                break;
              }
                
              case LETTER_E: {
                wParam = VK_PRIOR;
                break;
              }
                
              case LETTER_Z: {
                wParam = VK_END;
                break;
              }
                
              case LETTER_C: {
                wParam = VK_NEXT;
                break;
              }
                
              default: break;
            }
          }
        }
      }

      return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
    }
      
    case WM_SIZE: {
      if(wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED){
        g_dll.m_screenWidth  = LOWORD(lParam);
        g_dll.m_screenHeight = HIWORD(lParam);
        
        // All GUI windows will be reinitialized and their position will be adjusted to the center
        g_gui->onResolution();
      }
      
      return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
    }
      
    case WM_DESTROY: {
      // Take care of windows (release them all)
      g_gui->releaseWindows();
        
      // Take care of the pinger
      if(g_dll.pinger){
        delete g_dll.pinger;
      }
        
      // Take care of dialogs
      delete g_dll.m_newsDialog;
      delete g_dll.m_updateDialog;
      delete g_dll.m_optionsDialog;
      delete g_dll.m_questsDialog;
      delete g_dll.m_questDialog;
      delete g_dll.m_outfitDialog;
    }
      
    default: {
      return CallWindowProc(g_dll.m_wndProc, hWnd, uMsg, wParam, lParam);
    }
  }
  
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

