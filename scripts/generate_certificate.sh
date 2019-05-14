#!/bin/bash

# ./generate_certificate.sh clear . google.com
if [ "$#" = 3 ] && [ "$1" = "clear" ]; then
	eval "rm -r $2/$3"
	exit 1
elif [ "$#" -ne 2 ]; then
    echo "Error: Wrong number of arguments"
    echo "Usage: generate_certificate.sh [PATH TO CRETIFICATES] [NEW DOMAIN]"
    echo ""
fi

# If the folder already exists, do not create a new one
if [ -e "$1/$2" ]; then
    exit 
fi

# ./generate_certificate.sh . google.com
CERTIFICATE_FOLER_PATH=$1/$2 		    # Folder called 		 ./google.com
PKEY_NAME="$CERTIFICATE_FOLER_PATH/$2"	# Will create a nre file ./google.com/google.com.key
COMMON_NAME="*.$2"					

ROOT_CERTIFICATE_PATH="./keys/ROOT"
ROOT_CERTIFICATE_NAME="myCA"
ABSOLUTE_ROOT_CERTIFICATE_NAME="$ROOT_CERTIFICATE_PATH/$ROOT_CERTIFICATE_NAME"

# Create a new folder 
eval "mkdir $CERTIFICATE_FOLER_PATH"

# Create a provite key
eval "openssl genrsa -out $PKEY_NAME.key 2048"

# Create a csr request 
eval "openssl req -new -key $PKEY_NAME.key -out $PKEY_NAME.csr -subj \"/C=IL/ST=Israel/L=Israel=/O=Protobuf Stripper/OU=Development Department/CN=$COMMON_NAME\""

# Create the certificate
eval "openssl x509 -req -in $PKEY_NAME.csr -CA $ABSOLUTE_ROOT_CERTIFICATE_NAME.crt -CAkey $ABSOLUTE_ROOT_CERTIFICATE_NAME.key -CAcreateserial -out $PKEY_NAME.crt -days 1825 -sha256"

echo "Done"

