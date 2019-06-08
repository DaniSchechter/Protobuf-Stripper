#!/bin/bash

# ./generate_certificate.sh clear . google.com
if [ "$#" = 3 ] && [ "$1" = "clear" ]; then
	eval "rm -r $2/$3"
elif [ "$#" -ne 2 ]; then
    echo "You have entered: "
    for i in $*; do 
        echo $i 
    done
    echo "Error: Wrong number of arguments"
    echo "Usage: generate_certificate.sh [PATH TO CRETIFICATES] [NEW DOMAIN]"
    echo ""
    exit 1
fi

# If the folder already exists, do not create a new one
if [ -e "$1/$2" ]; then
    exit 
fi

# ./generate_certificate.sh . google.com
CERTIFICATE_FOLER_PATH=$1/$2 		    # Folder called 		 ./google.com
PKEY_NAME="$CERTIFICATE_FOLER_PATH/$2"	# Will create a new file ./google.com/google.com.key
COMMON_NAME="$2"					

ROOT_CERTIFICATE_PATH=$1"/ROOT"
ROOT_CERTIFICATE_NAME="myCA"
ABSOLUTE_ROOT_CERTIFICATE_NAME="$ROOT_CERTIFICATE_PATH/$ROOT_CERTIFICATE_NAME"

# Create a new folder 
mkdir $CERTIFICATE_FOLER_PATH

# Create a provite key
openssl genrsa -out $PKEY_NAME.key 2048

# Create a csr request 
openssl req -new -key $PKEY_NAME.key -out $PKEY_NAME.csr -subj "/C=IL/ST=Israel/L=Israel=/O=Protobuf Stripper/OU=Development Department/CN=$COMMON_NAME"

# Create the certificate
openssl x509 -req -in $PKEY_NAME.csr -CA $ABSOLUTE_ROOT_CERTIFICATE_NAME.crt -CAkey $ABSOLUTE_ROOT_CERTIFICATE_NAME.key -CAcreateserial -out $PKEY_NAME.crt -days 1825 -sha256

echo "Done"

