# Invoker

[![CMake CI](https://github.com/Miezhiko/Invoker/actions/workflows/cmake.yml/badge.svg)](https://github.com/Miezhiko/Invoker/actions/workflows/cmake.yml)

spell switching by nupad keys

 - tray icon with menu
 - ctrl+break to exit
 - scroll lock to turn on/off
 - scroll lock indicates whether it's active

```c
  Key keys[10] = { { VK_NUMPAD7, { 0x10, 0x10, 0x10, 0x13 } }
                 , { VK_NUMPAD4, { 0x10, 0x10, 0x11, 0x13 } }
                 , { VK_NUMPAD1, { 0x10, 0x10, 0x12, 0x13 } }
                 , { VK_NUMPAD8, { 0x11, 0x11, 0x11, 0x13 } }
                 , { VK_NUMPAD5, { 0x11, 0x11, 0x10, 0x13 } }
                 , { VK_NUMPAD2, { 0x11, 0x11, 0x12, 0x13 } }
                 , { VK_NUMPAD9, { 0x12, 0x12, 0x12, 0x13 } }
                 , { VK_NUMPAD6, { 0x12, 0x12, 0x10, 0x13 } }
                 , { VK_NUMPAD3, { 0x12, 0x12, 0x11, 0x13 } }
                 , { VK_NUMPAD0, { 0x10, 0x11, 0x12, 0x13 } }
                 };
```
