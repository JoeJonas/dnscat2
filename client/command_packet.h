/* command_packet.h
 * By Ron Bowes
 * Created May, 2014
 *
 * A class for creating and parsing dnscat command protocol packets.
 */
#ifndef __COMMAND_PACKET_H__
#define __COMMAND_PACKET_H__

#include <stdlib.h>

#ifdef WIN32
#include "pstdint.h"
#else
#include <stdint.h>
#endif

#define MAX_COMMAND_PACKET_SIZE 1024

typedef enum
{
  COMMAND_PING  = 0x0000,
  COMMAND_SHELL = 0x0001,
  COMMAND_EXEC  = 0x0002,
} command_packet_type_t;

typedef enum
{
  STATUS_BAD_REQUEST = 0x8001,
  STATUS_NOT_IMPLEMENTED = 0xFFFF,
} command_packet_status_t;

typedef struct
{
  uint16_t request_id;
  command_packet_type_t command_id;
  NBBOOL is_request;

  union
  {
    struct
    {
      union {
        struct { char *data; } ping;
        struct { char *name; } shell;
        struct { char *name; char *command; } exec;
      } body;
    } request;

    struct
    {
      command_packet_status_t status;

      union {
        struct { char *data; } ping;
        struct { uint16_t session_id; } shell;
        struct { uint16_t session_id; } exec;
      } body;
    } response;
  } r;
} command_packet_t;

/* Parse a packet from a byte stream. */
command_packet_t *command_packet_parse(uint8_t *data, size_t length, NBBOOL is_request);

/* Create a packet with the given characteristics. */
command_packet_t *command_packet_create_ping_request(uint16_t request_id, char *data);
command_packet_t *command_packet_create_ping_response(uint16_t request_id, char *data);

command_packet_t *command_packet_create_shell_request(uint16_t request_id, char *name);
command_packet_t *command_packet_create_shell_response(uint16_t request_id, uint16_t session_id);

command_packet_t *command_packet_create_exec_request(uint16_t request_id, char *name, char *command);
command_packet_t *command_packet_create_exec_response(uint16_t request_id, uint16_t session_id);

/* Free the packet data structures. */
void command_packet_destroy(command_packet_t *packet);

/* Print the packet (debugging, mostly) */
void command_packet_print(command_packet_t *packet);

/* Needs to be freed with safe_free() */
uint8_t *command_packet_to_bytes(command_packet_t *packet, size_t *length, NBBOOL is_request);

#endif
