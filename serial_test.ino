void setup() {
RTL8720D.begin(115200);
Serial.begin(115200);

}

void loop() {
 if (RTL8720D.available())
            {

                int inbyte = RTL8720D.read();

                Serial.write(inbyte);
                

}
}
