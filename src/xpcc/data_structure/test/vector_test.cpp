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

#include <xpcc/data_structure/vector.hpp>

#include "vector_test.hpp"

typedef xpcc::Vector<int16_t> MyVector;

void
VectorTest::testDefaultConstrutor()
{
	MyVector vector;

	TEST_ASSERT_TRUE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 0);
}

void
VectorTest::testAllocationConstructor()
{
	MyVector vector(10);

	TEST_ASSERT_TRUE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 0);
	TEST_ASSERT_TRUE(vector.getCapacity() >= 10);
}

void
VectorTest::testSequenceConstructor()
{
	MyVector vector(10, 5);

	TEST_ASSERT_FALSE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 10);

	for (uint_fast16_t i = 0; i < 10; ++i) {
		TEST_ASSERT_EQUALS(vector[i], 5);
	}
}

void
VectorTest::testReserve()
{
	MyVector vector;
	vector.reserve(10);

	TEST_ASSERT_TRUE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 0);
	TEST_ASSERT_TRUE(vector.getCapacity() >= 10);
}

void
VectorTest::testAppend()
{
	MyVector vector;

	vector.append(20);

	TEST_ASSERT_FALSE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 1);

	TEST_ASSERT_EQUALS(vector[0], 20);

	vector.append(5);

	TEST_ASSERT_EQUALS(vector.getSize(), 2);

	TEST_ASSERT_EQUALS(vector[0], 20);
	TEST_ASSERT_EQUALS(vector[1], 5);
}

void
VectorTest::testRemove()
{
	MyVector vector(2, 5);

	TEST_ASSERT_FALSE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 2);

	vector.removeLast();

	TEST_ASSERT_FALSE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 1);

	vector.removeLast();

	TEST_ASSERT_TRUE(vector.isEmpty());
	TEST_ASSERT_EQUALS(vector.getSize(), 0);
}
