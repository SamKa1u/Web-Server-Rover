window.addEventListener("gamepadconnected", function(e) {
    console.log("Gamepad connected:", e.gamepad.id);
    requestAnimationFrame(updateGamepad);
  });

  function updateGamepad() {
    const gamepads = navigator.getGamepads();
    if (gamepads[0]) {
      // Get joystick axes (typically 0 and 1 for left stick)
      // Values range from -1 to 1
      const x = Math.round(gamepads[0].axes[0] * 100);
      const y = Math.round(-gamepads[0].axes[1] * 100);

      // Update joystick UI
      const maxRadius = container.offsetWidth / 2 - joystick.offsetWidth / 2;
      joystick.style.left = (centerX + (x/100 * maxRadius)) + 'px';
      joystick.style.top = (centerY + (y/-100 * maxRadius)) + 'px';

      status.textContent = `X: ${x}, Y: ${y}`;

      // Send to server
      sendJoystickData(x, y);
    }
    requestAnimationFrame(updateGamepad);
  }