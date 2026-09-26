#include <Arduino.h>

#include <painlessMesh.h>

// WiFi Mesh khac han WiFi Station/AP thong thuong o cho: khong can 1 router
// trung tam - moi node ESP32 vua la client vua la "router" cho cac node
// khac, tu dong tim duong (routing) va tu vic khoi phuc (self-healing) neu
// 1 node giua duong bi mat ket nien. Thu vien painlessMesh lo het phan
// phuc tap nay, code chi can quan tam gui/nhan tin nhan.

#define MESH_PREFIX     "ESP32MeshNetwork" // ten mang mesh, moi node phai dung chung
#define MESH_PASSWORD   "MeshPassword123"  // mat khau mang mesh, toi thieu 8 ky tu
#define MESH_PORT       5555

const int ledPin = 26;

painlessMesh mesh;
Scheduler userScheduler;

void sendHeartbeat();
Task taskSendHeartbeat(TASK_SECOND * 5, TASK_FOREVER, &sendHeartbeat);

void sendHeartbeat() {
  String message = "Xin chao tu node: " + String(mesh.getNodeId());
  mesh.sendBroadcast(message);
  Serial.println("Da gui: " + message);
}

void receivedCallback(uint32_t from, String &message) {
  Serial.printf("Nhan tu node %u: %s\n", from, message.c_str());

  if (message == "LED_ON") {
    digitalWrite(ledPin, HIGH);
  } else if (message == "LED_OFF") {
    digitalWrite(ledPin, LOW);
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("Co node moi tham gia mang mesh: %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.println("Cau truc mang mesh vua thay doi (node them/mat ket noi)");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);

  userScheduler.addTask(taskSendHeartbeat);
  taskSendHeartbeat.enable();

  Serial.print("Node ID cua board nay: ");
  Serial.println(mesh.getNodeId());
}

void loop() {
  mesh.update(); // BAT BUOC goi lien tuc de mesh xu ly routing ngam
}
