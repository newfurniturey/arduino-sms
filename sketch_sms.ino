/**
 * Arduino SMS
 * 
 * This sketch, for the Arduino GSM shield, sends a preset
 * message upon connecting to the GSM network and then
 * listens (forever) for incoming SMS messages.
 * 
 * Circuit:
 * GSM shield attached to and Arduino
 * SIM card that can place/receive phone calls and
 *   send/receive SMS messages
 */

#include <GSM.h>

/**
 * Your SIM's PIN number, if it requires one
 * @var string
 */
#define PINNUMBER ""
 
/**
 * Library initialization
 */
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;

/**
 * Telephone number to send the initial "we're connected" SMS to.
 * Prefix with the country-code, such as `1` for the U.S.
 * Sample: `18005551212`
 * @var string
 */
char notificationNumber[20] = "";

/**
 * Holds the phone number for the sender of the received SMS.
 * @var string
 */
char smsFromNumber[20];

/**
 * The message to send when we've connected to the GSM network.
 * @var string
 */
char connectionEstablishedMsg[200] = ":wave: ... we're connected (^_^)";

/**
 * Initializes the serial comms for the Arduino and monitors the GSM
 * initialization and connection status.
 */
void setup() {
    // initialize serial comms on port 9600
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for the port to connect
    }

    Serial.println("Arduino SMS");
    Serial.println("-----------");

    // wait for the GSM connection
    Serial.print("Connecting to GSM Network");
    while (gsmAccess.begin(PINNUMBER) != GSM_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nConnected!");
    
    // send connection-established message
    sendConnectionEstablishedMsg();
}

/**
 * Sends the preset connection-established message to the preset number.
 */
void sendConnectionEstablishedMsg() {
  Serial.println("Sending connection established message to:");
  Serial.println(notificationNumber);

  sms.beginSMS(notificationNumber);
  sms.print(connectionEstablishedMsg);
  sms.endSMS();
  Serial.println("\nSENT!\n");
}

/**
 * Waits until we have received an SMS and prints it to the serial connection.
 */
void loop() {
    char c;

    if (sms.available()) {
        Serial.println("Message received from:");

        // fetch the sender's phone number
        sms.remoteNumber(smsFromNumber, 20);
        Serial.println(smsFromNumber);

        // read and print the received message, byte-by-byte
        while (c = sms.read()) {
            Serial.print(c);
        }

        Serial.println("\n-");

        // remove the message from the modem's memory
        sms.flush();
    }

    delay(1000);
}
