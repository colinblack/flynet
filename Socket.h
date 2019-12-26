#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

namespace cobred {
    namespace  net {
		void Socket(int32_t domain,  int32_t type, int32_t  protocol);
		void Bind(int32_t sockfd, const struct sockaddr *addr, socklen_t addrlen);
		void Listen(int sockfd, int backlog);
		int32_t Accept(int32_t sockfd, struct sockaddr *addr, socklen_t *addrlen);
    }
}
     

