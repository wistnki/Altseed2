﻿from . import CppBindingGenerator as cbg

from .common import *

ButtonState = cbg.Enum('altseed', 'ButtonState')
with ButtonState as enum:
    enum.add('Free', 0)
    enum.add('Push', 1)
    enum.add('Hold', 3)
    enum.add('Release', 2)

Keys = cbg.Enum('altseed', 'Keys')
with Keys as enum:
    enum.add('Unknown')
    enum.add('Space')
    enum.add('Apostrophe')
    enum.add('Comma')
    enum.add('Minus')
    enum.add('Period')
    enum.add('Slash')
    enum.add('Num0')
    enum.add('Num1')
    enum.add('Num2')
    enum.add('Num3')
    enum.add('Num4')
    enum.add('Num5')
    enum.add('Num6')
    enum.add('Num7')
    enum.add('Num8')
    enum.add('Num9')
    enum.add('Semicolon')
    enum.add('Equal')
    enum.add('A')
    enum.add('B')
    enum.add('C')
    enum.add('D')
    enum.add('E')
    enum.add('F')
    enum.add('G')
    enum.add('H')
    enum.add('I')
    enum.add('J')
    enum.add('K')
    enum.add('L')
    enum.add('M')
    enum.add('N')
    enum.add('O')
    enum.add('P')
    enum.add('Q')
    enum.add('R')
    enum.add('S')
    enum.add('T')
    enum.add('U')
    enum.add('V')
    enum.add('W')
    enum.add('X')
    enum.add('Y')
    enum.add('Z')
    enum.add('LeftBracket')
    enum.add('Backslash')
    enum.add('RightBracket')
    enum.add('GraveAccent')
    enum.add('World1')
    enum.add('World2')
    enum.add('Escape')
    enum.add('Enter')
    enum.add('Tab')
    enum.add('Backspace')
    enum.add('Insert')
    enum.add('Delete')
    enum.add('Right')
    enum.add('Left')
    enum.add('Down')
    enum.add('Up')
    enum.add('PageUp')
    enum.add('PageDown')
    enum.add('Home')
    enum.add('End')
    enum.add('CapsLock')
    enum.add('ScrollLock')
    enum.add('NumLock')
    enum.add('PrintScreen')
    enum.add('Pause')
    enum.add('F1')
    enum.add('F2')
    enum.add('F3')
    enum.add('F4')
    enum.add('F5')
    enum.add('F6')
    enum.add('F7')
    enum.add('F8')
    enum.add('F9')
    enum.add('F10')
    enum.add('F11')
    enum.add('F12')
    enum.add('F13')
    enum.add('F14')
    enum.add('F15')
    enum.add('F16')
    enum.add('F17')
    enum.add('F18')
    enum.add('F19')
    enum.add('F20')
    enum.add('F21')
    enum.add('F22')
    enum.add('F23')
    enum.add('F24')
    enum.add('F25')
    enum.add('Keypad0')
    enum.add('Keypad1')
    enum.add('Keypad2')
    enum.add('Keypad3')
    enum.add('Keypad4')
    enum.add('Keypad5')
    enum.add('Keypad6')
    enum.add('Keypad7')
    enum.add('Keypad8')
    enum.add('Keypad9')
    enum.add('KeypadDecimal')
    enum.add('KeypadDivide')
    enum.add('KeypadMultiply')
    enum.add('KeypadSubstract')
    enum.add('KeypadAdd')
    enum.add('KeypadEnter')
    enum.add('KeypadEqual')
    enum.add('LeftShift')
    enum.add('LeftControl')
    enum.add('LeftAlt')
    enum.add('LeftWin')
    enum.add('RightShift')
    enum.add('RightControl')
    enum.add('RightAlt')
    enum.add('RightWin')
    enum.add('Menu')
    enum.add('Last')
    enum.add('MAX')

Window = cbg.Class('altseed', 'Window')

Keyboard = cbg.Class('altseed', 'Keyboard')
with Keyboard as class_:
    with class_.add_func('Initialize') as func:
        func.add_arg(Window, 'window')
        func.return_type = bool
    class_.add_func('Terminate')
    with class_.add_func('GetInstance') as func:
        func.return_type = Keyboard
    class_.add_func('RefleshKeyStates')
    with class_.add_func('GetKeyState') as func:
        func.add_arg(Keys, 'key')
        func.return_type = ButtonState

MouseButtons = cbg.Enum('altseed', 'MouseButtons')
with MouseButtons as enum:
    enum.add('ButtonLeft', 0)
    enum.add('ButtonRight', 1)
    enum.add('ButtonMiddle', 2)
    enum.add('SubButton1', 3)
    enum.add('SubButton2', 4)
    enum.add('SubButton3', 5)
    enum.add('SubButton4', 6)
    enum.add('SubButton5', 7)

CursorMode = cbg.Enum('altseed', 'CursorMode')
with CursorMode as enum:
    enum.add('Normal', 0x00034001)
    enum.add('Hidden', 0x00034002)
    enum.add('Disable', 0x00034003)

Mouse = cbg.Class('altseed', 'Mouse')
with Mouse as class_:
    with class_.add_func('Initialize') as func:
        func.add_arg(Window, 'window')
        func.return_type = bool
    class_.add_func('Terminate')
    with class_.add_func('GetInstance') as func:
        func.return_type = Mouse
    class_.add_func('RefreshInputState')
    with class_.add_func('SetPosition') as func:
        func.add_arg(float, 'x')
        func.add_arg(float, 'y')
    with class_.add_func('GetPosition') as func:    # 参照引数を使った出力はサポートされない
        func.add_arg(float, 'x')
        func.add_arg(float, 'y')
    with class_.add_func('SetWheelCallback') as func:
        func.add_arg(VoidPtr, 'func')   # 関数ポインタの引数はサポートされない
    with class_.add_func('GetWheel') as func:
        func.return_type = float
    with class_.add_func('GetMouseButtonState') as func:
        func.add_arg(MouseButtons, 'button')
        func.return_type = ButtonState
    class_.add_property(CursorMode, 'CursorMode')