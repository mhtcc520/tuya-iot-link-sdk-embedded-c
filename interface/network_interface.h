/**
 * @file network_interface.h
 * @brief NetworkContext_t interface definition for HTTP and MQTT client.
 *
 * Defines an interface to the TLS layer to be used by the HTTP and MQTT client.
 * Starting point for porting the SDK to the networking layer of a new platform.
 */

#ifndef __NETWORK_INTERFACE_H_
#define __NETWORK_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "tuya_error_code.h"

/**
 * @brief NetworkContext_t Type
 *
 * Defines a type for the network struct.  See structure definition below.
 */
typedef struct NetworkContext NetworkContext_t;

/**
 * @brief tls_context_t Type
 *
 * Defines a type for the tls context struct.
 */
typedef struct tls_context tls_context_t;

/**
 * @brief TLS Connection Parameters
 *
 * Defines a type containing TLS specific parameters to be passed down to the
 * TLS networking layer to create a TLS secured socket.
 */
typedef struct {
	const char *pRootCALocation;                ///< Pointer to string containing the filename (including path) of the root CA file.
	const char *pDeviceCertLocation;            ///< Pointer to string containing the filename (including path) of the device certificate.
	const char *pDevicePrivateKeyLocation;    ///< Pointer to string containing the filename (including path) of the device private key file.
	const char *pDestinationURL;                ///< Pointer to string containing the endpoint of the MQTT service.
	uint16_t DestinationPort;            ///< Integer defining the connection port of the MQTT service.
	uint32_t timeout_ms;                ///< Unsigned integer defining the TLS handshake timeout value in milliseconds.
	bool ServerVerificationFlag;        ///< Boolean.  True = perform server certificate hostname validation.  False = skip validation \b NOT recommended.
} TLSConnectParams;

/**
 * @brief NetworkContext_t Structure
 *
 * Structure for defining a network connection.
 */
struct NetworkContext {
	int (*connect)(NetworkContext_t *, TLSConnectParams *);
	int (*read) (NetworkContext_t *, unsigned char *, size_t);   ///< Function pointer pointing to the network function to read from the network
	int (*write)(NetworkContext_t *, const unsigned char *, size_t);    ///< Function pointer pointing to the network function to write to the network
	int (*disconnect)(NetworkContext_t *);    ///< Function pointer pointing to the network function to disconnect from the network
	int (*destroy)(NetworkContext_t *);        ///< Function pointer pointing to the network function to destroy the network object
	TLSConnectParams tlsConnectParams;        ///< TLSConnect params structure containing the common connection parameters
	tls_context_t* context;
};

/**
 * @brief Initialize the TLS implementation
 *
 * Perform any initialization required by the TLS layer.
 * Connects the interface to implementation by setting up
 * the network layer function pointers to platform implementations.
 *
 * @param pNetwork - Pointer to a NetworkContext_t struct defining the network interface.
 * @param pRootCALocation - Path of the location of the Root CA
 * @param pDeviceCertLocation - Path to the location of the Device Cert
 * @param pDevicyPrivateKeyLocation - Path to the location of the device private key file
 * @param pDestinationURL - The target endpoint to connect to
 * @param DestinationPort - The port on the target to connect to
 * @param timeout_ms - The value to use for timeout of operation
 * @param ServerVerificationFlag - used to decide whether server verification is needed or not
 *
 * @return int - successful initialization or TLS error
 */
int iot_tls_init(NetworkContext_t *pNetwork, const char *pRootCALocation, const char *pDeviceCertLocation,
					const char *pDevicePrivateKeyLocation, const char *pDestinationURL,
					uint16_t DestinationPort, uint32_t timeout_ms, bool ServerVerificationFlag);

/**
 * @brief Create a TLS socket and open the connection
 *
 * Creates an open socket connection including TLS handshake.
 *
 * @param pNetwork - Pointer to a NetworkContext_t struct defining the network interface.
 * @param TLSParams - TLSConnectParams defines the properties of the TLS connection.
 * @return int - successful connection or TLS error
 */
int iot_tls_connect(NetworkContext_t *pNetwork, TLSConnectParams *TLSParams);

/**
 * @brief Disconnect from network socket
 *
 * @param NetworkContext_t - Pointer to a NetworkContext_t struct defining the network interface.
 * @return int - successful read or TLS error code
 */
int iot_tls_disconnect(NetworkContext_t *pNetwork);

/**
 * @brief Perform any tear-down or cleanup of TLS layer
 *
 * Called to cleanup any resources required for the TLS layer.
 *
 * @param NetworkContext_t - Pointer to a NetworkContext_t struct defining the network interface
 * @return int - successful cleanup or TLS error code
 */
int iot_tls_destroy(NetworkContext_t *pNetwork);

/**
 * @brief Write bytes to the network socket
 *
 * @param NetworkContext_t - Pointer to a NetworkContext_t struct defining the network interface.
 * @param unsigned char pointer - buffer to write to socket
 * @param integer - number of bytes to write
 * @return integer - number of bytes written or TLS error
 * @return int - successful write length or TLS error code
 */
int iot_tls_write(NetworkContext_t *pNetwork, const unsigned char *pMsg, size_t len);

/**
 * @brief Read bytes from the network socket
 *
 * @param NetworkContext_t - Pointer to a NetworkContext_t struct defining the network interface.
 * @param unsigned char pointer - pointer to buffer where read bytes should be copied
 * @param size_t - number of bytes to read
 * @param size_t - pointer to store number of bytes read
 * @return int - successful read length or TLS error code
 */
int iot_tls_read(NetworkContext_t *pNetwork, unsigned char *pMsg, size_t len);


#ifdef __cplusplus
}
#endif

#endif //__NETWORK_INTERFACE_H_
