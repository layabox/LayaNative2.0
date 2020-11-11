
interface GamepadButton {
    pressed: boolean;
    value: number;
}

class Gamepad{
    axes: number[];
    buttons: GamepadButton[];
    connected: boolean;
    id: string;
    index: number;
    mapping: string;
    timestamp: number;    
}

class GamepadEvent extends Event {
    gamepad: Gamepad;
}

