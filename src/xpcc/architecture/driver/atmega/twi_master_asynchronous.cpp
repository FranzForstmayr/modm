// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
// ----------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "twi_master.hpp"	

// TWI State machine value when finished
#define TWI_STATUS_DONE 0xff

// ----------------------------------------------------------------------------
static uint8_t twiAddress;
static const uint8_t *twiWriteBuffer;
static uint8_t *twiReadBuffer;
static uint8_t twiReadCounter;
static uint8_t twiWriteCounter;

static uint8_t twiStatus = TWI_STATUS_DONE;

// ----------------------------------------------------------------------------
/// TWI state machine interrupt handler
ISR(TWI_vect)
{
	switch(TW_STATUS)
	{
	case TW_START:
		// START has been transmitted
		// Fall through...
	case TW_REP_START:
		// REPEATED START has been transmitted
		
		// Load data register with TWI slave address
		TWDR = twiAddress;
		// TWI Interrupt enabled and clear flag to send next byte
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		break;
	
	case TW_MT_SLA_ACK:
		// SLA+W has been transmitted and ACK received
		// Fall through...
	case TW_MT_DATA_ACK:
		// Data byte has been transmitted and ACK received
		if (twiWriteCounter != 0)
		{
			// Decrement counter
			twiWriteCounter--;
			// Load data register with next byte
			TWDR = *twiWriteBuffer++;
			// TWI Interrupt enabled and clear flag to send next byte
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		}
		else
		{
			// Transfer finished
			if (twiReadCounter == 0) {
				// Initiate STOP condition after last byte; TWI Interrupt disabled
				TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
				
				twiStatus = TWI_STATUS_DONE;
			}
			else {
				// Copy address; set R/~W bit in SLA+R/W address field
				twiAddress = twiAddress | 0x01;
				
				// Initiate a Repeated START condition;
				// Interrupt enabled and flag cleared
				TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
			}
		}
		break;
	
	case TW_MR_DATA_ACK:
		// Data byte has been received and ACK transmitted
		*twiReadBuffer++ = TWDR;
		twiReadCounter--;
		
		// Fall through...
	case TW_MR_SLA_ACK:
		// SLA+R has been transmitted and ACK received
		// See if last expected byte will be received ...
		if (twiReadCounter > 1) {
			// Send ACK after reception
			TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
		}
		else {
			// Send NACK after next reception
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
		}
		break;
	
	case TW_MR_DATA_NACK:
		// Data byte has been received and NACK transmitted
		// => Transfer finished
		*twiReadBuffer++ = TWDR;
		twiReadCounter--;
		
		//if (twiStopCondition) {
			// Initiate STOP condition after last byte; TWI Interrupt disabled
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
		//}
		//else {
			// Disable TWI Interrupt, stop condition must be generated by
			// a call of endTransmission()
			//TWCR = (1 << TWEN);
		//}
		twiStatus = TWI_STATUS_DONE;
		break;
	
	case TW_MT_ARB_LOST:
		// Arbitration lost...
		// Initiate a (repeated) start condition, Interrupt enabled and flag cleared
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
		break;
	
	default:
		// Error condition; save status
		// Reset TWI Interface; disable interrupt
		twiStatus = TWSR;
		TWCR = (1 << TWEN);
	}
}

// ----------------------------------------------------------------------------
void
xpcc::AsynchronousTwiMaster::initialize(uint8_t twbr, uint8_t twps)
{
	twiReadCounter = 0;
	twiWriteCounter = 0;
	
	// Initialize TWI clock
	TWBR = twbr;
	TWSR = twps & 0x03;
	
	// Load data register with default content; release SDA
	TWDR = 0xff;
	
	// Enable TWI peripheral with interrupt disabled
	TWCR = (1 << TWEN);
}

// ----------------------------------------------------------------------------
void
xpcc::AsynchronousTwiMaster::read(
		uint8_t address, uint8_t *data, std::size_t size)
{
	while (isBusy()) {
		// Wait for previous transaction to finish
	}
	
	// Copy address; set R/~W bit in SLA+R/W address field
	twiAddress = address | 0x01;
	
	// Save pointer to data and number of bytes to receive
	twiReadBuffer = data;
	twiReadCounter = size;
	
	// Initiate a START condition; Interrupt enabled and flag cleared
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

// ----------------------------------------------------------------------------
void
xpcc::AsynchronousTwiMaster::write(
		uint8_t address, const uint8_t *data, std::size_t size)
{
	while (isBusy()) {
		// Wait for previous transaction to finish
	}
	
	// Copy address; clear R/~W bit in SLA+R/W address field
	twiAddress = address & 0xfe;
	
	// Save pointer to data and number of bytes to send
	twiWriteBuffer = data;
	twiWriteCounter = size;
	twiReadCounter = 0;
	
	// Initiate a START condition; Interrupt enabled and flag cleared
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}

// ----------------------------------------------------------------------------
void
xpcc::AsynchronousTwiMaster::writeRead(uint8_t address, uint8_t *data,
		std::size_t writeSize, std::size_t readSize)
{
	while (isBusy()) {
		// Wait for previous transaction to finish
	}
	
	// prepare read operation, switching from write to read will be done
	// automatically in the TWI interrupt
	twiReadCounter = readSize;
	twiReadBuffer = data;
	
	// Start the first write operation which does the rest
	write(address, data, writeSize);
}

// ----------------------------------------------------------------------------
bool
xpcc::AsynchronousTwiMaster::isBusy()
{
	// If the TWI interrupt is enabled then the peripheral is busy
	return (TWCR & (1 << TWIE));
}

// ----------------------------------------------------------------------------
xpcc::i2c::Status
xpcc::AsynchronousTwiMaster::getStatus()
{
	if (twiStatus == TWI_STATUS_DONE) {
		return i2c::SUCCESS;
	}
	else {
		return i2c::ERROR;
	}
}
