#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

static int open_connection(char *host, char *service);

int	main(int argc, char *argv[])
{
	int		socket_fd;
	FILE	*file_stream;
	char	*hostname;
	char	buffer[1024];

	if (argc > 2)
	{
		fprintf(stderr, "usage: %s [hostname]\n", argv[0]);
		return (1);
	}

	if (argc == 1)
		hostname = "localhost";
	hostname = argv[1];

	socket_fd = open_connection(hostname, "daytime");
	if (socket_fd < 0)
	{
		fprintf(stderr, "Error: could not open connection to %s\n", hostname);
		return (1);
	}

	file_stream = fdopen(socket_fd, "r");
	if (file_stream == NULL)
	{
		fprintf(stderr, "Error: could not create file stream\n");
		return (1);
	}

	fgets(buffer, sizeof(buffer), file_stream);
	fclose(file_stream);
	fputs(buffer, stdout);
	return (0);
}


static int open_connection(char *host, char *service)
{
	int							socket_fd;
	struct addrinfo	hints, *response;
	int							error_code;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((error_code = getaddrinfo(host, service, &hints, &response)) != 0)
	{
		fprintf(stderr, "Error: getaddrinfo: %s\n", gai_strerror(error_code));
		return (-1);
	}

	for (struct addrinfo *ai = response; ai != NULL; ai = ai->ai_next)
	{
		socket_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (socket_fd < 0)
			continue;

		if (connect(socket_fd, ai->ai_addr, ai->ai_addrlen) < 0)
		{
			close(socket_fd);
			continue;
		}

		freeaddrinfo(response);
		return (socket_fd);
	}

	fprintf(stderr, "Error: could not connect to %s:%s\n", host, service);
	freeaddrinfo(response);
	return (-1);
}

