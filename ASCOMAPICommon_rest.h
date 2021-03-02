/*
File to be included into relevant device REST setup 
*/
#if !defined _ASCOMAPI_Common
#define _ASCOMAPI_Common

/*
  Alpaca documentation: 
  https://github.com/ASCOMInitiative/ASCOMRemote/blob/master/Documentation/ASCOM%20Alpaca%20API%20Reference.pdf
  https://ascom-standards.org/api/
*/

#include "AlpacaErrorConsts.h"
#include "ArduinoJson.h" // version 6
#include "JSONHelperFunctions.h"
#define JSON_SIZE  512

//GET /{DeviceType}/{DeviceNumber}/SupportedActions Returns the list of action names supported by this driver.  
void handleSupportedActionsGet(void);
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

int serverTransID= 0;

void handleNotImplemented(void) {
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "", AE_notImplemented, "Property is not implemented" );    
    serializeJson(doc, message);
    server.send(200, "application/json", message);
}


void handleSupportedActionsGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "SupportedActions", AE_Success, "" );    
    JsonArray values  = root.createNestedArray("Value"); 
	if(SUPPORTED_ACTIONS_COUNT) {
		for(int i= 0; i < SUPPORTED_ACTIONS_COUNT; i++) {
			values.add(SupportedActions[i]);
		}	
	} // else it's empty an empty array
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

void handleAction(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
	String Action= server.arg("Action");
	String Parameters= server.arg("Parameters");
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
	// do we know how to deal with this Action?
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Action", AE_notImplemented, "Not implemented" );
    root["Value"]= "";
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return;
 }

void handleCommandBlind(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
	String Command= server.arg("Command");
	String Raw= server.arg("Raw");
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
	// Handle Command
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBlind", AE_notImplemented, "Not implemented" );
    root["Value"]= "";
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return;
}

void handleCommandBool(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
	String Command= server.arg("Command");
	String Raw= server.arg("Raw");
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    // Handle Command
	jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBool", AE_notImplemented, "Not implemented" );
    root["Value"]= false; 
    serializeJson(doc, message);   
    server.send(200, "application/json", message);
    return;
}

void handleCommandString(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandString", AE_notImplemented, "Not implemented" );
    root["Value"]= ""; 
    serializeJson(doc, message);   
    server.send(200, "application/json", message);
    return;
}

void handleConnected(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" );
    
    if ( server.method() == HTTP_PUT )
    { 
      //don't like the logic here - if it's already connected for this client we should refuse a connect. 
      if( server.arg("Connected").equalsIgnoreCase( "true" ) ) 
      { //setting to true 
        if ( connected )//already true
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" /*"Setting connected when already connected"*/ );        
          root["Value"]= true;    
          serializeJsonPretty(doc, message);
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
          serializeJson(doc, message);   
          server.send( 200, "application/json", message);
          return;          
        }
      }
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" );        
      root.remove( "Value" );
      serializeJson(doc, message);
      server.send( 200, "application/json", message);
      return;          
    }
    else if ( server.method() == HTTP_GET )
    {
      //Check error numbers
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_Success, "" );        
      root["Value"] = connected;      
      serializeJson(doc, message);
      server.send( 200, "application/json", message);
      return;          
    }
    else
    {
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", AE_invalidOperation , "Unexpected HTTP method" );        
      root["Value"] = connected;      
      serializeJson(doc, message);
      server.send( 200, "application/json", message);
      return;          
    }
}

void handleDescriptionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Description", AE_Success, "" );    
    root["Value"]= DriverName;  // ASCOM device type  
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

void handleDriverInfoGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "DriverInfo", AE_Success, "" );    
    root["Value"]= DriverInfo;    
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

void handleDriverVersionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "DriverVersion", AE_Success, "" );    
    root["Value"]= DriverVersion; // int   
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

void handleInterfaceVersionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "InterfaceVersion", AE_Success, "" );    
    root["Value"]= InterfaceVersion;    
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

void handleNameGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Name", AE_Success, "" );    
    root["Value"] = DriverName;  // ASCOM device type
    serializeJson(doc, message);
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
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIversions", AE_Success, "" );    
    JsonArray values  = root.createNestedArray("Value");     
    values.add(atoi(InterfaceVersion.c_str()));
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

//   /management/v1/description
void handleAPIdescription(void)
{
	//  Returns cross-cutting information that applies to all devices available at this URL:Port.
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIDescription", AE_Success, "" );    
	JsonObject Value = root.createNestedObject("Value");
    Value["ServerName"]= SERVERNAME;
    Value["Manufacturer"]= MFG;
    Value["ManufacturerVersion"]= MFG_VERSION;
    Value["Location"]= LOCATION;   
	serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}

//  /management/v1/configureddevices
void handleAPIconfiguredDevices(void)
{
	// Returns an array of device description objects, providing unique information for each served device, 
	// enabling them to be accessed through the Alpaca Device API.
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    StaticJsonDocument<JSON_SIZE> doc;
    JsonObject root = doc.to<JsonObject>();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIconfigureddevices", AE_Success, "" );    
    JsonArray values  = root.createNestedArray("Value");
	JsonObject device = values.createNestedObject(); // creates an object in the array
    device["DeviceName"]= Description; // To appear in list to user
    device["DeviceType"]= DriverName; // ASCOM type
    device["DeviceNumber"]= INSTANCE_NUMBER;
    device["UniqueID"]= GUID; 
	// todo are there more?
    serializeJson(doc, message);
    server.send(200, "application/json", message);
    return ;
}


#endif
  
