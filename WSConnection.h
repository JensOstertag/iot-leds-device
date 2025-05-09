#include <ArduinoWebsockets.h>

using namespace websockets;

WebsocketsClient websocketsClient;
String uuid = "";

bool newMessage = false;
String messageBuffer;

void onMessageCallback(WebsocketsMessage websocketsMessage) {
  messageBuffer = String(websocketsMessage.c_str());
  newMessage = true;
}

void subscribeToChannel() {
  Serial.println("Subscribing to WebSocket channel...");

  JsonDocument subscriptionMessageData;
  subscriptionMessageData["messageType"] = "CLIENT_ACTION";
  subscriptionMessageData["uuid"] = uuid;
  subscriptionMessageData["action"] = "SUBSCRIBE";
  JsonObject subscribeChannelData = subscriptionMessageData.createNestedObject("data");
  subscribeChannelData["channel"] = WS_CHANNEL;

  String subscriptionMessage;
  serializeJson(subscriptionMessageData, subscriptionMessage);

  websocketsClient.send(subscriptionMessage);
  Serial.println("Sent WebSocket channel subscription request");
}

void handleBufferedWebsocketsMessage() {
  if(!newMessage) {
    return;
  }

  JsonDocument payload;
  DeserializationError deserializationError = deserializeJson(payload, messageBuffer);

  if(deserializationError) {
    Serial.println("Received non-parsable message from the server:");
    Serial.println(messageBuffer);
    newMessage = false;
    return;
  }

  String messageType = payload["messageType"].as<const char*>();
  String message = payload["message"].as<const char*>();

  if(messageType.equals("CLIENT_ACK")) {
    Serial.print("Received ACK: ");
    Serial.println(message);

    if(message.equals("Connected")) {
      uuid = payload["data"]["uuid"].as<const char*>();
      Serial.print("WebSocket UUID: ");
      Serial.println(uuid);
      subscribeToChannel();
    }
  } else if(messageType.equals("CLIENT_PUSH")) {
    String pushChannel = payload["data"]["pushMessage"]["channel"].as<const char*>();
    String pushMessage = payload["data"]["pushMessage"]["message"].as<const char*>();

    if(!pushChannel.equals(WS_CHANNEL)) {
      Serial.print("Received message on wrong channel: ");
      Serial.println(pushMessage);
      newMessage = false;
      return;
    }

    String decryptedPushMessage = decryptAes(decodeBase64(pushMessage));

    JsonDocument systemMessage;
    DeserializationError systemMessageDeserializationError = deserializeJson(systemMessage, decryptedPushMessage);

    if(systemMessageDeserializationError) {
      Serial.println("Received non-parsable message from the server:");
      Serial.println(pushMessage);
      Serial.println(decryptedPushMessage);
      newMessage = false;
      return;
    }

    String systemMessageType = systemMessage["type"].as<const char*>();
    JsonObject systemMessageData = systemMessage["data"];

    Serial.print("Received system message: ");
    Serial.println(systemMessageType);

    if(systemMessageType.equals("PING")) {
      refreshTimeout();
    } else if(systemMessageType.equals("ANIMATION")) {
      parseAnimation(systemMessageData);
    }
  }

  newMessage = false;
}

void setupWSConnection() {
  Serial.println("Connecting to WebSocket server...");
  websocketsClient.onMessage(onMessageCallback);
  String wsHost = "";
  wsHost += WS_SECURE ? "wss://" : "ws://";
  wsHost += String(WS_HOST) + ":";
  wsHost += String(WS_PORT) + "/";
  bool connected = websocketsClient.connect(wsHost);
  if(!connected) {
    Serial.println("Failed to connect to WebSocket server");
  }
}

void maintainWSConnection() {
  if(websocketsClient.available()) {
    websocketsClient.poll();
    handleBufferedWebsocketsMessage();
  }
}
