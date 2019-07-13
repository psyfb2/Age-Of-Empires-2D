#pragma once

// Responses from queries
#define NOTIFY_NO 0
#define NOTIFY_YES 1

// Ask components whether click is inside it - QUERY
#define NOTIFY_TOKEN_CHECK_MOUSEDOWN 1001

// Used for tokens - tell them that the mouse down has happened
#define NOTIFY_TOKEN_START_DRAG 1002

// Used to tell a token that the card it is on has moved - gives x and y offsets
#define NOTIFY_TOKEN_CONTAINER_MOVED 1003

// Used to find out which token container is on top - QUERY
#define NOTIFY_FIND_TOKEN_CONTAINER_FOR_POSITION 1004


// Ask desks whether they are the top deck - allows dragging of cards onto decks
#define NOTIFY_IS_CARD_DECK_AT_LOCATION 1101


