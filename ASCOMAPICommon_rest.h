/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOMAPI_Common
#define _ASCOMAPI_Common
#include "AlpacaErrorConsts.h"
#include "ArduinoJson.h"
#include "JSONHelperFunctions.h"
#define JSON_BUF_SIZE  512

//PUT /{DeviceType}/{DeviceNumber}/Action Invokes the specified device-specific action.
void handleAction(void);
//PUT /{DeviceType}/{DeviceNumber}/CommandBlind Transmits an arbitrary string to the device
void handleCommandBlind(void);
//PUT /{DeviceType}/{DeviceNumber}/CommandBool Transmits an arbitrary string to the device and returns a boolean value from the device.
void handleCommandBool(void);
//PUT /{DeviceType}/{DeviceNumber}/CommandString Transmits an arbitrary string to the device and returns a string value from the device
void handleCommandString(void);
//GET /{DeviceType}/{DeviceNumber}/Connected Retrieves the connected state of the device
//PUT /{DeviceType}/{DeviceNumber}/Connected Sets the connected state of the device
void handleConnected(void);
//GET /{DeviceType}/{DeviceNumber}/Description Device description
void handleDescriptionGet(void);
//GET /{DeviceType}/{DeviceNumber}/Driverinfo Device driver description
void handleDriverInfoGet(void);
//GET /{DeviceType}/{DeviceNumber}/DriverVersion Driver Version
void handleDriverVersionGet(void);
//GET /{DeviceType}/{DeviceNumber}/Name Device name
void handleNameGet(void);
void handleNamePut(void); //Non-ASCOM , required by setup
void handleFilterCountPut(void); 
//GET /{DeviceType}/{DeviceNumber}/SupportedActions Returns the list of action names supported by this driver.  
void handleSupportedActionsGet(void);

int serverTransID= 0;

void handleAction(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBlind", AE_notImplemented, "Not implemented" );
    root["Value"]= "";
	root["ErrorNumber"]= AE_notImplemented;
    root["ErrorMessage"] = "not implemented";
    root.printTo(message);
    server.send(200, "application/json", message);
    return;
 }

void handleCommandBlind(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBlind", AE_notImplemented, "Not implemented" );
    root["Value"]= "";
	root["ErrorNumber"]= AE_notImplemented;
    root["ErrorMessage"] = "not implemented";
    root.printTo(message);
    server.send(200, "application/json", message);
    return;
}

void handleCommandBool(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBool", AE_notImplemented, "Not implemented" );
    root["Value"]= false; 
	root["ErrorNumber"]= AE_notImplemented;
    root["ErrorMessage"] = "not implemented";
    root.printTo(message);   
    server.send(200, "application/json", message);
    return;
}

void handleCommandString(void)
{
    String message;
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandString", AE_notImplemented, "Not implemented" );
    root["Value"]= ""; 
	root["ErrorNumber"]= AE_notImplemented;
    root["ErrorMessage"] = "not implemented";
    root.printTo(message);   
    server.send(200, "application/json", message);
    return;
}

void handleConnected(void)
{
    String message;
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" );
    
    if ( server.method() == HTTP_PUT )
    { 
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.printf( "handleConnected: arg:%s, connected: %i\n", server.arg("Connected").c_str(), connected );
#endif

      //don't like the logic here - if it's already connected for this client we should refuse a connect. 
      if( server.arg("Connected").equalsIgnoreCase( "true" ) ) 
      { //setting to true 
        if ( connected )//already true
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" /*"Setting connected when already connected"*/ );        
          root["Value"]= true;    
          root.prettyPrintTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.printf( "handleConnected: output:%s\n", message.c_str() );
#endif
          server.send( 200, "application/json", message ); 
          return;
        }
        else //OK
        {
           connected = true;
           root["Value"] = true; 
        }
      }
      else //'Connected' = false
      {
        if ( connected ) //
        {
          //If we are disconnecting, we should park or home and close the shutter ?
#ifdef _ASCOM_Dome
          if( parkDomeOnDisconnect )
              addDomeCmd( 0, 0, CMD_DOME_PARK, 0);
          if( closeShutterOnDisconnect )
              addDomeCmd( 0, 0, CMD_SHUTTER_CLOSE, 0 );
#endif 
          connected = false; //OK          
          root.remove( "Value" );
        }
        else
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "Clearing 'connected' when already cleared" );        
          root.remove( "Value" );
          root.printTo(message);   
          server.send( 200, "application/json", message);
          return;          
        }
      }
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" );        
      root.remove( "Value" );
      root.printTo(message);
      server.send( 200, "application/json", message);
      return;          
    }
    else if ( server.method() == HTTP_GET )
    {
      //Check error numbers
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" );        
      root["Value"] = connected;      
      root.printTo(message);
      server.send( 200, "application/json", message);
      return;          
    }
    else
    {
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_invalidOperation , "Unexpected HTTP method" );        
      root["Value"] = connected;      
      root.printTo(message);
      server.send( 200, "application/json", message);
      return;          
    }
}

void handleDescriptionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Description", AE_Success, "" );    
    root["Value"]= DriverName;  // ASCOM device type  
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}

void handleDriverInfoGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "DriverInfo", AE_Success, "" );    
    root["Value"]= DriverInfo;    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}

void handleDriverVersionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "DriverVersion", AE_Success, "" );    
    root["Value"]= DriverVersion; // int   
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}

void handleInterfaceVersionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "InterfaceVersion", AE_Success, "" );    
    root["Value"]= InterfaceVersion;    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}

void handleNameGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Name", AE_Success, "" );    
    root["Value"] = DriverName;  // ASCOM device type
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}

void handleSupportedActionsGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "SupportedActions", AE_Success, "" );    
    JsonArray& values  = root.createNestedArray("Value");   
    //values.add(""); //Explicitly empty array    todo
    root.printTo(message);
    server.send(200, "application/json", message);
    return ;
}



//  =====        Management API    =====

// /management/apiversions
void handleAPIversions(void)
{
	// Returns an integer array of supported Alpaca API version numbers.
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    JsonArray& values  = root.createNestedArray("Value");     
    values.add(atoi(InterfaceVersion.c_str()));  
	root["ClientTransactionID"]= clientTransID;
	root["ServerTransactionID"]= ++serverTransID;
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
    // if 400 or 500
	//root["ErrorNumber"]= errNum;
    //root["ErrorMessage"] = errMsg;
}

//   /management/v1/description
void handleAPIdescription(void)
{
	//  Returns cross-cutting information that applies to all devices available at this URL:Port.
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    //jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Description", AE_Success, "" );    
	JsonObject& Value = root.createNestedObject("Value");
    Value["ServerName"]= SERVERNAME;
    Value["Manufacturer"]= MFG;
    Value["ManufacturerVersion"]= MFG_VERSION;
    Value["Location"]= LOCATION;   
	root["ClientTransactionID"]= clientTransID;
	root["ServerTransactionID"]= ++serverTransID;
	root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}

//  /management/v1/configureddevices
void handleAPIconfiguredDevices(void)
{
	// Returns an array of device description objects, providing unique information for each served device, 
	// enabling them to be accessed through the Alpaca Device API.
    String message, msg2;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(JSON_BUF_SIZE);
    JsonObject& root = jsonBuff.createObject();
    JsonArray& values  = root.createNestedArray("Value");
    DynamicJsonBuffer jsonBuff2(JSON_BUF_SIZE-200); // necessary for the mega
    JsonObject& device= jsonBuff2.createObject();
    device["DeviceName"]= Description; // To appear in list to user
    device["DeviceType"]= DriverName; // ASCOM type
    device["DeviceNumber"]= INSTANCE_NUMBER;
    device["UniqueID"]= GUID; 
    values.add(device);
	// todo are there more?
	root["ClientTransactionID"]= clientTransID;
	root["ServerTransactionID"]= ++serverTransID;
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, "application/json", message);
    return ;
}



#endif
  
