/* Store the input data in a register to speed things up */
register unsigned char data asm("r16");

void setup()
{
  /* Set input modes for pins 2 & 3 */
  DDRD |= 0x0C;
  Serial.begin(9600);
}

void loop()
{
  /* Grab the values of pins 2 & 3 */
  data = (PIND >> 2) & 0x03;

  /* If there is data to send, send it */
  Serial.write(data);
  Serial.flush();
  
  if (!data)
    delay(5);
}
