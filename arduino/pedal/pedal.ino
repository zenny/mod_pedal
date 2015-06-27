/*
 * Copyright (C) 2015  Alister Sanders
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
