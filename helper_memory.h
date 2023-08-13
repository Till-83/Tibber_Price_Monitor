// For debugging - The Tibber API provides JSON data that requieres memory efficient handling
void display_freeram() {
    Serial.print("- SRAM left: ");
    Serial.println(ESP.getFreeHeap());
}