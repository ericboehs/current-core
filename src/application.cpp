/**
 ******************************************************************************
 * @file    application.cpp
 * @authors  Satish Nair, Zachary Crockett and Mohit Bhoite
 * @version V1.0.0
 * @date    05-November-2013
 * @brief   Tinker application
 ******************************************************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/  
#include "application.h"

/* Function prototypes -------------------------------------------------------*/
int rssi = 0;
int sr_data = A5;
int sr_clock = A3;
int sr_latch = A2;
int shift(String args);

/* This function is called once at start up ----------------------------------*/
void setup()
{
  Spark.variable("rssi", &rssi, INT);
  Spark.function("shiftout", shift);

  pinMode(sr_data, OUTPUT);
  pinMode(sr_clock, OUTPUT);
  pinMode(sr_latch, OUTPUT);
  digitalWrite(sr_data, LOW);
  digitalWrite(sr_clock, LOW);
  digitalWrite(sr_latch, LOW);

  Spark.publish("connected");
}

/* This function loops forever --------------------------------------------*/
void loop()
{
  rssi = Network.RSSI();
  delay(5000);
}

int shift(String bits) {
	uint8_t i;
	for (i = 0; i < 16; i++)  {
    if (bits.charAt(i) == '1') {
      digitalWrite(sr_data, HIGH);
    }
    digitalWrite(sr_clock, HIGH);
    digitalWrite(sr_clock, LOW);
    digitalWrite(sr_data, LOW);
  }

  digitalWrite(sr_latch, HIGH);
  digitalWrite(sr_latch, LOW);

  Spark.publish("shifted", bits);
  return 0;
}
