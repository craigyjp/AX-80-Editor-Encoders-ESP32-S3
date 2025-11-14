#include "TButton.h"

TButton::TButton(uint8_t pin)
  : TButton(pin, LOW, 500, 10, 400) {}

TButton::TButton(uint8_t pin, uint32_t activeState)
  : TButton(pin, activeState, 500, 10, 400) {}

TButton::TButton(uint8_t pin, uint32_t activeState, uint32_t holdThresh, uint32_t debounceWindow, uint32_t clickTime)
  : _holdThresh(holdThresh), _clickWindow(clickTime), _pressedState(activeState), _clicks(0), clicks(0), buttonHeld(false)
{
    pinMode(pin, INPUT_PULLUP);
    debouncer.attach(pin);
    debouncer.interval(debounceWindow);
    _currentState = !activeState;
    _windowStartTime = 0;
}

void TButton::update()
{
    debouncer.update();
    _currentState = debouncer.read();

    // Reset activation status
    clicks = 0;
    buttonHeld = false;

    if (_currentState == _pressedState) {
        if (_windowStartTime == 0) {
            _windowStartTime = millis();
            _clicks++;
        }

        if (millis() - _windowStartTime > _holdThresh) {
            buttonHeld = true;
            _clicks = 0;
        }
    } else {
        if (_windowStartTime > 0 && !buttonHeld) {
            if (millis() - _windowStartTime < _clickWindow) {
                clicks = _clicks;
            }
        }

        _windowStartTime = 0;
        _clicks = 0;
    }
}

