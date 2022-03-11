// InfinityLastInput.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

// System Libraries
#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <string>
#include <winstring.h>
#include <ctime>
#define _WIN32_WINNT 0x0400
#pragma comment(lib, "user32.lib")
#include <stdio.h>
#include <wchar.h>

// User Libraries
#include "KeyInfo.h"
#include "ArrayList.h"

// Initialize KeyInfoList
ArrayList<KeyInfo*> keyInfoList = ArrayList<KeyInfo*>(
                                                                                                     // Name Numeric value Description
    new KeyInfo(VK_BACK                 , "Backspace"                 ),                             // 0008 - VK_BACK 0x08 Backspace
    new KeyInfo(VK_CANCEL               , "Break"                     ),                             // VK_CANCEL 0x03 Break
    new KeyInfo(VK_CLEAR                , "Clear"                     ),                             // VK_CLEAR 0x0C Clear
    new KeyInfo(VK_CRSEL                , "Cr Sel"                    ),                             // VK_CRSEL 0xF7 Cr Sel
    new KeyInfo(VK_DECIMAL              , "Numpad ."                  ),                             // VK_DECIMAL 0x6E Numpad .
    new KeyInfo(VK_DIVIDE               , "Numpad /"                  ),                             // VK_DIVIDE 0x6F Numpad /
    new KeyInfo(VK_EREOF                , "Er Eof"                    ),                             // VK_EREOF 0xF9 Er Eof
    new KeyInfo(VK_ESCAPE               , "Esc"                       ),                             // VK_ESCAPE 0x1B Esc
    new KeyInfo(VK_EXECUTE              , "Execute"                   ),                             // VK_EXECUTE 0x2B Execute
    new KeyInfo(VK_EXSEL                , "Sel"                       ),                             // VK_EXSEL 0xF8 Ex Sel
    new KeyInfo(VK_ICO_CLEAR            , "IcoClr"                    ),                             // VK_ICO_CLEAR 0xE6 IcoClr
    new KeyInfo(VK_ICO_HELP             , "IcoHlp"                    ),                             // VK_ICO_HELP 0xE3 IcoHlp
    new KeyInfo(48                      , "0"                         ),                             // VK_KEY_0 0x30 ('0') 0
    new KeyInfo(49                      , "1"                         ),                             // VK_KEY_1 0x31 ('1') 1
    new KeyInfo(50                      , "2"                         ),                             // VK_KEY_2 0x32 ('2') 2
    new KeyInfo(51                      , "3"                         ),                             // VK_KEY_3 0x33 ('3') 3
    new KeyInfo(52                      , "4"                         ),                             // VK_KEY_4 0x34 ('4') 4
    new KeyInfo(53                      , "5"                         ),                             // VK_KEY_5 0x35 ('5') 5
    new KeyInfo(54                      , "6"                         ),                             // VK_KEY_6 0x36 ('6') 6
    new KeyInfo(55                      , "7"                         ),                             // VK_KEY_7 0x37 ('7') 7
    new KeyInfo(56                      , "8"                         ),                             // VK_KEY_8 0x38 ('8') 8
    new KeyInfo(57                      , "9"                         ),                             // VK_KEY_9 0x39 ('9') 9
    new KeyInfo(65                      , "A"                         ),                             // VK_KEY_A 0x41 ('A') A
    new KeyInfo(66                      , "B"                         ),                             // VK_KEY_B 0x42 ('B') B
    new KeyInfo(67                      , "C"                         ),                             // VK_KEY_C 0x43 ('C') C
    new KeyInfo(68                      , "D"                         ),                             // VK_KEY_D 0x44 ('D') D
    new KeyInfo(69                      , "E"                         ),                             // VK_KEY_E 0x45 ('E') E
    new KeyInfo(70                      , "F"                         ),                             // VK_KEY_F 0x46 ('F') F
    new KeyInfo(71                      , "G"                         ),                             // VK_KEY_G 0x47 ('G') G
    new KeyInfo(72                      , "H"                         ),                             // VK_KEY_H 0x48 ('H') H
    new KeyInfo(73                      , "I"                         ),                             // VK_KEY_I 0x49 ('I') I
    new KeyInfo(74                      , "J"                         ),                             // VK_KEY_J 0x4A ('J') J
    new KeyInfo(75                      , "K"                         ),                             // VK_KEY_K 0x4B ('K') K
    new KeyInfo(76                      , "L"                         ),                             // VK_KEY_L 0x4C ('L') L
    new KeyInfo(77                      , "M"                         ),                             // VK_KEY_M 0x4D ('M') M
    new KeyInfo(78                      , "N"                         ),                             // VK_KEY_N 0x4E ('N') N
    new KeyInfo(79                      , "O"                         ),                             // VK_KEY_O 0x4F ('O') O
    new KeyInfo(80                      , "P"                         ),                             // VK_KEY_P 0x50 ('P') P
    new KeyInfo(81                      , "Q"                         ),                             // VK_KEY_Q 0x51 ('Q') Q
    new KeyInfo(82                      , "R"                         ),                             // VK_KEY_R 0x52 ('R') R
    new KeyInfo(83                      , "S"                         ),                             // VK_KEY_S 0x53 ('S') S
    new KeyInfo(84                      , "T"                         ),                             // VK_KEY_T 0x54 ('T') T
    new KeyInfo(85                      , "U"                         ),                             // VK_KEY_U 0x55 ('U') U
    new KeyInfo(86                      , "V"                         ),                             // VK_KEY_V 0x56 ('V') V
    new KeyInfo(87                      , "W"                         ),                             // VK_KEY_W 0x57 ('W') W
    new KeyInfo(88                      , "X"                         ),                             // VK_KEY_X 0x58 ('X') X
    new KeyInfo(89                      , "Y"                         ),                             // VK_KEY_Y 0x59 ('Y') Y
    new KeyInfo(90                      , "Z"                         ),                             // VK_KEY_Z 0x5A ('Z') Z
    new KeyInfo(VK_MULTIPLY             , "Numpad *"                  ),                             // VK_MULTIPLY 0x6A Numpad *
    new KeyInfo(VK_NONAME               , "NoName"                    ),                             // VK_NONAME 0xFC NoName
    new KeyInfo(VK_NUMPAD0              , "0"                         ),                             // VK_NUMPAD0 0x60 Numpad 0
    new KeyInfo(VK_NUMPAD1              , "1"                         ),                             // VK_NUMPAD1 0x61 Numpad 1
    new KeyInfo(VK_NUMPAD2              , "2"                         ),                             // VK_NUMPAD2 0x62 Numpad 2
    new KeyInfo(VK_NUMPAD3              , "3"                         ),                             // VK_NUMPAD3 0x63 Numpad 3
    new KeyInfo(VK_NUMPAD4              , "4"                         ),                             // VK_NUMPAD4 0x64 Numpad 4
    new KeyInfo(VK_NUMPAD5              , "5"                         ),                             // VK_NUMPAD5 0x65 Numpad 5
    new KeyInfo(VK_NUMPAD6              , "6"                         ),                             // VK_NUMPAD6 0x66 Numpad 6
    new KeyInfo(VK_NUMPAD7              , "7"                         ),                             // VK_NUMPAD7 0x67 Numpad 7
    new KeyInfo(VK_NUMPAD8              , "8"                         ),                             // VK_NUMPAD8 0x68 Numpad 8
    new KeyInfo(VK_NUMPAD9              , "9"                         ),                             // VK_NUMPAD9 0x69 Numpad 9
    new KeyInfo(VK_OEM_1                , "OEM_1 (:"                  ),                             // VK_OEM_1 0xBA OEM_1 (:
    new KeyInfo(VK_OEM_102              , "OEM_102 (> <)"             ),                             // VK_OEM_102 0xE2 OEM_102 (> <)
    new KeyInfo(VK_OEM_2                , "OEM_2 (? /)"               ),                             // VK_OEM_2 0xBF OEM_2 (? /)
    new KeyInfo(VK_OEM_3                , "OEM_3 (~ `)"                ),                            // VK_OEM_3 0xC0 OEM_3 (~`)
    new KeyInfo(VK_OEM_4                , "OEM_4 ({ [)"               ),                             // VK_OEM_4 0xDB OEM_4 ({ [)
    new KeyInfo(VK_OEM_5                , "OEM_5 (| \)"               ),                             // VK_OEM_5 0xDC OEM_5 (| \)
    new KeyInfo(VK_OEM_6                , "OEM_6 (} ])"                ),                             // VK_OEM_6 0xDD OEM_6 (} ])
    new KeyInfo(VK_OEM_7                , "OEM_7 (\" ')"              ),                             // VK_OEM_7 0xDE OEM_7 (" ')
    new KeyInfo(VK_OEM_8                , "OEM_8 (§ !)"               ),                             // VK_OEM_8 0xDF OEM_8 (§ !)
    new KeyInfo(VK_OEM_ATTN             , "OEM_ATTN Attn"             ),                             // VK_OEM_ATTN 0xF0 Oem Attn
    new KeyInfo(VK_OEM_AUTO             , "OEM_AUTO Auto"             ),                             // VK_OEM_AUTO 0xF3 Auto
    new KeyInfo(VK_OEM_AX               , "OEM_AX Ax"                 ),                             // VK_OEM_AX 0xE1 Ax
    new KeyInfo(VK_OEM_BACKTAB          , "OEM_BACKTAB Back Tab"      ),                             // VK_OEM_BACKTAB 0xF5 Back Tab
    new KeyInfo(VK_OEM_CLEAR            , "OEM_CLEAR OemClr"          ),                             // VK_OEM_CLEAR 0xFE OemClr
    new KeyInfo(VK_OEM_COMMA            , "OEM_COMMA (< ,)"           ),                             // VK_OEM_COMMA 0xBC OEM_COMMA (< ,)
    new KeyInfo(VK_OEM_COPY             , "OEM_COPY Copy"             ),                             // VK_OEM_COPY 0xF2 Copy
    new KeyInfo(VK_OEM_CUSEL            , "OEM_CUSEL Cu Sel"          ),                             // VK_OEM_CUSEL 0xEF Cu Sel
    new KeyInfo(VK_OEM_ENLW             , "OEM_ENLW Enlw"             ),                             // VK_OEM_ENLW 0xF4 Enlw
    new KeyInfo(VK_OEM_FINISH           , "OEM_FINISH Finish"         ),                             // VK_OEM_FINISH 0xF1 Finish
    new KeyInfo(VK_OEM_FJ_LOYA          , "OEM_FJ_LOYA Loya"          ),                             // VK_OEM_FJ_LOYA 0x95 Loya
    new KeyInfo(VK_OEM_FJ_MASSHOU       , "OEM_FJ_MASSHOU Mashu"      ),                             // VK_OEM_FJ_MASSHOU 0x93 Mashu
    new KeyInfo(VK_OEM_FJ_ROYA          , "OEM_FJ_ROYA Roya"          ),                             // VK_OEM_FJ_ROYA 0x96 Roya
    new KeyInfo(VK_OEM_FJ_TOUROKU       , "OEM_FJ_TOUROKU Touroku"    ),                             // VK_OEM_FJ_TOUROKU 0x94 Touroku
    new KeyInfo(VK_OEM_JUMP             , "OEM_JUMP Jump"             ),                             // VK_OEM_JUMP 0xEA Jump
    new KeyInfo(VK_OEM_MINUS            , "OEM_MINUS (_ -)"           ),                             // VK_OEM_MINUS 0xBD OEM_MINUS (_ -)
    new KeyInfo(VK_OEM_PA1              , "OEM_PA1 OemPa1"            ),                             // VK_OEM_PA1 0xEB OemPa1
    new KeyInfo(VK_OEM_PA2              , "OEM_PA2 OemPa2"            ),                             // VK_OEM_PA2 0xEC OemPa2
    new KeyInfo(VK_OEM_PA3              , "OEM_PA3 OemPa3"            ),                             // VK_OEM_PA3 0xED OemPa3
    new KeyInfo(VK_OEM_PERIOD           , "OEM_PERIOD (> .)"          ),                             // VK_OEM_PERIOD 0xBE OEM_PERIOD(> .)
    new KeyInfo(VK_OEM_PLUS             , "OEM_PLUS (+=)"             ),                             // VK_OEM_PLUS 0xBB OEM_PLUS (+=)
    new KeyInfo(VK_OEM_RESET            , "OEM_RESET Reset"           ),                             // VK_OEM_RESET 0xE9 Reset
    new KeyInfo(VK_OEM_WSCTRL           , "OEM_WSCTRL WsCtrl"         ),                             // VK_OEM_WSCTRL 0xEE WsCtrl
    new KeyInfo(VK_PA1                  , "Pa1"                       ),                             // VK_PA1 0xFD Pa1
    new KeyInfo(VK_PACKET               , "Packet"                    ),                             // VK_PACKET 0xE7 Packet
    new KeyInfo(VK_PLAY                 , "Play"                      ),                             // VK_PLAY 0xFA Play
    new KeyInfo(VK_PROCESSKEY           , "Process"                   ),                             // VK_PROCESSKEY 0xE5 Process
    new KeyInfo(VK_RETURN               , "Enter"                     ),                             // VK_RETURN 0x0D Enter
    new KeyInfo(VK_SELECT               , "Select"                    ),                             // VK_SELECT 0x29 Select
    new KeyInfo(VK_SEPARATOR            , "Separator"                 ),                             // VK_SEPARATOR 0x6C Separator
    new KeyInfo(VK_SPACE                , "Space"                     ),                             // VK_SPACE 0x20 Space
    new KeyInfo(VK_SUBTRACT             , "Num -"                     ),                             // VK_SUBTRACT 0x6D Num -
    new KeyInfo(VK_TAB                  , "Tab"                       ),                             // VK_TAB 0x09 Tab
    new KeyInfo(VK_ZOOM                 , "Zoom"                      ),                             // VK_ZOOM 0xFB Zoom
    new KeyInfo(255                     , "no VK mapping"             ),                             // VK__none_ 0xFF no VK mapping
    new KeyInfo(VK_ACCEPT               , "Accept"                    ),                             // VK_ACCEPT 0x1E Accept
    new KeyInfo(VK_APPS                 , "Context Menu"              ),                             // VK_APPS 0x5D Context Menu
    new KeyInfo(VK_BROWSER_BACK         , "Browser Back"              ),                             // VK_BROWSER_BACK 0xA6 Browser Back
    new KeyInfo(VK_BROWSER_FAVORITES    , "Browser Favorites"         ),                             // VK_BROWSER_FAVORITES 0xAB Browser Favorites
    new KeyInfo(VK_BROWSER_FORWARD      , "Browser Forward"           ),                             // VK_BROWSER_FORWARD 0xA7 Browser Forward
    new KeyInfo(VK_BROWSER_HOME         , "Browser Home"              ),                             // VK_BROWSER_HOME 0xAC Browser Home
    new KeyInfo(VK_BROWSER_REFRESH      , "Browser Refresh"           ),                             // VK_BROWSER_REFRESH 0xA8 Browser Refresh
    new KeyInfo(VK_BROWSER_SEARCH       , "Browser Search"            ),                             // VK_BROWSER_SEARCH 0xAA Browser Search
    new KeyInfo(VK_BROWSER_STOP         , "Browser Stop"              ),                             // VK_BROWSER_STOP 0xA9 Browser Stop
    new KeyInfo(VK_CAPITAL              , "Caps Lock"                 ),                             // VK_CAPITAL 0x14 Caps Lock
    new KeyInfo(VK_CONVERT              , "Convert"                   ),                             // VK_CONVERT 0x1C Convert
    new KeyInfo(VK_DELETE               , "Delete"                    ),                             // VK_DELETE 0x2E Delete
    new KeyInfo(VK_DOWN                 , "Arrow Down"                ),                             // VK_DOWN 0x28 Arrow Down
    new KeyInfo(VK_END                  , "End"                       ),                             // VK_END 0x23 End
    new KeyInfo(VK_F1                   , "F1"                        ),                             // VK_F1 0x70 F1
    new KeyInfo(VK_F10                  , "F10"                       ),                             // VK_F10 0x79 F10
    new KeyInfo(VK_F11                  , "F11"                       ),                             // VK_F11 0x7A F11
    new KeyInfo(VK_F12                  , "F12"                       ),                             // VK_F12 0x7B F12
    new KeyInfo(VK_F13                  , "F13"                       ),                             // VK_F13 0x7C F13
    new KeyInfo(VK_F14                  , "F14"                       ),                             // VK_F14 0x7D F14
    new KeyInfo(VK_F15                  , "F15"                       ),                             // VK_F15 0x7E F15
    new KeyInfo(VK_F16                  , "F16"                       ),                             // VK_F16 0x7F F16
    new KeyInfo(VK_F17                  , "F17"                       ),                             // VK_F17 0x80 F17
    new KeyInfo(VK_F18                  , "F18"                       ),                             // VK_F18 0x81 F18
    new KeyInfo(VK_F19                  , "F19"                       ),                             // VK_F19 0x82 F19
    new KeyInfo(VK_F2                   , "F2"                        ),                             // VK_F2 0x71 F2
    new KeyInfo(VK_F20                  , "F20"                       ),                             // VK_F20 0x83 F20
    new KeyInfo(VK_F21                  , "F21"                       ),                             // VK_F21 0x84 F21
    new KeyInfo(VK_F22                  , "F22"                       ),                             // VK_F22 0x85 F22
    new KeyInfo(VK_F23                  , "F23"                       ),                             // VK_F23 0x86 F23
    new KeyInfo(VK_F24                  , "F24"                       ),                             // VK_F24 0x87 F24
    new KeyInfo(VK_F3                   , "F3"                        ),                             // VK_F3 0x72 F3
    new KeyInfo(VK_F4                   , "F4"                        ),                             // VK_F4 0x73 F4
    new KeyInfo(VK_F5                   , "F5"                        ),                             // VK_F5 0x74 F5
    new KeyInfo(VK_F6                   , "F6"                        ),                             // VK_F6 0x75 F6
    new KeyInfo(VK_F7                   , "F7"                        ),                             // VK_F7 0x76 F7
    new KeyInfo(VK_F8                   , "F8"                        ),                             // VK_F8 0x77 F8
    new KeyInfo(VK_F9                   , "F9"                        ),                             // VK_F9 0x78 F9
    new KeyInfo(VK_FINAL                , "Final"                     ),                             // VK_FINAL 0x18 Final
    new KeyInfo(VK_HELP                 , "Help"                      ),                             // VK_HELP 0x2F Help
    new KeyInfo(VK_HOME                 , "Home"                      ),                             // VK_HOME 0x24 Home
    new KeyInfo(VK_ICO_00               , "Ico00 *"                   ),                             // VK_ICO_00 0xE4 Ico00 *
    new KeyInfo(VK_INSERT               , "Insert"                    ),                             // VK_INSERT 0x2D Insert
    new KeyInfo(VK_JUNJA                , "Junja"                     ),                             // VK_JUNJA 0x17 Junja
    new KeyInfo(VK_KANA                 , "Kanas"                     ),                             // VK_KANA 0x15 Kana
    new KeyInfo(VK_KANJI                , "Kanji"                     ),                             // VK_KANJI 0x19 Kanji
    new KeyInfo(VK_LAUNCH_APP1          , "App1"                      ),                             // VK_LAUNCH_APP1 0xB6 App1
    new KeyInfo(VK_LAUNCH_APP2          , "App2"                      ),                             // VK_LAUNCH_APP2 0xB7 App2
    new KeyInfo(VK_LAUNCH_MAIL          , "Mail"                      ),                             // VK_LAUNCH_MAIL 0xB4 Mail
    new KeyInfo(VK_LAUNCH_MEDIA_SELECT  , "Media"                     ),                             // VK_LAUNCH_MEDIA_SELECT 0xB5 Media
    new KeyInfo(VK_LBUTTON              , "Left Button * *"           ),                             // VK_LBUTTON 0x01 Left Button * *
    new KeyInfo(VK_LCONTROL             , "Left Ctrl"                 ),                             // VK_LCONTROL 0xA2 Left Ctrl
    new KeyInfo(VK_LEFT                 , "Arrow Left"                ),                             // VK_LEFT 0x25 Arrow Left
    new KeyInfo(VK_LMENU                , "Left Alt"                  ),                             // VK_LMENU 0xA4 Left Alt
    new KeyInfo(VK_LSHIFT               , "Left Shift"                ),                             // VK_LSHIFT 0xA0 Left Shift
    new KeyInfo(VK_LWIN                 , "Left Win"                  ),                             // VK_LWIN 0x5B Left Win
    new KeyInfo(VK_MBUTTON              , "Middle Button * *"         ),                             // VK_MBUTTON 0x04 Middle Button * *
    new KeyInfo(VK_MEDIA_NEXT_TRACK     , "Next Track"                ),                             // VK_MEDIA_NEXT_TRACK 0xB0 Next Track
    new KeyInfo(VK_MEDIA_PLAY_PAUSE     , "Play / Pause"              ),                             // VK_MEDIA_PLAY_PAUSE 0xB3 Play / Pause
    new KeyInfo(VK_MEDIA_PREV_TRACK     , "Previous Track"            ),                             // VK_MEDIA_PREV_TRACK 0xB1 Previous Track
    new KeyInfo(VK_MEDIA_STOP           , "Stop"                      ),                             // VK_MEDIA_STOP 0xB2 Stop
    new KeyInfo(VK_MODECHANGE           , "Mode Change"               ),                             // VK_MODECHANGE 0x1F Mode Change
    new KeyInfo(VK_NEXT                 , "Page Down"                 ),                             // VK_NEXT 0x22 Page Down
    new KeyInfo(VK_NONCONVERT           , "Non Convert"               ),                             // VK_NONCONVERT 0x1D Non Convert
    new KeyInfo(VK_NUMLOCK              , "Num Lock"                  ),                             // VK_NUMLOCK 0x90 Num Lock
    new KeyInfo(VK_OEM_FJ_JISHO         , "Jisho"                     ),                             // VK_OEM_FJ_JISHO 0x92 Jisho
    new KeyInfo(VK_PAUSE                , "Pause"                     ),                             // VK_PAUSE 0x13 Pause
    new KeyInfo(VK_PRINT                , "Print"                     ),                             // VK_PRINT 0x2A Print
    new KeyInfo(VK_PRIOR                , "Page Up"                   ),                             // VK_PRIOR 0x21 Page Up
    new KeyInfo(VK_RBUTTON              , "Right Button * *"          ),                             // VK_RBUTTON 0x02 Right Button * *
    new KeyInfo(VK_RCONTROL             , "Right Ctrl"                ),                             // VK_RCONTROL 0xA3 Right Ctrl
    new KeyInfo(VK_RIGHT                , "Arrow Right"               ),                             // VK_RIGHT 0x27 Arrow Right
    new KeyInfo(VK_RMENU                , "Right Alt"                 ),                             // VK_RMENU 0xA5 Right Alt
    new KeyInfo(VK_RSHIFT               , "Right Shift"               ),                             // VK_RSHIFT 0xA1 Right Shift
    new KeyInfo(VK_RWIN                 , "Right Win"                 ),                             // VK_RWIN 0x5C Right Win
    new KeyInfo(VK_SCROLL               , "Scrol Lock"                ),                             // VK_SCROLL 0x91 Scrol Lock
    new KeyInfo(VK_SLEEP                , "Sleep"                     ),                             // VK_SLEEP 0x5F Sleep
    new KeyInfo(VK_SNAPSHOT             , "Print Screen"              ),                             // VK_SNAPSHOT 0x2C Print Screen
    new KeyInfo(VK_UP                   , "Arrow Up"                  ),                             // VK_UP 0x26 Arrow Up
    new KeyInfo(VK_VOLUME_DOWN          , "Volume Down"               ),                             // VK_VOLUME_DOWN 0xAE Volume Down
    new KeyInfo(VK_VOLUME_MUTE          , "Volume Mute"               ),                             // VK_VOLUME_MUTE 0xAD Volume Mute
    new KeyInfo(VK_VOLUME_UP            , "Volume Up"                 ),                             // VK_VOLUME_UP 0xAF Volume Up
    new KeyInfo(VK_XBUTTON1             , "X Button 1 * *"            ),                             // VK_XBUTTON1 0x05 X Button 1 * *
    new KeyInfo(VK_XBUTTON2             , "X Button 2 * *"            ),                             // VK_XBUTTON2 0x06 X Button 2 * *
    nullptr
);

void thread1_func(void const* args) {
    while (true) {
        for (size_t i=0; i<keyInfoList.getSize(); i++) {
            if (GetAsyncKeyState(keyInfoList.get(i)->getKeyCode())) {
                std::cout << "thread1 - {"<< keyInfoList.get(i)->getKeyDescription() <<"} pressed..." << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void thread2_func(void const* args) {
    while(true) {
        // LASTINPUTINFO lastInputInfo;
        // GetLastInputInfo(&lastInputInfo);
        // std::cout << "lastInpputTime:" << lastInputInfo.dwTime << std::endl;

        // Get the last input event info.
        LASTINPUTINFO li;
        li.cbSize = sizeof(LASTINPUTINFO);
        ::GetLastInputInfo(&li);
        // Calculate the time elapsed in seconds.
        DWORD te = ::GetTickCount();
        //int elapsed = (te - li.dwTime) / 100;
        int idleTime = te - li.dwTime;
        int lastInputTime = ((idleTime > 0) ? (idleTime / 1000) : idleTime);
        int lastInputTimeMilis = ((idleTime > 0) ? (idleTime / 10) : idleTime);

        // Test against a preset timeout period in
        // seconds.
        //if (5000 < elapsed) {
            // Call a function or set an application flag
            // that can be acted on outside this function.
            //AfxMessageBox(_T("This session has timeout"));
        //}
        // std::cout << "thread2 - lastInputTime:" << lastInputTime << std::endl;
        std::cout << "thread2 - lastInputTimeMilis:" << lastInputTimeMilis << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void thread3_func(POINT* mousePointer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));


    // Then send mouse left single click event that is avoiding the error
    INPUT Inputs[3] = { 0 };
    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dx = mousePointer->x; // desired X coordinate
    Inputs[0].mi.dy = mousePointer->y; // desired Y coordinate
    Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[2].type = INPUT_MOUSE;
    Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(3, Inputs, sizeof(INPUT));
}

HHOOK hKeyboardHook;
__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam) {
    DWORD SHIFT_key = 0;
    DWORD CTRL_key = 0;
    DWORD ALT_key = 0;

    if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))) {
        KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*) lParam);
        DWORD dwMsg = 1;
        dwMsg += hooked_key.scanCode << 16;
        dwMsg += hooked_key.flags << 24;
        WCHAR lpszKeyName[1024] = { 0 };
        lpszKeyName[0] = '[';

        int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;
        lpszKeyName[i] = ']';

        int key = hooked_key.vkCode;

        SHIFT_key = GetAsyncKeyState(VK_SHIFT);
        CTRL_key = GetAsyncKeyState(VK_CONTROL);
        ALT_key = GetAsyncKeyState(VK_MENU);

        for (size_t i = 0; i < keyInfoList.getSize(); i++) {
            if (keyInfoList.get(i)->getKeyCode() == key) {
                std::cout << "KeyboardEvent - " << "{lpszKeyName: " << lpszKeyName << "}" << "{keyCode: " << std::to_string(key) << "}" << "{keyName: " << keyInfoList.get(i)->getKeyDescription() << "} pressed..." << std::endl;
            }
        }

        //if (key == VK_SPACE)   //Space key detected, do stuff
        //{
        //    MessageBox(NULL, L"Space key detected", L"Keyboard hook", MB_OK);
        //    return 1;

        //}

    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

HHOOK hLowLevelMouseHook;
DWORD lastLeftMouseClickTime = ::GetTickCount();
__declspec(dllexport) LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if ((nCode == HC_ACTION) && ((wParam == WM_LBUTTONDOWN) || (wParam == WM_LBUTTONDBLCLK))) {
        MSLLHOOKSTRUCT hooked_key = *((MSLLHOOKSTRUCT*) lParam);

        // Get the time elapsed.
        DWORD te = ::GetTickCount();
        int idleTime = te - lastLeftMouseClickTime;
        int lastInputTimeMilis = idleTime;

        lastLeftMouseClickTime = te;

        if (lastInputTimeMilis < 100) {
            // Fix double click mouse bug by disabling the mouse click event
            time_t rawtime;
            struct tm timeinfo[80];
            char buffer[80];

            time(&rawtime);
            localtime_s(timeinfo, &rawtime);

            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);
            std::string strTime(buffer);

            std::cout << "[" << strTime << "]" << " - MOUSE CLICK BUG DETECTED -> LeftMouseClicked - lastMouseLeftClick: " << lastInputTimeMilis << std::endl;
            
            // std::thread thread3(thread3_func, &hooked_key.pt);
            
            return 1;
        } 

        lastLeftMouseClickTime = te;
        /*DWORD dwMsg = 1;
        dwMsg += hooked_key.scanCode << 16;
        dwMsg += hooked_key.flags << 24;
        WCHAR lpszKeyName[1024] = { 0 };
        lpszKeyName[0] = '[';

        int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;
        lpszKeyName[i] = ']';

        int key = hooked_key.vkCode;*/
    }
    return CallNextHookEx(hLowLevelMouseHook, nCode, wParam, lParam);
}


void MessageLoop() {
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

DWORD WINAPI my_HotKey(LPVOID lpParm) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!hInstance) hInstance = LoadLibrary((LPCWSTR)lpParm);
    if (!hInstance) return 1;

    hKeyboardHook      = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
    hLowLevelMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)LowLevelMouseProc, hInstance, NULL);
    MessageLoop();
    UnhookWindowsHookEx(hKeyboardHook);
    UnhookWindowsHookEx(hLowLevelMouseHook);
    return 0;
}

int main(int argc, char** argv) {
    //int arg = 0;

    //// std::thread thread1(thread1_func, &arg);
    //// std::thread thread2(thread2_func, &arg);

    //HKL hKL = GetKeyboardLayout(GetCurrentThreadId());
    //int ihKL = (int)((unsigned int)hKL & 0x0000FFFF);

    //WORD pLang = PRIMARYLANGID(hKL);
    //WORD sLang = SUBLANGID(hKL);

    ////LANG_ENGLISH
    //LPWSTR langName = new TCHAR[85];
    //PWSTR ctryName;
    //GetLocaleInfo(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT), LOCALE_SNAME, langName, 85);

    //std::wstring wsLangName(langName);
    //std::string strLangName = std::string(wsLangName.begin(), wsLangName.end());

    //std::cout << "primaryLangId:" << strLangName << std::endl;

    //while(true) {
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //}

    HANDLE hThread;
    DWORD dwThread;

    hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_HotKey, (LPVOID)argv[0], NULL, &dwThread);

    if (hThread) return WaitForSingleObject(hThread, INFINITE);
    else return 1;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
