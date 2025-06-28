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
        const req = new XMLHttpRequest();
        req.addEventListener("load", reqListener);
        // let l=0,r=0;
        switch (true) {
            case y>5:
                switch (true) {
                    //right
                    case x>25:
                        req.open("GET", "http://192.168.4.1/js?json={%22T%22:1,%22L%22:0.5,%22R%22:-0.5}");
                        req.send();
                        break;
                    //left
                    case x<-25:
                        req.open("GET", "http://192.168.4.1/js?json={%22T%22:1,%22L%22:-0.5,%22R%22:0.5}");
                        req.send();
                        break;
                    //forwards
                    default:
                        req.open("GET", "http://192.168.4.1/js?json={%22T%22:1,%22L%22:0.5,%22R%22:0.5}");
                        req.send();
                        break;
                }
                break;
            case y<-5:
                req.open("GET", "http://192.168.4.1/js?json={%22T%22:1,%22L%22:-0.5,%22R%22:-0.5}");
                req.send();
                break;
            default:
                req.open("GET", "http://192.168.4.1/js?json={%22T%22:1,%22L%22:0.0,%22R%22:0.0}");
                req.send();
                break;

        }

        // teleoperation
        // const data = {
        //     "T":1,
        //     "L":l,
        //     "R":r
        // };
        // const JsonStr = JSON.stringify(data);
        // const url = `http://192.168.4.1/js?json=${JsonStr}`

        //heartbeat
        // const heartB = {
        //     "T":130,
        // };
        // const JsonHBStr = JSON.stringify(data);
        // const HBurl = `http://192.168.4.1/js?json=${JsonHBStr}`
        //
        // req.open("GET", HBurl);
        // req.send();


    }
    function reqListener() {
      console.log(this.responseText);
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