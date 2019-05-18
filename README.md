# Protobuf-Stripper

## Getting started

Because git doesnt track empty directories, and beacase we havn't added secret files to some directories just so they wont be empty, some directories need to be created manually before the program can be compiled successfully.

## Folders to be Created under the root directory of the project

1. **.sslkeys** - containing all the certificatese the program generates for secure HTTPS connection.
*scripts/generate_certificate.sh* will add the certificates under this folder

2. **.sslkeys/ROOT** - containing the self generated ROOT CA and private keys that will be used to sign every leaf certificate.

## Generating the needed files

1. For creating a compiled executable using 'make'

       i. create a folder called *bin*, under the root directory of the project
2. For running the application with a configuration file

    1. create a folder called include
    2. create a file called config.json and paste the following into it: ( you can modify the settings )

    ```javascript
    {
        "httpProxy" :
        {
            "localHost": "127.0.0.1",
            "localPort": 5432,
            "numOfThreads": 5
        }
    }
    ```

    localHost - If you want to run locally, leave it    '127.0.0.1'. If you want to run on the LAN, change it to   the external address

3. For generating certificates

    Under ***.sslkeys/ROOT*** folder run the following, and do not change names

    1. ```openssl genrsa -des3 -out myCA.key 2048``` :      will generate a private key that will be used while       signing every leaf certificates for each domain

    2. Fill In The pass phrase. We will remove it in just a second

    3. ```openssl rsa -in myCA.key -out myCA.key``` : will     remove the pass phrases you have just creadted

    4. ```openssl req -x509 -new -nodes -key myCA.key   -sha256      -days 1825 -out myCA.crt``` : will generate a r   ot     certificate called *myCA.crt*

    5. You will prompted for a bunch of questions. The       answers to those questions aren’t that important. They just show up when looking at the certificte. Only the Common Name should be set to something that you’ll        recognize as your root certificate in a list of other    certificates.

    6. You should now have two files under *.sslkeys*   directory: *myCA.key* (your private key) and *myCA.crt* (your root certificate.

    7. Now import your root certificate - *myCA.crt* (and not your key - *myCA.key*) as a trusted ROOT CA of your favorite web browser ( Firefox prefared, easy to be made )
