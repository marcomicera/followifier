//
// Created by matteo on 28/10/19.
//
#include <stdio.h>
#include "database.h"

database::database(){
    mongoc_init ();
    client = mongoc_client_new ("mongodb://localhost:27017/?appname=insert-example");

}



void database::insert_message(followifier::ESP32Message message){
    collection = mongoc_client_get_collection (client, "followifier", "messages");
    bson_t *doc;
    bson_error_t error;
    doc = bson_new ();

    std::cerr << " hash: "<< message.frame_hash() << " mac." << message.metadata().apmac() << std::endl;

    BSON_APPEND_UTF8 (doc, "hash",message.frame_hash().c_str());
    BSON_APPEND_UTF8 (doc, "mac",message.metadata().apmac().c_str());
    BSON_APPEND_INT64 (doc, "timestamp",message.metadata().timestamp());


    if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
    bson_destroy (doc);
}

void database::insert_board(std::string mac, int x, int y){
    collection = mongoc_client_get_collection (client, "followifier", "boards");
    bson_t *doc;
    bson_error_t error;
    doc = bson_new ();
    BSON_APPEND_UTF8 (doc, "mac",mac.c_str());
    BSON_APPEND_INT64 (doc, "x",x);
    BSON_APPEND_INT64 (doc, "y",y);
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