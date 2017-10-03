/**
 *  Main.cpp
 *
 *  Test program
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Local libraries
 */
#include "myconnection.h"

/**
 *  Main procedure
 *  @param  argc
 *  @param  argv
 *  @return int
 */
int main(int argc, const char *argv[])
{
        //std::string broker = "10.41.3.51";
        std::string broker = "rabbitmq-clt-3.vm.a-tono.net";

        // create the loop
        React::MainLoop loop;

        // create connection
        MyConnection connection(&loop, broker.c_str());

        // start the main event loop
        loop.run();

        // done
        return 0;

}

