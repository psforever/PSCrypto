include Makefile.inc

$(info === Build configuration ===)
$(info Archtecture: $(ARCH))
$(info Operating system: $(OS))

CRYPTOPP_DIR=external/psf-cryptopp
CRYPTOPP_LIB=$(CRYPTOPP_DIR)/libcryptopp.a
PSCRYPTO=pscrypto/$(call lib-name,pscrypto)

all : $(CRYPTOPP_LIB) $(PSCRYPTO)

$(CRYPTOPP_LIB) :
	$(MAKE) -C $(CRYPTOPP_DIR)

$(PSCRYPTO) : $(CRYPTOPP_LIB)
	$(MAKE) -C pscrypto/

clean :
	$(MAKE) -C pscrypto/ clean
	$(MAKE) -C $(CRYPTOPP_DIR) clean

.PHONY : all clean
