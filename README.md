# p2p_client

This is a cliet that make p2p connection with the help of [p2p_server](https://gitlab.com/complete_graph/p2p_server) that is a simple STUN server.
source code from https://www.tuicool.com/articles/YNBVrei with a little edit.

### Prerequisites

* [cmake    instalation](https://cmake.org/install/)
* [Boost    instalation](https://www.boost.org/doc/libs/1_70_0/more/getting_started/unix-variants.html)

## Build

```
cmake -Bbuild -H.
cd build
make 
```

## Deployment

Afret running server run ```p2p_client [server host] [host port]```.
> Note : For create a p2p connection with udp hole punching you need to tun 2 clients that connect to server and login with client 1 and punch it client 2 if you need more help send help command to server and after you punch client 1 you need to send a command to server to connection be inited.

## Built With

* [cmake](https://cmake.org/documentation/)
* [Boost](https://www.boost.org/doc/)

## Authors

* **Behnam Sabaghi** - *Initial work* - [FONQRI](https://github.com/FONQRI)


## License

This project is licensed under the Boost License - see the [Licence](LICENSE) file for details

## Acknowledgments

* [Modern Cpp](http://moderncpp.ir/) website 




