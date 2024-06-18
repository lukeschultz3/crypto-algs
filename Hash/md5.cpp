/*
 * MD5 implementation
 * Based on the pseudocode from http://www.practicalcryptography.com/hashes/md5-hash/
 * Written with the help of GitHub Copilot
 *
 * Luke Schultz
 * 2024
*/

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <cassert>

using namespace std;

int F(int X, int Y, int Z) {
    return (X & Y) | (~X & Z);
}

int G(int X, int Y, int Z) {
    return (X & Y) | (Y & ~Z);
}

int H(int X, int Y, int Z) {
    return X ^ Y ^ Z;
}

int I(int X, int Y, int Z) {
    return Y ^ (X | ~Z);
}

// initialize T array
int T[64] = [0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
             0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
             0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
             0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
             0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
             0xd62f105d,  0x2441453, 0xd8a1e681, 0xe7d3fbc8,
             0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
             0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
             0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
             0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
             0x289b7ec6, 0xeaa127fa, 0xd4ef3085,  0x4881d05,
             0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
             0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
             0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
             0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
             0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391];

int md5(string message) {
    int messageBits = message.length() * 8;
    int numPadding = 512 - ((messageBits + 64) % 512);
    if (numPadding == 0) {
        numPadding = 512;
    }

    cout << "message length: " << message.length() << endl;
    cout << "messageBits: " << messageBits << endl;
    cout << "numPadding: " << numPadding << endl;

    assert(numPadding > 0 && numPadding <= 512);
    assert((numPadding + messageBits + 64) % 512 == 0);

    const int numWords = (messageBits + numPadding + 64) / 32;
    cout << "numWords: " << numWords << endl;

    vector <unsigned long long> words(numWords);

    int wordIndex = 0;
    for (int i = 0; i < message.length(); i++) {
        words[wordIndex] = (words[wordIndex] << 8) | message[i];
        if ((i+1) % 4 == 0) {
            wordIndex++;
        }
    }

    // Add padding of 1 followed by 0s
    words[words] = (words[words] << 1) | 0x01;
    for (int i = 0; i < numPadding-1; i++) {
        words[wordIndex] = (words[words] << 1) | 0x00;
        if ((i+2) % 32 == 0) {
            wordIndex++;
        }
    }

    // Add length of message
    assert(messageBits < (1 << 32));
    words[wordIndex] = 0x00;
    words[wordIndex+1] = (words[wordIndex] << 32) | messageBits;

    int A = 0x01234567;
    int B = 0x89abcdef;
    int C = 0xfedcba98;
    int D = 0x76543210;

    for (int i = 0; i < numWords / 16; i++) {
        array<int, 16> X;
        for (int j = 0; j < 16; j++) {
            X[j] = words[i*16 + j];
        }

        int AA = A;
        int BB = B;
        int CC = C;
        int DD = D;

        // Round 1
        for (int i = 0; i < 4; i++) {
            A = B + ((A + F(B, C, D) + X[i*4+0] + T[i*4+0]) <<< 7)
            D = A + ((D + F(A, B, C) + X[i*4+1] + T[i*4+1]) <<< 12)
            C = D + ((C + F(D, A, B) + X[i*4+2] + T[i*4+2]) <<< 17)
            B = C + ((B + F(C, D, A) + X[i*4+3] + T[i*4+3]) <<< 22)
        }

        // Round 2
        for (int i = 0; i < 4; i++) {
            A = B + ((A + G(B, C, D) + X[(i*4+1)%16] + T[i*4+17]) <<< 5)
            D = A + ((D + G(A, B, C) + X[(i*4+6)%16] + T[i*4+18]) <<< 9)
            C = D + ((C + G(D, A, B) + X[(i*4+11)%16] + T[i*4+19]) <<< 14)
            B = C + ((B + G(C, D, A) + X[(i*4+0)%16] + T[i*4+20]) <<< 20)
        }

    }


    return 0;

}