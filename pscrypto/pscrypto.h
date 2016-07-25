#ifndef PSCRYPTO_H
#define PSCRYPTO_H

#define _PS_STRINGIZE(S) #S

#define PSCRYPTO_VERSION_MAJOR 1
#define PSCRYPTO_VERSION_MINOR 0

#define PSCRYPTO_VERSION_STRING _PS_STRINGIZE(PSCRYPTO_VERSION_MAJOR) "." _PS_STRINGIZE(PSCRYPTO_VERSION_MINOR)

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: all crypto is 128-bit (16 bytes) which is terrible and pretty much ineffective.

// Version verification

/// Returns initialization status. 1 for okay, 0 for failure
bool PSCrypto_Init(int major, int minor);

/// Returns version numbers
void PSCrypto_Get_Version(int * major, int * minor);

/// Returns version string
const char * PSCrypto_Version_String();

// Diffie-Helman (shared secret agreement)

/**
 * Starts a Diffie-Helman key exchange with another party who's P and G values
 * are already known.
 *
 * @param p 16 bytes of P value
 * @param g 16 bytes of G value
 * @param privKey A generated private key (16 bytes) [out]
 * @param pubKey A generated public key (16 bytes) [out]
 * @returns Newly created CryptoPP DH object or NULL on failure
 */
DH * DH_Start(const byte * p, const byte * g, byte * privKey, byte * pubKey);

/**
 * Starts a new Diffie-Helman key exchange
 *
 * @param privKey A generated private key (16 bytes) [out]
 * @param pubKey A generated public key (16 bytes) [out]
 * @param p Generated P value (16 bytes) [out]
 * @param g Generated G value (16 bytes) [out]
 * @returns Newly created CryptoPP DH object or NULL on failure
 */
DH * DH_Start_Generate(byte * privKey, byte * pubKey, byte * p, byte * g);

/**
 * Perform an agreement with another party to come to a shared secret
 *
 * @param dh A previously created DH object
 * @param agreedValue A generated 128-bit shared secret when function returns true [out]
 * @param privKey Our private key
 * @param otherPubKey The other party's public key
 * @returns True on a successful agreement. False on some failure
 */
bool DH_Agree(DH * dh, byte * agreedValue, const byte * privKey, const byte * otherPubKey);

/// Free created DH object
void Free_DH(DH * ptr);

// RC5 (encryption and decryption)

/**
 * Initialize an RC5 instance with the specfied key
 *
 * @param key The input key buffer
 * @param keyLen The input key length in bytes
 * @param encrypt True for an encryption instance. False for decryption
 * @returns A newly created RC5 instance or NULL on failure
 */
void * RC5_Init(byte * key, size_t keyLen, bool encrypt);

/**
 * Perform an RC5 decryption
 *
 * @param dec A previously created RC5 decryption instance
 * @param ct A ciphertext buffer
 * @param ptLen The length of the ciphertext. Must be aligned to the size of an
 *              RC5 block (16 bytes)
 * @param pt A pointer to a destination plaintext buffer of size ctLen [out]
 * @returns True on a successful decryption
 */
bool RC5_Decrypt(RC5::Decryption * dec, byte * ct, size_t ctLen, byte * pt);

/**
 * Perform an RC5 encryption
 *
 * @param enc A previously created RC5 encryption instance
 * @param pt A plaintext buffer
 * @param ptLen The length of the plaintext. Must be aligned to the size of an
 *              RC5 block (16 bytes)
 * @param ct A pointer to a destination ciphertext buffer of size ptLen [out]
 * @returns True on a successful encryption
 */
bool RC5_Encrypt(RC5::Encryption * enc, byte * pt, size_t ptLen, byte * ct);

/// Free a previously created RC5 object
void Free_RC5(RC5 * ptr);

// MD5MAC (packet integrity and authenticity)

/**
 * Performs a single MD5 MAC computation
 *
 * @param key Input key buffer
 * @param keyLen Size of the input key buffer
 * @param msg Message to create a MAC for
 * @param msgLen Length of the message
 * @param outBuf Output buffer to receive the MAC value (16 for a full MAC)
 * @param outBufLen Output buffer length. If the length is smaller than the size
 *                  of a single MAC, the result will be truncated. If it is
 *                  bigger, multiple MAC values will be concatenated until the
 *                  size is reached.
 * @returns Returns true on a successful MAC
 */
bool MD5_MAC(byte * key, size_t keyLen, byte * msg, size_t msgLen, byte * outBuf, size_t outBufLen);

#ifdef __cplusplus
}
#endif

#endif
