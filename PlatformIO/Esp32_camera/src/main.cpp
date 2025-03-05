#include "esp_camera.h"
#include "Arduino.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define ENABLE_WEBSERVER 1  // Imposta a 0 per disattivare il web server

const char* ssid = "Paolo";
const char* password = "12345678";

#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

AsyncWebServer server(80);

void setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_QVGA; // QVGA (320x240) o usa FRAMESIZE_VGA per 640x480
    config.jpeg_quality = 10;  // Qualità JPEG (8-12 è buono)
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.fb_count = 2;  // Usa doppio buffer per evitare lag

    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera init failed!");
        return;
    }
}

void detectBall() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Failed to capture image");
        return;
    }
    
    int width = fb->width;
    int height = fb->height;
    uint8_t *data = fb->buf;

    int bestX = -1, bestY = -1;
    bool foundSilver = false;

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int pixel = data[y * width + x];
            if (pixel > 200) {
                bestX = x;
                bestY = y;
                foundSilver = true;
            } else if (pixel < 50 && !foundSilver) {
                bestX = x;
                bestY = y;
            }
        }
    }
    
    if (bestX != -1 && bestY != -1) {
        Serial.printf("%d,%d\n", bestX, bestY);
    }
    
    esp_camera_fb_return(fb);
}

#if ENABLE_WEBSERVER
void startWebServer() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.println(WiFi.localIP());

    //Pagina HTML con aggiornamento automatico dello streaming
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", R"rawliteral(
            <html>
            <body>
                <h2>ESP32 Camera Stream</h2>
                <img id="stream" src="/stream">
                <script>
                    setInterval(() => {
                        document.getElementById("stream").src = "/stream?" + new Date().getTime();
                    }, 100);
                </script>
            </body>
            </html>
        )rawliteral");
    });

    //Singola cattura immagine
    server.on("/capture", HTTP_GET, [](AsyncWebServerRequest *request) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            request->send(500, "text/plain", "Camera capture failed");
            return;
        }
        AsyncWebServerResponse *response = request->beginResponse_P(200, "image/jpeg", fb->buf, fb->len);
        response->addHeader("Content-Type", "image/jpeg");
        request->send(response);
        esp_camera_fb_return(fb);
    });

    //Streaming video MJPEG
    server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginChunkedResponse("multipart/x-mixed-replace; boundary=frame",
        [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
            camera_fb_t *fb = esp_camera_fb_get();
            if (!fb) return 0;

            size_t len = snprintf((char *)buffer, maxLen,
                "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %zu\r\n\r\n", fb->len);
            
            size_t copyLen = (fb->len > (maxLen - len)) ? (maxLen - len) : fb->len;
            memcpy(buffer + len, fb->buf, copyLen);

            esp_camera_fb_return(fb);
            return len + copyLen;
        });
        
        request->send(response);
    });

    server.begin();
}
#endif

void setup() {
    Serial.begin(115200);
    setupCamera();
    #if ENABLE_WEBSERVER
    startWebServer();
    #endif
}

void loop() {
    detectBall();
    delay(500);
}
