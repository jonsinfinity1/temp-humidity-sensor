# temp-humidity-sensor
Project that contains source, stl file, and info for a BME280 temperature humidity sensor controlled by an Adafruit Feather HUZZAH with ESP8266 for use in an environment such as a greenhouse.

## code
The temp-humidity-sensor.ino file is amalgamation of a few different examples that I found online.

## secrets
The temp-humidity-sensor.ino file imports a secrets.h file that is not included in the project for obvious reasons. This file contains the ssid, ssid password, aws iot enpoint, and certificates.

Here is the format of the certificates that I got to work. The example is the publicly available AWS Root CA so no problem using it as an example. I created the file manually but if you're using the Arduino Web IDE you can probably enter it as one long string without all the quoted and backslashes.

```

#define SECRET_CA_CERT "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy" \
"rqXRfboQnoZsG4q5WTP468SQvvG5"

```

** build
I went through quite a few iterations to get the project box to build right. One of the issues I ran into is that the diagram that Adafruit provides for the Feather HUZZAH with ESP8266 is that the screw hole measurements on the diagram did not match the feather that I ordered. Not sure if it's a fluke or if they're just not manufactured to spec. Make sure to double check your feather screw hole measurements agains the measurements on the Fusion 360 files included in the project.

I used knurled inserts for M2 screws that I found at (https://www.amazon.com/gp/product/B01IZ157KS/ref=ppx_yo_dt_b_asin_title_o09_s01?ie=UTF8&psc=1)

For the battery pack I used the battery I found at (https://www.amazon.com/gp/product/B08FD39Y5R/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)

The BME280 I used was also from Amazon found at (https://www.amazon.com/gp/product/B01N47LZ4P/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1)
