//
// Created by matteo on 28/10/19.
//
#include <stdio.h>
#include "database.h"

database::database(){
    mongoc_init ();
    client = mongoc_client_new ("mongodb://localhost:27017/?appname=insert-example");
    collection = mongoc_client_get_collection (client, "followifier", "mycoll");

}



void database::insert_message(followifier::ESP32Message message){
    bson_t *doc;
    bson_error_t error;
    doc = bson_new ();

    BSON_APPEND_INT32 (doc, "kApMacFieldNumber",message.metadata().kApMacFieldNumber);
    BSON_APPEND_INT32 (doc, "kSsidFieldNumber",message.metadata().kSsidFieldNumber);
    BSON_APPEND_INT32 (doc, "kTimestampFieldNumber",message.metadata().kTimestampFieldNumber);
    BSON_APPEND_INT32 (doc, "kFrameHashFieldNumber",message.kFrameHashFieldNumber);
    BSON_APPEND_INT32 (doc, "kRsiFieldNumber",message.metadata().kRsiFieldNumber);

    if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
    bson_destroy (doc);
}

void database::destroy_database_connection(){

    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);
    mongoc_cleanup ();
}