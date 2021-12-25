package arch.sm213.machine.student;

import machine.AbstractMainMemory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.*;

import java.io.IOException;

import static org.junit.jupiter.api.Assertions.*;

public class MainMemoryTest {

    private MainMemory mainMemory;

    byte byte1;
    byte byte2;
    byte byte3;
    byte byte4;

    byte byte5;
    byte byte6;
    byte byte7;
    byte byte8;

    int int1;
    int int2;
    byte[] bytes1;
    byte[] bytes2;

    // creates a MainMemory object called mainMemory before each test
    @BeforeEach
    void runBefore() {
        mainMemory = new MainMemory(32);
    }

    @Test
    void testIsAccessAligned() {
        // checks if isAccessAligned is true when address == length
        assertTrue(mainMemory.isAccessAligned(4,4));
        // checks if isAccessAligned is true when address % length == 0
        assertTrue(mainMemory.isAccessAligned(12,4));

        // checks if isAccessAligned is false when address < length and address % length != 0
        assertFalse(mainMemory.isAccessAligned(3,4));
        // checks if isAccessAligned is false when address > length and address % length != 0
        assertFalse(mainMemory.isAccessAligned(13,4));
    }

    @Test
    void testBytesToInteger() {

        // checks if bytesToInteger can convert positive bytes

        byte1 = 01; // 01 bytes is 0000 0001 in binary a.k.a 01 in hexadecimal
        byte2 = 02; // 02 bytes is 0000 0010 in binary a.k.a 02 in hexadecimal
        byte3 = 03; // 03 bytes is 0000 0011 in binary a.k.a 03 in hexadecimal
        byte4 = 04; // 04 bytes is 0000 0100 in binary a.k.a 04 in hexadecimal

        assertEquals(mainMemory.bytesToInteger(byte1, byte2, byte3, byte4), 16909060);

        // checks if bytesToInteger can convert both positive and negative bytes at the same time

        byte5 = 70;  //  70 in bytes is 0100 0110 in binary a.k.a 46 in hexadecimal
        byte6 = 113; // 113 in bytes is 0111 0001 in binary a.k.a 71 in hexadecimal
        byte7 = -43; // -43 in bytes is 1101 0101 in binary a.k.a d5 in hexadecimal
        byte8 = -99; // -99 in bytes is 1001 1101 in binary a.k.a 9d in hexadecimal

        assertEquals(mainMemory.bytesToInteger(byte5, byte6, byte7, byte8), 1181865373);

    }

    @Test
    void testIntegerToBytes() {

        // checks if integerToBytes can convert positive integers to bytes
        // we first use the bytesToInteger method to to create a positive integer
        int1 = mainMemory.bytesToInteger((byte) 32, (byte) 88, (byte) -94, (byte) 59);
        // then, we call the integerToBytes method and store the values inside the array of bytes called "bytes1"
        bytes1 = new byte[4];
        bytes1 = mainMemory.integerToBytes(int1);

        assertEquals(bytes1[0], 32);
        assertEquals(bytes1[1], 88);
        assertEquals(bytes1[2], -94);
        assertEquals(bytes1[3], 59);

        // checks if integerToBytes can convert negative integers to bytes
        // we first use the bytesToInteger method to to create a negative integer
        int2 = mainMemory.bytesToInteger((byte) -125, (byte) 2, (byte) 57, (byte) 113);
        // then, we call the integerToBytes method and store the values inside the array of bytes called "bytes2"
        bytes2 = new byte[4];
        bytes2 = mainMemory.integerToBytes(int2);

        assertEquals(bytes2[0], -125);
        assertEquals(bytes2[1], 2);
        assertEquals(bytes2[2], 57);
        assertEquals(bytes2[3], 113);
    }

    @Test
    void testSetBytesWithInvalidAddress() {
        // first, create an array of 6 bytes so that we have an object of byte[] to set in memory
        byte[] value = new byte[6];
        value[0] = 1;
        value[1] = -24;
        value[2] = 99;
        value[3] = -64;
        value[4] = 127;
        value[5] = -127;

        // we are expecting to catch an exception in this case because 32 <= 30+6-1
        try {
            mainMemory.set(30, value);
            fail("We shouldn't have gotten here.");
        } catch (AbstractMainMemory.InvalidAddressException e) {
            // do nothing
        }
    }

    @Test
    void testSetBytesWithValidAddress() {
        // first, create an array of 6 bytes so that we have an object of byte[] to set in memory
        byte[] value = new byte[6];
        value[0] = 1;
        value[1] = -24;
        value[2] = 99;
        value[3] = -64;
        value[4] = 127;
        value[5] = -127;

        // we are NOT expecting to catch an exception in this case because 32 > 20+6-1
        try {
            mainMemory.set(20, value);
        } catch (AbstractMainMemory.InvalidAddressException e) {
            fail("We shouldn't have gotten here.");
        }

        //now, we check if what we have set in the memory is correct
        try {
            assertEquals(mainMemory.get(20,6)[0], 1);
            assertEquals(mainMemory.get(20,6)[1], -24);
            assertEquals(mainMemory.get(20,6)[2], 99);
            assertEquals(mainMemory.get(20,6)[3], -64);
            assertEquals(mainMemory.get(20,6)[4], 127);
            assertEquals(mainMemory.get(20,6)[5], -127);
        } catch (AbstractMainMemory.InvalidAddressException e) {
            fail("We shouldn't have gotten here.");
        }

    }

    @Test
    void testGetBytesWithInvalidAddress() {
        // we are expecting to catch an exception in this case because 32 <= 30+8
        try {
            assertEquals(mainMemory.get(30,8), bytes1);
            fail("We shouldn't have gotten here.");
        } catch (AbstractMainMemory.InvalidAddressException e) {
            // do nothing
        }
    }

    @Test
    void testGetBytesWithValidAddress() {
        // first, we create an array of 4 bytes called "value"
        byte[] value = new byte[4];
        value[0] = 1;
        value[1] = -24;
        value[2] = 99;
        value[3] = -64;

        // we set 4 values starting from address 16 in mainMemory for testing purposes
        try {
            mainMemory.set(16, value);
        } catch (AbstractMainMemory.InvalidAddressException e) {
            fail("We shouldn't have gotten here.");
        }

        // we are NOT expecting to catch an exception in this case because 32 > 16+4
        try {
            assertEquals(mainMemory.get(16,4)[0], 1);
            assertEquals(mainMemory.get(16,4)[1], -24);
            assertEquals(mainMemory.get(16,4)[2], 99);
            assertEquals(mainMemory.get(16,4)[3], -64);
        } catch (AbstractMainMemory.InvalidAddressException e) {
            fail("We shouldn't have gotten here.");
        }
    }

    @Test
    void testLength() {
        assertEquals(mainMemory.length(), 32);
        assertNotEquals(mainMemory.length(), 30);
    }


}