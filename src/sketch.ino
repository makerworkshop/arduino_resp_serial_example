// RESP library for encoding/decoding RESP messages.
#include "resp.h"

// Bee library for communicating devices using RESP encoded messages.
#include "Bee.h"

// Serial port baud rate.
#define SERIAL_BAUD_RATE 115200

// SerialConn is a communication channel for Bee that can be used to send and
// receive messages over serial port.
class SerialConn :
public Bee {
  typedef Bee super;
public:
  bool Open();
  bool Read(char *);
  bool Write(unsigned char *, int);
  bool Close();
  // respObject *OnMessage(respObject *); // OnMessage is commented since it's
                                          // an optional method.
};

// Open sets up the serial port.
bool SerialConn::Open()
{
  Serial.begin(SERIAL_BAUD_RATE);
  return true;
}

// Read reads a byte from the serial port and sets the value to <*b>.
bool SerialConn::Read(char *b)
{
  if (Serial.available() > 0) {
    *b = Serial.read();
    return true;
  }
  return false;
}

// Write writes the given buffer to the serial port.
bool SerialConn::Write(unsigned char *buf, int len)
{
  return (Serial.write(buf, len) == len);
}

// Close disables serial port communication.
bool SerialConn::Close()
{
  Serial.end();
  return true;
}

/*
// OnMessage implements some demo messages that are interpreted if
// super::OnMessage does not know how to handle them.
respObject *SerialConn::OnMessage(respObject *in)
{
  respObject *out = NULL;

  // Calling super::OnMessage is completely optional, you may as well ignore
  // any pre-defined message interpretation.
  out = super::OnMessage(in);

  if (out == NULL) {

    // FOO command returns a status BAR message.
    // FOO
    // +BAR
    if (RESP_TOKEN_EQUALS(in, 0, "FOO")) {
      out = createRespString(RESP_OBJECT_STATUS, "BAR");
    }

    // SUM command sums two integers and returns the resulting number.
    // SUM 5 4
    // :9
    if (RESP_TOKEN_EQUALS(in, 0, "SUM")) {
      int a = RESP_TOKEN_TO_INT(in, 1);
      int b = RESP_TOKEN_TO_INT(in, 2);
      out = createRespInteger(a + b);
    }

  }

  return out;
}
*/

// Global pointer to SerialConn instance.
SerialConn *conn;

void setup()
{
  // Setting up communication channel over serial port.
  conn = new SerialConn();
  if (!conn->Open()) {
    while(1);
  }
}

void loop()
{
  // Reading next message.
  conn->NextMessage();
}
