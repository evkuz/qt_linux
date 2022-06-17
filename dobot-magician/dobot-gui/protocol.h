#ifndef PROTOCOL_H
#define PROTOCOL_H
/*
 *
 * Checksum Calculation
 * The checksum value is calculated based only on the payload portion of a command and used to both
 * signify the end of an instruction and provide some assurance that data has been transmitted correctly.
 * In Dobot Magician communication protocol, the checksum at the ending side is calculated as
 * follows.
 *
 * 1. Add all the contents of the Payload byte by byte (8 bits) to get a result R (8 bits);
 *    First sum all the values in the payload – including ID and Ctrl. This sum must always be
 *    8 bits long and uses big-endian representation. If the calculated value is larger than 255,
 *   it must be truncated so that only the 8 least significant bits are kept.
 *   277 (1 0001 0101) for example, will become 21 (‘0001 0101’). The sum for the getPose command
 *   is 10 (10 + 0).
 *
 * 2. Get the 2's complement of the result R (8 bits), and put it into "check byte".
 *
 * Note
    2's complement. For an N-bit number, the 2’s complement is equal to 2 ^ N minus the number. In
    this protocol, assuming the result R is 0x0A, and the 2’s complement, i.e., the result of the above
    checking is equal to (2 ^ 8 - 0x0A) = (256 - 10) = 246 = 0xF6.
    At the receiving end, the method of verifying whether a frame of data is correct as follows:
 *
 * 1. Add all the contents of the Payload byte by byte (8 bits) to get a result A;
 * 2. Add result A and the "check byte". If the result is 0, the checksum is correct.
 *
*/

typedef struct tagPose {
float x; //Robotic arm coordinate system x
float y; //Robotic arm .
float z; //Robotic arm coordinate system z
float r; //Robotic arm coordinate system r
float jointAngle[4];
//Robotic arm 4 axis(The basement, rear arm, forearm,EndEffector) angles
} Pose;

/*
From device : 0x31, 0x37, 0x30, 0x20, 0x31, 0x37, 0x30, 0x20, 0x32, 0x20, 0x31, 0x30, 0x20, 0x30, 0x20, 0x32, 0x34, 0x36, 0x0a, 0x57, 0x72, 0x6f, 0x6e, 0x67, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x21, 0x21
From device : 0x21, 0x0d, 0x0a, 0x52, 0x6f, 0x62, 0x6f, 0x74, 0x20, 0x63, 0x75, 0x72, 0x72, 0x65, 0x6e, 0x74, 0x20, 0x6d, 0x6f, 0x76, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x44, 0x4f, 0x4e, 0x45, 0x21, 0x20
From device : 0x21, 0x21, 0x21


From device : 0xaa, 0xaa, 0x22, 0x0a, 0x00,
              0xb0, 0x0a, 0x42, 0x43, = x == 194.042
              0x36, 0x85, 0x95, 0x42, = y == 74.7602
              0x94, 0xe8, 0x06, 0x43, = z == 134.909
              0xa0, 0x90, 0xa8, 0x41, = r == 21.0706
              0xa0, 0x90, 0xa8, 0x41, Base angle
              0x00, 0xb6, 0xbd, 0xbc, Rear arm angle
              0x98, 0x0c, 0x12, 0x3d, Forearm angle
              0x00, 0x00, 0x00, 0x00, Endeffector angle
              0x0c                    checksum

*/

#endif // PROTOCOL_H
