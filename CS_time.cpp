static clock_t begin_time;


void start_timer(void)
{
    begin_time = clock();
    return;
}

float stop_timer(void)
{
    return (float(clock() - begin_time) / CLOCKS_PER_SEC);
}

    
