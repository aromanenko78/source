import serial

modem = [f for f in os.listdir('/dev/') if f.find('tty.usbmodem') == 0][0]
ser = serial.Serial('/dev/' + modem, 9600)
while True:
  if ser.inWaiting() > 0:
    print 'Received from serial: %s' % ser.readline()
  text = raw_input("Enter text: ")
  if text:
    text += '\n'
    print 'Sending: %s' % text
    ser.write(text)
  else:
    print 'Checking serial'
