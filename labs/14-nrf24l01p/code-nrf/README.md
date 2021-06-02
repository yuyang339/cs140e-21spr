2-client-ping-pong.c
2-server-ping-pong.c

    nrf_config_t c = nrf_conf_reliable_mk(RETRAN_ATTEMPTS, RETRAN_DELAY);
    // can mess with the mhz delta to see if less interference
    c = nrf_set_Mhz_delta(c, 31);
    // can mess with the power to see if less interference
    c = nrf_set_db(c, dmb_minus6);
or:

    // but; might require too much power
    c = nrf_set_db(c, dmb_0);


