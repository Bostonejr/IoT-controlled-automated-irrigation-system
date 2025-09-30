#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Arduino_NetworkConfigurator.h>
#include "configuratorAgents/agents/BLEAgent.h"
#include "configuratorAgents/agents/SerialAgent.h"
void onPumpStateChange();
void onValveState1Change();
void onValveState2Change();

CloudSwitch pump_state;
CloudSwitch valve_state1;
CloudSwitch valve_state2;
int moisture_level_1;
int moisture_level_2;

KVStore kvStore;
BLEAgentClass BLEAgent;
SerialAgentClass SerialAgent;
WiFiConnectionHandler ArduinoIoTPreferredConnection; 
NetworkConfiguratorClass NetworkConfigurator(ArduinoIoTPreferredConnection);

void initProperties(){
  NetworkConfigurator.addAgent(BLEAgent);
  NetworkConfigurator.addAgent(SerialAgent);
  NetworkConfigurator.setStorage(kvStore);
  // For changing the default reset pin uncomment and set your preferred pin. Use DISABLE_PIN for disabling the reset procedure.
  //NetworkConfigurator.setReconfigurePin(your_pin);
  ArduinoCloud.setConfigurator(NetworkConfigurator);

  ArduinoCloud.addProperty(pump_state, READWRITE, ON_CHANGE, onPumpStateChange);
  ArduinoCloud.addProperty(valve_state1, READWRITE, 2 * SECONDS, onValveState1Change);
  ArduinoCloud.addProperty(valve_state2, READWRITE, 2 * SECONDS, onValveState2Change);
  ArduinoCloud.addProperty(moisture_level_1, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(moisture_level_2, READ, 1 * SECONDS, NULL);

}
