/***********************************************************************************************************************************
Tls Test Harness
***********************************************************************************************************************************/
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <openssl/conf.h>
#include <openssl/ssl.h>

#include "common/crypto/common.h"
#include "common/error.h"
#include "common/io/socket/session.h"
#include "common/io/tls/session.intern.h"
#include "common/type/buffer.h"
#include "common/wait.h"

#include "common/harnessTest.h"
#include "common/harnessTls.h"

/***********************************************************************************************************************************
Test defaults
***********************************************************************************************************************************/
#define TLS_TEST_HOST                                               "tls.test.pgbackrest.org"

static int testServerSocket = 0;
static SSL_CTX *testServerContext = NULL;
static TlsSession *testServerSession = NULL;

/***********************************************************************************************************************************
Initialize TLS and listen on the specified port for TLS connections
***********************************************************************************************************************************/
void
harnessTlsServerInit(unsigned int port, const char *serverCert, const char *serverKey)
{
    // Add test hosts
    if (testContainer())
    {
        if (system("echo \"127.0.0.1 " TLS_TEST_HOST "\" | sudo tee -a /etc/hosts > /dev/null") != 0)
            THROW(AssertError, "unable to add test host to /etc/hosts");
    }

    // Initialize ssl and create a context
    cryptoInit();

    const SSL_METHOD *method = SSLv23_method();
    cryptoError(method == NULL, "unable to load TLS method");

    testServerContext = SSL_CTX_new(method);
    cryptoError(testServerContext == NULL, "unable to create TLS context");

    // Configure the context by setting key and cert
    cryptoError(
        SSL_CTX_use_certificate_file(testServerContext, serverCert, SSL_FILETYPE_PEM) <= 0, "unable to load server certificate");
    cryptoError(
        SSL_CTX_use_PrivateKey_file(testServerContext, serverKey, SSL_FILETYPE_PEM) <= 0, "unable to load server private key");

    // Create the socket
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons((uint16_t)port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((testServerSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        THROW_SYS_ERROR(AssertError, "unable to create socket");

    // Set the address as reusable so we can bind again in the same process for testing
    int reuseAddr = 1;
    setsockopt(testServerSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));

    // Bind the address.  It might take a bit to bind if another process was recently using it so retry a few times.
    Wait *wait = waitNew(2000);
    int result;

    do
    {
        result = bind(testServerSocket, (struct sockaddr *)&address, sizeof(address));
    }
    while (result < 0 && waitMore(wait));

    if (result < 0)
        THROW_SYS_ERROR(AssertError, "unable to bind socket");

    // Listen for client connections
    if (listen(testServerSocket, 1) < 0)
        THROW_SYS_ERROR(AssertError, "unable to listen on socket");
}

/**********************************************************************************************************************************/
void
harnessTlsServerInitDefault(void)
{
    if (testContainer())
        harnessTlsServerInit(harnessTlsTestPort(), TLS_CERT_TEST_CERT, TLS_CERT_TEST_KEY);
    else
    {
        harnessTlsServerInit(
            harnessTlsTestPort(),
            strPtr(strNewFmt("%s/" TEST_CERTIFICATE_PREFIX ".crt", testRepoPath())),
            strPtr(strNewFmt("%s/" TEST_CERTIFICATE_PREFIX ".key", testRepoPath())));
    }
}

/***********************************************************************************************************************************
Expect an exact string from the client
***********************************************************************************************************************************/
void
harnessTlsServerExpect(const char *expected)
{
    Buffer *buffer = bufNew(strlen(expected));

    ioRead(tlsSessionIoRead(testServerSession), buffer);

    // Treat any ? characters as wildcards so variable elements (e.g. auth hashes) can be ignored
    String *actual = strNewBuf(buffer);

    for (unsigned int actualIdx = 0; actualIdx < strSize(actual); actualIdx++)
    {
        if (expected[actualIdx] == '?')
            ((char *)strPtr(actual))[actualIdx] = '?';
    }

    // Error if actual does not match expected
    if (!strEqZ(actual, expected))
        THROW_FMT(AssertError, "server expected '%s' but got '%s'", expected, strPtr(actual));
}

/***********************************************************************************************************************************
Send a reply to the client
***********************************************************************************************************************************/
void
harnessTlsServerReply(const char *reply)
{
    ioWrite(tlsSessionIoWrite(testServerSession), BUF((unsigned char *)reply, strlen(reply)));
    ioWriteFlush(tlsSessionIoWrite(testServerSession));
}

/***********************************************************************************************************************************
Accept a TLS connection from the client
***********************************************************************************************************************************/
void
harnessTlsServerAccept(void)
{
    struct sockaddr_in addr;
    unsigned int len = sizeof(addr);

    int testClientSocket = accept(testServerSocket, (struct sockaddr *)&addr, &len);

    if (testClientSocket < 0)
        THROW_SYS_ERROR(AssertError, "unable to accept socket");

    SSL *testClientSSL = SSL_new(testServerContext);

    testServerSession = tlsSessionNew(
        testClientSSL, sckSessionNew(sckSessionTypeServer, testClientSocket, STRDEF("client"), 0, 5000), 5000);
}

/***********************************************************************************************************************************
Close the connection
***********************************************************************************************************************************/
void
harnessTlsServerClose(void)
{
    tlsSessionClose(testServerSession, true);
    tlsSessionFree(testServerSession);
    testServerSession = NULL;
}

/**********************************************************************************************************************************/
void
harnessTlsServerAbort(void)
{
    tlsSessionClose(testServerSession, false);
    tlsSessionFree(testServerSession);
    testServerSession = NULL;
}

/**********************************************************************************************************************************/
const String *harnessTlsTestHost(void)
{
    return strNew(testContainer() ? TLS_TEST_HOST : "127.0.0.1");
}

/**********************************************************************************************************************************/
unsigned int harnessTlsTestPort(void)
{
    return 44443 + testIdx();
}
