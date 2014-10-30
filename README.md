SparkCore-PumpkinPush
=====================

Motion Activated Pumpkin Push Notifications

The Spark Core monitors a PIR motion sensor and when activated sends a customizable Title and Message to a PHP script that sits on your Web Host, this PHP script then forwards the Title and Message to Pushover.net's API securely via HTTPS.  The Title and Message is then sent from Pushover.net as a Push Notification to your phone.  The system can be remotely enabled or disabled with an ON/OFF Spark function call via a simple web app like [this one](https://github.com/technobly/Simple-Spark-Core-Controller).  By default it's enabled though, so the ON/OFF web app is not required.

Components Required
---

- Spark Core
- Parallax PIR Sensor (or equivalent) available at [Radio Shack](radioshack.com)

Hookup
---

```
PIR sensor to Spark Core
GND to GND
VCC to 3V3
OUT to D0
EN to 3V3
```

Setup
---

#### Pushover.net
  - Create an account and make note of your "User Key".
  - Add Pushover app to your phone.
  - Under "Your Devices", register your phone with pushover acct.
  - Under "Your Applications", create a new application and call it whatever you'd like.  This is where you get your "API token".
    
    
#### Web host and PHP script
  - Add the Pushover User Key and API Token to the PHP script.
  - Upload the PHP script to a directory like this `your_web_host.com/pumpkin/pushover.php`

Credits and Inspiration
---

- Steph's Instructable http://www.instructables.com/id/The-JackO-Lantern-Early-Warning-System/
- WGBartley's Simple Spark PHP Proxy https://community.spark.io/t/simple-spark-php-proxy/4073
