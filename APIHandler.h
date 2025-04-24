void sendSubscriptionConfirmation(String uuid) {
  Serial.println("Sending subscription confirmation...");

  String endpoint = "/api/subscribe-to-websocket";

  JsonDocument payload;
  payload["deviceUid"] = DEV_UID;
  payload["deviceApiKey"] = DEV_API_KEY;
  payload["webSocketUuid"] = uuid;

  String rawPayload;
  serializeJson(payload, rawPayload);

  fetch(endpoint, rawPayload);

  Serial.println("Subscription confirmation sent");
}
