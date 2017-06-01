﻿/**
 * Copyright (c) 2017 China Mobile IOT.
 * All rights reserved.
**/

#include "internal.h"

int nbiot_uri_decode( nbiot_uri_t   *uri,
                      const uint8_t *buffer,
                      size_t         buffer_len )
{
    int ret;
    uint16_t uri_path_len;
    const char *uri_path;

    /* object id */
    ret = coap_option( buffer,
                       buffer_len,
                       COAP_OPTION_URI_PATH,
                       (const uint8_t**)&uri_path,
                       &uri_path_len,
                       true );
    if ( !ret )
    {
        return -1;
    }

#ifdef NBIOT_BOOTSTRAP
    if ( uri_path_len == 2 &&
         ((uri_path[0] == 'r' && uri_path[1] == 'd') || /* rd */
          (uri_path[0] == 'b' && uri_path[1] == 's')) ) /* bs */
#else
    if ( uri_path_len == 2 &&
         uri_path[0] == 'r' &&
         uri_path[1] == 'd' ) /* rd */
#endif
    {
        buffer += ret;
        buffer_len -= ret;
        ret = coap_option( buffer,
                           buffer_len,
                           COAP_OPTION_URI_PATH,
                           (const uint8_t**)&uri_path,
                           &uri_path_len,
                           false );
        if ( !ret )
        {
            return -1;
        }
    }

    /* object id */
    uri->flag = NBIOT_SET_OBJID;
    uri->objid = (uint16_t)nbiot_atoi( uri_path, uri_path_len );

    /* object instance id */
    buffer += ret;
    buffer_len -= ret;
    ret = coap_option( buffer,
                       buffer_len,
                       COAP_OPTION_URI_PATH,
                       (const uint8_t**)&uri_path,
                       &uri_path_len,
                       false );
    if ( ret )
    {
        uri->flag |= NBIOT_SET_INSTID;
        uri->instid = (uint16_t)nbiot_atoi( uri_path, uri_path_len );
    }
    else
    {
        return 0;
    }

    /* resource id */
    buffer += ret;
    buffer_len -= ret;
    ret = coap_option( buffer,
                       buffer_len,
                       COAP_OPTION_URI_PATH,
                       (const uint8_t**)&uri_path,
                       &uri_path_len,
                       false );
    if ( ret )
    {
        uri->flag |= NBIOT_SET_RESID;
        uri->resid = (uint16_t)nbiot_atoi( uri_path, uri_path_len );
    }

    return 0;
}
