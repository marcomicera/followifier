idf_component_register(SRCS "followifier.c"
        "components/sniffer.c"
        "gen/message.pb-c.c"
        "components/flusher.c"
        "components/wifi.c"
        "components/nvs.c"
        "util/misc.c"
        INCLUDE_DIRS ".")