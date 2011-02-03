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
/**
 * \ingroup		driver
 * \defgroup	i2c		(I2C) Inter-Integrated Circuit
 * 
 * I2C is a synchronous protocol that allows a master device to initiate
 * communication with a slave device. Data is exchanged between these devices.
 * It was invented by Philips to attach low-speed peripherals to a motherboard,
 * embedded system or other devices.
 * 
 * The I2C bus consists of two lines: Clock and Data with a strong pull-up
 * resistor (usually 4.7kOhm) on each. A logical 0 is signified by actively
 * pulling the line low (open collector output) and a logical 1 is signified
 * by passively leaving the line high (hi-Z). Standard communication speed
 * is up to 100kHz and high speed is up to 400 kHz. The maximum communication
 * speed is affected by the line capacitance and EMI.
 * 
 * After every 8 bits of data transferred a 1 bit handshake is performed to
 * indicate success ... an ACK is generated by pulling the line low (logical 0).
 * A NACK is signified by leaving the line high (logical 1).
 * 
 * \see	<a href="http://en.wikipedia.org/wiki/I2c" target="_blank">
 * 		http://en.wikipedia.org/wiki/I2c</a>
 */

#include "i2c/interface.hpp"
#include "i2c/software_i2c.hpp"
