CRYPTOPP_DIR=external/psf-cryptopp
CRYPTOPP_LIB=$(CRYPTOPP_DIR)/libcryptopp.a
PSCRYPTO=pscrypto/pscrypto.dll

all : $(CRYPTOPP_LIB) $(PSCRYPTO)

$(CRYPTOPP_LIB) :
	$(MAKE) -C $(CRYPTOPP_DIR)

$(PSCRYPTO) : $(CRYPTOPP_LIB)
	$(MAKE) -C pscrypto/

clean :
	$(MAKE) -C pscrypto/ clean
	$(MAKE) -C $(CRYPTOPP_DIR) clean
