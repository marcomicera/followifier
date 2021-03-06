//
// Created by matteo on 28/10/19.
//
#include <stdio.h>
#include "database.h"
#include <iostream>
#include <ctime>

database::database(){
    mongoc_init ();
    client = mongoc_client_new ("mongodb://localhost:27017/?appname=insert-example");
}


void database::dropBoards(){
    bson_error_t error;
    collection = mongoc_client_get_collection (client, "followifier", "boards");
    mongoc_collection_drop (collection, &error);
}

void database::drop_room_collection(){
    bson_error_t error;
    collection = mongoc_client_get_collection (client, "followifier", "room");
    mongoc_collection_drop (collection, &error);
}

void database::insert_room_coordinate(Point p){
    collection = mongoc_client_get_collection (client, "followifier", "room");
    bson_t *doc;
    bson_error_t error;
    doc = bson_new ();

    BSON_APPEND_DOUBLE(doc, "x", p.getX());
    BSON_APPEND_DOUBLE(doc, "y", p.getY());

    if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error))
        std::cerr <<  error.message << std::endl;

    bson_destroy (doc);

}

void database::insert_message(followifier::ESP32Message message, Point position){
    collection = mongoc_client_get_collection (client, "followifier", "messages");
    bson_t *doc;
    bson_error_t error;
    doc = bson_new ();
    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    BSON_APPEND_UTF8 (doc, "hash",message.frame_hash().c_str());
    BSON_APPEND_UTF8 (doc, "mac",message.metadata().devicemac().c_str());
    BSON_APPEND_DOUBLE(doc, "timestamp",now);
    BSON_APPEND_DOUBLE(doc, "x", position.getX());
    BSON_APPEND_DOUBLE(doc, "y", position.getY());

    if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error))
        std::cerr <<  error.message << std::endl;
    bson_destroy (doc);
}

void database::insert_board(std::string mac, int x, int y){
    bson_error_t error;
    collection = mongoc_client_get_collection (client, "followifier", "boards");
    bson_t *doc;
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

