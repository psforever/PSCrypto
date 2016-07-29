#!/usr/bin/env python2
from binascii import hexlify
from ctypes import *
import os

lib = cdll.LoadLibrary(os.path.dirname(os.path.realpath(__file__)) + "/libpscrypto.so")

lib.MD5_MAC.restype = c_bool
lib.MD5_MAC.prototype = [c_char_p, c_uint, c_char_p, c_uint, c_char_p, c_uint]

lib.DH_Start.restype = c_void_p 
lib.DH_Start.prototype = [c_char_p, c_char_p, c_char_p]

lib.DH_Agree.restype = c_bool
lib.DH_Agree.prototype = [c_void_p, c_char_p, c_char_p, c_char_p]

lib.RC5_Init.restype = c_void_p
lib.RC5_Init.prototype = [c_char_p, c_uint, c_bool]

lib.RC5_Decrypt.restype = c_bool
lib.RC5_Decrypt.prototype = [c_void_p, c_char_p, c_uint, c_char_p]

lib.RC5_Encrypt.restype = c_bool
lib.RC5_Encrypt.prototype = [c_void_p, c_char_p, c_uint, c_char_p]

def hexToBin(data):
    return data.decode('hex')

def binToHex(data):
    return hexlify(data)

def md5mac(key, msg, outSz):
    outBuf = create_string_buffer(outSz)
    lib.MD5_MAC(key, len(key), msg, len(msg), outBuf, outSz)

    return outBuf.raw

def ServerDHStart(p, g):
    outPrivate = create_string_buffer(16)
    outPublic = create_string_buffer(16)
    dhHandle = lib.DH_Start(p, g, outPrivate, outPublic)

    return (dhHandle, outPrivate.raw, outPublic.raw)

def ServerDHAgree(dhHandle, privKey, otherPubKey):
    outAgree = create_string_buffer(16)
    status = lib.DH_Agree(dhHandle, outAgree, privKey, otherPubKey)

    return (status, outAgree.raw)

def ServerInitEnc(key):
    return lib.RC5_Init(key, len(key), True)

def ServerInitDec(key):
    return lib.RC5_Init(key, len(key), False)

def ServerDec(decHandle, ct):
    pt = create_string_buffer(len(ct))
    ret = lib.RC5_Decrypt(decHandle, ct, len(ct), pt)

    if not ret:
        print "Decryption failed!"
        return ""

    return pt.raw

def ServerEnc(encHandle, pt):
    ct = create_string_buffer(len(pt))
    ret = lib.RC5_Encrypt(encHandle, pt, len(pt), ct)

    if not ret:
        print "Encryption failed!"
        return ""

    return ct.raw

def main():
    (dhHandle, priv, pub) = ServerDHStart("\x41\x75"*8, "\x01"*15 + "\x03")
    print "Started"
    print ServerDHAgree(dhHandle, priv, "A"*16)

    key = "377b60f8790f91b35a9da82945743da9"
    label = "master secret"
    msg = "b4aea1559444a20b6112a2892de40eac00000000c8aea155b53d187076b79abab59001b600000000"
    expect = "5aa15de41f5220cf5cca489155e1438c5aa15de4"

    print "Key: " + key
    print "Msg: " + msg

    # append the label
    key = hexToBin(key)
    msg = label + hexToBin(msg)
    outBuf = md5mac(key, msg, 20)

    print "Expect: " + expect
    print "Got: " + binToHex(outBuf)

if __name__ == "__main__":
    main()
