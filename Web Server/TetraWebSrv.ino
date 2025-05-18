#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "******";
const char* password = "*********";

// Web server on port 80
WebServer server(80);

// UART communication with Cytron
#define RX_PIN 44
#define TX_PIN 43

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());
  
  // Serve the main page
  server.on("/", handleRoot);
  
  // API endpoint for joystick data
  server.on("/joystick", HTTP_POST, handleJoystick);
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Send HTML with joystick interface
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <script>
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
</script>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Robot Joystick Controller</title>
  <style>
    #joystickContainer {
      width: 300px;
      height: 300px;
      background-color: #f0f0f0;
      border-radius: 150px;
      position: relative;
      margin: 50px auto;
    }
    #joystick {
      width: 100px;
      height: 100px;
      background-color: #007bff;
      border-radius: 50px;
      position: absolute;
      top: 100px;
      left: 100px;
      cursor: pointer;
    }
    #status {
      text-align: center;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <h1 style="text-align:center">Robot Joystick Control</h1>
  <div id="joystickContainer">
    <div id="joystick"></div>
  </div>
  <div id="status">X: 0, Y: 0</div>

  <script>
    const joystick = document.getElementById('joystick');
    const container = document.getElementById('joystickContainer');
    const status = document.getElementById('status');

    let active = false;
    let currentX = 0;
    let currentY = 0;

    // Center position
    const centerX = container.offsetWidth / 2 - joystick.offsetWidth / 2;
    const centerY = container.offsetHeight / 2 - joystick.offsetHeight / 2;

    // Max radius for joystick movement
    const maxRadius = container.offsetWidth / 2 - joystick.offsetWidth / 2;

    function handleStart(e) {
      active = true;
    }

    function handleMove(e) {
      if (!active) return;

      let clientX, clientY;

      if (e.type === 'touchmove') {
        clientX = e.touches[0].clientX;
        clientY = e.touches[0].clientY;
      } else {
        clientX = e.clientX;
        clientY = e.clientY;
      }

      const rect = container.getBoundingClientRect();
      const x = clientX - rect.left - centerX - joystick.offsetWidth / 2;
      const y = clientY - rect.top - centerY - joystick.offsetHeight / 2;

      // Calculate distance from center
      const distance = Math.sqrt(x * x + y * y);

      // Normalize if outside the max radius
      let normalizedX = x;
      let normalizedY = y;

      if (distance > maxRadius) {
        normalizedX = (x / distance) * maxRadius;
        normalizedY = (y / distance) * maxRadius;
      }

      // Update joystick position
      joystick.style.left = (centerX + normalizedX) + 'px';
      joystick.style.top = (centerY + normalizedY) + 'px';

      // Calculate values between -100 and 100
      currentX = Math.round((normalizedX / maxRadius) * 100);
      currentY = Math.round((normalizedY / maxRadius) * -100); // Invert Y for intuitive control

      status.textContent = `X: ${currentX}, Y: ${currentY}`;

      // Send to server
      sendJoystickData(currentX, currentY);
    }

    function handleEnd() {
      active = false;

      // Return to center
      joystick.style.left = centerX + 'px';
      joystick.style.top = centerY + 'px';

      currentX = 0;
      currentY = 0;

      status.textContent = `X: ${currentX}, Y: ${currentY}`;

      // Send to server
      sendJoystickData(currentX, currentY);
    }

    function sendJoystickData(x, y) {
      fetch('/joystick', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({ x, y })
      });
    }

    // Mouse events
    joystick.addEventListener('mousedown', handleStart);
    document.addEventListener('mousemove', handleMove);
    document.addEventListener('mouseup', handleEnd);

    // Touch events
    joystick.addEventListener('touchstart', handleStart);
    document.addEventListener('touchmove', handleMove);
    document.addEventListener('touchend', handleEnd);

    // Prevent scrolling on touch devices
    document.body.addEventListener('touchmove', function(e) {
      if (active) e.preventDefault();
    }, { passive: false });
  </script>
</body>
</html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void handleJoystick() {
  String json = server.arg("plain");
  
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  
  int x = doc["x"];
  int y = doc["y"];
  
  // Format: J:X,Y\n
  String command = "J:" + String(x) + "," + String(y) + "\n";
  // Send to Cytron RP2040 via UART
  Serial1.println(command);
  
  // Debug output
  Serial.print("Joystick: ");
  Serial.println(command);

  // Check for incoming data
  if (Serial1.available()) {
    String receivedData = Serial1.readStringUntil('\n');
    Serial.print("RP2040: ");
    String x = String(receivedData[0]) + String(receivedData[1]);
    String y = String(receivedData[2]) + String(receivedData[3]);
    Serial.print("x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.println(y);
    Serial.println(receivedData);
    delay(50);
  }
  
  server.send(200, "text/plain", "OK");
}
