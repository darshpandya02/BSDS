#include <iostream>
#include <string>

#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Message.h"

/******************************************************************************
    Part 3 - 1
******************************************************************************/

// This is a function that the client program will use
// before sending the request to the server.
// The argument, buffer, is a C-string or a character array
// that needs to be filled-in in this function body.
// TODO: return value should match with the amount of
// data encoded into the buffer.
int Request::Marshal(char *buffer) {

    // Format: [req_id 4B][user_id 2B][requester 32B][num1 4B][num2 4B]
    int offset = 0;
    
    // Marshal req_id (4 bytes) - converting to network byte order
    unsigned int net_req_id = htonl(req_id);
    memcpy(buffer + offset, &net_req_id, sizeof(unsigned int));
    offset += sizeof(unsigned int);
    
    // Marshal user_id (2 bytes) - converting to network byte order
    unsigned short net_user_id = htons(user_id);
    memcpy(buffer + offset, &net_user_id, sizeof(unsigned short));
    offset += sizeof(unsigned short);
    
    // Marshal requester (32 bytes) - ensuring null termination
    char requester_cstr[MAX_REQUESTER_LEN];
    memset(requester_cstr, 0, MAX_REQUESTER_LEN); // Initialize with zeros
    strncpy(requester_cstr, requester.c_str(), MAX_REQUESTER_LEN - 1);
    requester_cstr[MAX_REQUESTER_LEN - 1] = '\0'; // Ensure null termination
    memcpy(buffer + offset, requester_cstr, MAX_REQUESTER_LEN);
    offset += MAX_REQUESTER_LEN;
    
    // Marshal num1 (4 bytes) - converting to network byte order
    int net_num1 = htonl(num1);
    memcpy(buffer + offset, &net_num1, sizeof(int));
    offset += sizeof(int);
    
    // Marshal num2 (4 bytes) - converting to network byte order
    int net_num2 = htonl(num2);
    memcpy(buffer + offset, &net_num2, sizeof(int));
    offset += sizeof(int);
    
    return offset; 

    //TODO: Write a code that marshals req_id, user_id
    // requester (32 character string), num1, and
    // num2 into a C-string (i.e., buffer) for the
    // transfer over the network. Note that (unsigned) int and short
    // types need to be encoded into network format
    // using hton functions (use htons or htonl depending on 
    // the size of data). Requester only needs to be converted
    // to a C-string and does not need to be network encoded,
    // but make sure the string is a null terminating 
    // C-string (i.e., ends with '\0' character) .
    //
    // The format should be
    // [req_id 4B][user_id 2B][requester 32B][num1 4B][num2 4B].
    // You will need to use memcpy to copy the data to the buffer.

    // feel free to change "return 0;" below.
    return 0;
}

// This is a function that the server program will use
// after receiving the request from the client to
// parse the Message content out of the C-string.
// The argument, buffer, is a C-string or a character array
// which contains the marshalled data.
void Request::Unmarshal(char *buffer) {

    // Format: [req_id 4B][user_id 2B][requester 32B][num1 4B][num2 4B]
    int offset = 0;
    
    // Unmarshal req_id (4 bytes) - converting from network byte order
    unsigned int net_req_id;
    memcpy(&net_req_id, buffer + offset, sizeof(unsigned int));
    req_id = ntohl(net_req_id);
    offset += sizeof(unsigned int);
    
    // Unmarshal user_id (2 bytes) - converting from network byte order
    unsigned short net_user_id;
    memcpy(&net_user_id, buffer + offset, sizeof(unsigned short));
    user_id = ntohs(net_user_id);
    offset += sizeof(unsigned short);
    
    // Unmarshal requester (32 bytes) - converting to std::string
    char requester_cstr[MAX_REQUESTER_LEN];
    memcpy(requester_cstr, buffer + offset, MAX_REQUESTER_LEN);
    requester_cstr[MAX_REQUESTER_LEN - 1] = '\0'; // Ensuring null termination
    requester = std::string(requester_cstr);
    offset += MAX_REQUESTER_LEN;
    
    // Unmarshal num1 (4 bytes) - converting from network byte order
    int net_num1;
    memcpy(&net_num1, buffer + offset, sizeof(int));
    num1 = ntohl(net_num1);
    offset += sizeof(int);
    
    // Unmarshal num2 (4 bytes) - converting from network byte order
    int net_num2;
    memcpy(&net_num2, buffer + offset, sizeof(int));
    num2 = ntohl(net_num2);
    offset += sizeof(int);

    // TODO: unmarshal input C-string (buffer) and assign the
    // retrieved values to the member variables of this class.
    // Input buffer format should be
    // [req_id 4B][user_id 2B][requester 32B][num1 4B][num2 4B].
    // (unsigned) int and short types are in network format so you will need to
    // convert them to host format using ntoh functions (use ntons or ntonl 
    // depending on the size of data). Requester only needs to be converted
    // to std::string and does not need to be converted to the host format.

}

// This is a function that the server program will use
// before sending the response to the client.
// The argument, buffer, is a C-string or a character array
// that needs to be filled-in in this function body.
// TODO: return value should match with the amount of
// data encoded into the buffer.
int Response::Marshal(char *buffer) {

    // Format: [req_id 4B][user_id 2B][res_id 4B][response 4B]
    int offset = 0;
    
    // Marshal req_id (4 bytes) - converting to network byte order
    unsigned int net_req_id = htonl(req_id);
    memcpy(buffer + offset, &net_req_id, sizeof(unsigned int));
    offset += sizeof(unsigned int);
    
    // Marshal user_id (2 bytes) - converting to network byte order
    unsigned short net_user_id = htons(user_id);
    memcpy(buffer + offset, &net_user_id, sizeof(unsigned short));
    offset += sizeof(unsigned short);
    
    // Marshal res_id (4 bytes) - converting to network byte order
    unsigned int net_res_id = htonl(res_id);
    memcpy(buffer + offset, &net_res_id, sizeof(unsigned int));
    offset += sizeof(unsigned int);
    
    // Marshal sum (4 bytes) - converting to network byte order
    int net_sum = htonl(sum);
    memcpy(buffer + offset, &net_sum, sizeof(int));
    offset += sizeof(int);
    
    return offset;

    //TODO: Write a code that marshals req_id, user_id,
    // and sum into a C-string (i.e., buffer) for the
    // transfer over the network. Note that (unsigned) int and short
    // types need to be encoded into network format
    // using hton variant functions (use htons or htonl depending on 
    // the size of data).
    // The format should be
    // [req_id 4B][user_id 2B][res_id 4B][response 4B].

    // feel free to change "return 0;" below.
    return 0;
}

// This is a function that the client program will use
// after receiving the response from the server.
// Parse the Message content out of the C-string.
// The argument, buffer, is a C-string or a character array
// which contains the marshalled data.
void Response::Unmarshal(char *buffer) {
    // Format: [req_id 4B][user_id 2B][res_id 4B][response 4B]
    int offset = 0;
    
    // Unmarshal req_id (4 bytes) - converting from network byte order
    unsigned int net_req_id;
    memcpy(&net_req_id, buffer + offset, sizeof(unsigned int));
    req_id = ntohl(net_req_id);
    offset += sizeof(unsigned int);
    
    // Unmarshal user_id (2 bytes) - converting from network byte order
    unsigned short net_user_id;
    memcpy(&net_user_id, buffer + offset, sizeof(unsigned short));
    user_id = ntohs(net_user_id);
    offset += sizeof(unsigned short);
    
    // Unmarshal res_id (4 bytes) - converting from network byte order
    unsigned int net_res_id;
    memcpy(&net_res_id, buffer + offset, sizeof(unsigned int));
    res_id = ntohl(net_res_id);
    offset += sizeof(unsigned int);
    
    // Unmarshal sum (4 bytes) - converting from network byte order
    int net_sum;
    memcpy(&net_sum, buffer + offset, sizeof(int));
    sum = ntohl(net_sum);
    offset += sizeof(int);

    // TODO: unmarshal input C-string (buffer) and assign the
    // retrieved values to the member variables of this class.
    // Input buffer format should be
    // [req_id 4B][user_id 2B][res_id 4B][response 4B].
    // (unsigned) int and short types are in network format so you will need to
    // convert them to host format using ntoh functions (use ntons or ntonl 
    // depending on the size of data). 

}


/******************************************************************************
    Part 3 - 2
******************************************************************************/

// This is the code going into the client of Part 3 - 2
void part_3_client_marshal_send_recv_unmarshal(Request &req, Response &res, 
        char *buffer, int sockfd) {

    
    // 1) Getting marshalled byte array using Request
    int req_size = req.Marshal(buffer);
    
    // 2) Sending the marshalled buffer over the network
    int bytes_sent = send(sockfd, buffer, req_size, 0);
    if (bytes_sent < 0) {
        perror("ERROR: failed to send request");
        return;
    }
    
    // 3) Waiting and receiving server's response
    int bytes_received = recv(sockfd, buffer, RESPONSE_BUF_SIZE, 0);
    if (bytes_received < 0) {
        perror("ERROR: failed to receive response");
        return;
    }
    
    // 4) Unmarshaling the received byte array into Response
    res.Unmarshal(buffer);

    // TODO: 
    // 1) get marshalled byte array (i.e., buffer) using Request (i.e., req).
    // 2) send the marshalled buffer over the network.
    // 3) wait and receive server's response.
    // 4) unmarshal the received byte array into Response (i.e., res).
    
}

// This is the code going into the server of Part 3 - 2
void part_3_server_recv_unmarshal(Request &req, char *buffer, int sockfd) {


    // 1) Receiving the request in byte array using the socket from the client
    int bytes_received = recv(sockfd, buffer, REQUEST_BUF_SIZE, 0);
    if (bytes_received < 0) {
        perror("ERROR: failed to receive request");
        return;
    }
    
    // 2) Unmarshaling the received byte array into Request
    req.Unmarshal(buffer);

    // TODO: 
    // 1) receive the request in byte array using the socket from the client
    // 2) unmashal the received byte array into Request (i.e., res)

}

// This is the code going into the server of Part 3 - 2
void part_3_server_marshal_send(Response &res, char *buffer, int sockfd) {


    // 1) Getting marshalled byte array using Response
    int res_size = res.Marshal(buffer);
    
    // 2) Sending the marshalled buffer over the network using send
    int bytes_sent = send(sockfd, buffer, res_size, 0);
    if (bytes_sent < 0) {
        perror("ERROR: failed to send response");
        return;
    }
    // TODO: 
    // 1) get marshalled byte array (i.e., buffer) using Response (i.e., res). 
    // 2) send the marshalled buffer over the network using send.

}


