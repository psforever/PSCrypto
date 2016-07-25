// PSCrypto - PSForever
// https://cryptopp.com/wiki/Diffie-Hellman

#ifdef BUILD_DLL
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C" __declspec(dllimport)
#endif

#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptlib.h"
using CryptoPP::Exception;

#include "hmac.h"
using CryptoPP::HMAC;

#include "sha.h"
using CryptoPP::SHA256;

#include "md5.h"
using CryptoPP::MD5;

#include "rc5.h"
using CryptoPP::RC5;

#include "dh.h"
using CryptoPP::DH;
using CryptoPP::DH_Domain;
using CryptoPP::DL_GroupParameters_GFP_DefaultSafePrime;
using CryptoPP::DL_GroupParameters_IntegerBasedImpl;

#include "integer.h"
using CryptoPP::Integer;

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "md5mac.h"
using CryptoPP::MD5MAC;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HashVerificationFilter;

#include "secblock.h"
using CryptoPP::SecByteBlock;

#include <stdint.h>
#include <iostream>
#include <iomanip>
using namespace std;

#include "pscrypto.h"

#define min(x, y) (((x) > (y)) ? (y) : (x))

DLL_EXPORT bool PSCrypto_Init(int major, int minor)
{
  return PSCRYPTO_VERSION_MAJOR == major && PSCRYPTO_VERSION_MINOR == minor;
}

DLL_EXPORT void PSCrypto_Get_Version(int * major, int * minor)
{
  if(major && minor)
  {
    *major = PSCRYPTO_VERSION_MAJOR;
    *minor = PSCRYPTO_VERSION_MINOR;
  }
}

DLL_EXPORT const char * PSCrypto_Version_String()
{
  return PSCRYPTO_VERSION_STRING;
}

DLL_EXPORT DH * DH_Start(const byte * p, const byte * g, byte * privKey, byte * pubKey)
{
	DH * dh = new DH();
	if(!dh)
              return NULL;

	AutoSeededRandomPool rnd(0x128);
	Integer pInt(p, 0x10);
	Integer gInt(g, 0x10);

	dh->AccessGroupParameters().Initialize(pInt, gInt);
	dh->GenerateKeyPair(rnd, privKey, pubKey);

	return dh;
}

DLL_EXPORT DH * DH_Start_Generate(byte * privKey, byte * pubKey, byte * p, byte * g)
{
	DH * dh = new DH();
	AutoSeededRandomPool rnd;

	if(!dh)
		return NULL;

	dh->AccessGroupParameters().Initialize(rnd, 128);
	dh->GenerateKeyPair(rnd, privKey, pubKey);

	const Integer& pInt = dh->GetGroupParameters().GetModulus();
	const Integer& gInt = dh->GetGroupParameters().GetGenerator();

	pInt.Encode(p, 0x10);
	gInt.Encode(g, 0x10);

	return dh;
}

DLL_EXPORT bool DH_Agree(DH * dh, byte * agreedValue, const byte * privKey,
		const byte * otherPubKey)
{
	if(!dh || !agreedValue || !privKey || !otherPubKey)
		return false;

	return dh->Agree(agreedValue, privKey, otherPubKey, true);
}

DLL_EXPORT void * RC5_Init(byte * key, size_t keyLen, bool encrypt)
{
	if(!key || !keyLen)
		return NULL;

	if(encrypt)
		return new RC5::Encryption(key, keyLen);
	else
		return new RC5::Decryption(key, keyLen);

}

DLL_EXPORT bool RC5_Decrypt(RC5::Decryption * dec, byte * ct, size_t ctLen, byte * pt)
{
	if(!dec || !ct || !pt || ctLen & (dec->BlockSize()-1))
		return false;

	for(int i = 0; i < ctLen; i+=dec->BlockSize())
		dec->ProcessAndXorBlock(ct+i, NULL, pt+i);

	return true;
}

DLL_EXPORT bool RC5_Encrypt(RC5::Encryption * enc, byte * pt, size_t ptLen, byte * ct)
{
	if(!enc || !ct || !pt || ptLen & (enc->BlockSize()-1))
		return false;

	for(int i = 0; i < ptLen; i+= enc->BlockSize())
		enc->ProcessAndXorBlock(pt+i, NULL, ct+i);

	return true;
}

DLL_EXPORT bool MD5_MAC(byte * key, size_t keyLen, byte * msg, size_t msgLen, byte * outBuf, size_t outBufLen)
{
	if(!key || !msg || !outBuf)
		return false;

	byte localKey[16];
	byte digest[MD5MAC::DIGESTSIZE];

	memset(digest, 0, sizeof(digest));
	memset(localKey, 0, sizeof(localKey));
	memcpy(localKey, key, min(keyLen, 16));

	// key length of 16 bytes or less
	MD5MAC mac(localKey);

	mac.Update(msg, msgLen);
	mac.Final(digest);

	for(size_t i = 0; i < outBufLen; i += MD5MAC::DIGESTSIZE)
		memcpy(outBuf+i, digest, min(MD5MAC::DIGESTSIZE, outBufLen-i));

	return true;
}

DLL_EXPORT void Free_DH(DH * ptr)
{
	delete ptr;
}

DLL_EXPORT void Free_RC5(RC5 * ptr)
{
	delete ptr;
}

DLL_EXPORT void MD5Test()
{
	const byte key[] = {0x37, 0x7b, 0x60, 0xf8, 0x79, 0x0f, 0x91, 0xb3, 0x5a, 0x9d, 0xa8, 0x29, 0x45, 0x74, 0x3d, 0xa9};
	const byte msg[] =  {'m', 'a', 's', 't', 'e', 'r', ' ', 's', 'e', 'c', 'r', 'e', 't', 
	0xb4, 0xae, 0xa1, 0x55, 0x94, 0x44, 0xa2, 0x0b, 0x61, 0x12, 0xa2, 0x89, 0x2d, 0xe4, 0x0e, 0xac, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xae, 0xa1, 0x55, 0xb5, 0x3d, 0x18, 0x70, 0x76, 0xb7, 0x9a, 0xba, 0xb5, 0x90, 0x01, 0xb6, 0x00, 0x00, 0x00, 0x00};
	const byte output[] = {0x5a, 0xa1, 0x5d, 0xe4, 0x1f, 0x52, 0x20, 0xcf, 0x5c, 0xca, 0x48, 0x91, 0x55, 0xe1, 0x43, 0x8c, 0x5a, 0xa1, 0x5d, 0xe4};
	byte digest[MD5MAC::DIGESTSIZE+4];
	string encoded;

	std::ios::sync_with_stdio(true);

	MD5MAC mac(key);

	StringSource(key, sizeof(key), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "key: " << encoded << endl;

	encoded = "";

	StringSource(msg, sizeof(msg), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "msg: " << encoded << endl;

	encoded = "";

	StringSource(output, sizeof(output), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "expected: " << encoded << endl;

	mac.Update(msg, sizeof(msg));
	mac.Final(digest);

	encoded = "";

	StringSource(digest, sizeof(digest), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "got: " << encoded << endl;

}

static void HMACTest()
{
	AutoSeededRandomPool prng;

	unsigned char fixedKey[16] = {0x37, 0x7b, 0x60, 0xf8, 0x79, 0x0f, 0x91, 0xb3, 0x5a, 0x9d, 0xa8, 0x29, 0x45, 0x74, 0x3d, 0xa9};
	SecByteBlock key(fixedKey, 16);

	//for(int i = 0; i++; i < 16)
		//key[i] = fixedKey[i];
	//prng.GenerateBlock(key, key.size());

	string plain = "master secret";
	string mac, encoded;

	/*********************************\
	\*********************************/

	// Pretty print key
	encoded.clear();
	StringSource(key, key.size(), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	cout << "key: " << encoded << endl;

	cout << "plain text: " << plain << endl;

	/*********************************\
	\*********************************/

	try
	{
		HMAC< MD5 > hmac(key, key.size());		

		StringSource(plain, true, 
			new HashFilter(hmac,
				new StringSink(mac)
			) // HashFilter      
		); // StringSource
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}
}

static bool ValidateMD5MAC()
{
	const byte keys[2][MD5MAC::KEYLENGTH]={
		{0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff},
		{0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}};

	const char *TestVals[7]={
		"",
		"a",
		"abc",
		"message digest",
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890"};

	const byte output[2][7][MD5MAC::DIGESTSIZE]={
		{{0x1f,0x1e,0xf2,0x37,0x5c,0xc0,0xe0,0x84,0x4f,0x98,0xe7,0xe8,0x11,0xa3,0x4d,0xa8},
		{0x7a,0x76,0xee,0x64,0xca,0x71,0xef,0x23,0x7e,0x26,0x29,0xed,0x94,0x52,0x73,0x65},
		{0xe8,0x01,0x3c,0x11,0xf7,0x20,0x9d,0x13,0x28,0xc0,0xca,0xa0,0x4f,0xd0,0x12,0xa6},
		{0xc8,0x95,0x53,0x4f,0x22,0xa1,0x74,0xbc,0x3e,0x6a,0x25,0xa2,0xb2,0xef,0xd6,0x30},
		{0x91,0x72,0x86,0x7e,0xb6,0x00,0x17,0x88,0x4c,0x6f,0xa8,0xcc,0x88,0xeb,0xe7,0xc9},
		{0x3b,0xd0,0xe1,0x1d,0x5e,0x09,0x4c,0xb7,0x1e,0x35,0x44,0xac,0xa9,0xb8,0xbf,0xa2},
		{0x93,0x37,0x16,0x64,0x44,0xcc,0x95,0x35,0xb7,0xd5,0xb8,0x0f,0x91,0xe5,0x29,0xcb}},
		{{0x2f,0x6e,0x73,0x13,0xbf,0xbb,0xbf,0xcc,0x3a,0x2d,0xde,0x26,0x8b,0x59,0xcc,0x4d},
		{0x69,0xf6,0xca,0xff,0x40,0x25,0x36,0xd1,0x7a,0xe1,0x38,0x03,0x2c,0x0c,0x5f,0xfd},
		{0x56,0xd3,0x2b,0x6c,0x34,0x76,0x65,0xd9,0x74,0xd6,0xf7,0x5c,0x3f,0xc6,0xf0,0x40},
		{0xb8,0x02,0xb2,0x15,0x4e,0x59,0x8b,0x6f,0x87,0x60,0x56,0xc7,0x85,0x46,0x2c,0x0b},
		{0x5a,0xde,0xf4,0xbf,0xf8,0x04,0xbe,0x08,0x58,0x7e,0x94,0x41,0xcf,0x6d,0xbd,0x57},
		{0x18,0xe3,0x49,0xa5,0x24,0x44,0xb3,0x0e,0x5e,0xba,0x5a,0xdd,0xdc,0xd9,0xf1,0x8d},
		{0xf2,0xb9,0x06,0xa5,0xb8,0x4b,0x9b,0x4b,0xbe,0x95,0xed,0x32,0x56,0x4e,0xe7,0xeb}}};

	byte digest[MD5MAC::DIGESTSIZE];
	bool pass=true, fail;

	cout << "\nMD5MAC validation suite running...\n";

	for (int k=0; k<2; k++)
	{
		MD5MAC mac(keys[k]);
		cout << "\nKEY: ";
		for (int j=0;j<MD5MAC::KEYLENGTH;j++)
			cout << setw(2) << setfill('0') << hex << (int)keys[k][j];
		cout << endl << endl;
		for (int i=0;i<7;i++)
		{
			mac.Update((byte *)TestVals[i], strlen(TestVals[i]));
			mac.Final(digest);
			fail = memcmp(digest, output[k][i], MD5MAC::DIGESTSIZE)
				 || !mac.VerifyDigest(output[k][i], (byte *)TestVals[i], strlen(TestVals[i]));
			pass = pass && !fail;
			cout << (fail ? "FAILED   " : "passed   ");
			for (int j=0;j<MD5MAC::DIGESTSIZE;j++)
				cout << setw(2) << setfill('0') << hex << (int)digest[j];
			cout << "   \"" << TestVals[i] << '\"' << endl;
		}
	}

	return pass;
}

/*key = "377b60f8790f91b35a9da82945743da9"
label = "master secret"
msg = "b4aea1559444a20b6112a2892de40eac00000000c8aea155b53d187076b79abab59001b600000000"
#msg = ""
expect = "5aa15de41f5220cf5cca489155e1438c5aa15de4"

print "Key: " + key
print "Msg: " + msg

# append the label
msg = label + hexToBin(msg)*/

int main(int argc, char* argv[])
{
        DH dh;
	AutoSeededRandomPool rnd(0x128);

	cout << "Generating parameters..." << endl;
	dh.AccessGroupParameters().Initialize(rnd, 128);
	// for server, we are given p and g
	// use this: dh.AccessGroupParameters().Initialize(p, g);

	cout << "Generating Keypair..." << endl;
	
	SecByteBlock privKey(dh.PrivateKeyLength());
	SecByteBlock pubKey(dh.PublicKeyLength());
	dh.GenerateKeyPair(rnd, privKey, pubKey);

	const Integer& p = dh.GetGroupParameters().GetModulus();
	cout << "P: " << hex << p << endl;

	Integer q = (p-1)/2;
	cout << "Q: " << q << endl;

	const Integer& g = dh.GetGroupParameters().GetGenerator();
	cout << "G: " << g << endl;

	Integer r = dh.GetGroupParameters().GetSubgroupOrder();
	cout << "Subgroup order: " << r << endl;

	cout << "Key generation test..." << endl;

	byte buf1[16] = {
			'a','a','b','b',
			'a','a','b','b',
			'a','a','b','b',
			'a','a','b','c'};
	byte buf2[16];
	byte buf3[16];


	cout << "Output " << DH_Start(buf1, buf1, buf2, buf3) << endl;

	return 0;
}
