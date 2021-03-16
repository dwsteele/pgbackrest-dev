/***********************************************************************************************************************************
Configuration Protocol Handler
***********************************************************************************************************************************/
#ifndef CONFIG_PROTOCOL_H
#define CONFIG_PROTOCOL_H

#include "common/type/string.h"
#include "common/type/variantList.h"
#include "protocol/client.h"
#include "protocol/server.h"

/***********************************************************************************************************************************
Constants
***********************************************************************************************************************************/
#define PROTOCOL_COMMAND_CONFIG_OPTION                              "configOption"
    STRING_DECLARE(PROTOCOL_COMMAND_CONFIG_OPTION_STR);

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
// Process config protocol requests
void configOptionProtocol(const VariantList *paramList, ProtocolServer *server);

// Get option values from a remote process
VariantList *configOptionRemote(ProtocolClient *client, const VariantList *paramList);

/***********************************************************************************************************************************
Protocol commands for ProtocolServerHandler arrays passed to protocolServerProcess()
***********************************************************************************************************************************/
#define PROTOCOL_SERVER_HANDLER_OPTION_LIST                                                                                        \
    {.command = PROTOCOL_COMMAND_CONFIG_OPTION, .handler = configOptionProtocol},

#endif
