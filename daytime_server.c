#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

char	*get_time(void)
{
	static char	buffer[128];
	time_t			current_time;
	struct tm		*time_info;

	time(&current_time);
	time_info = localtime(&current_time);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
	return (buffer);
}

int	main(void)
{
	int									socket_fd;
	struct sockaddr_in	server_addr;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0)
	{
		perror("socket");
		return (1);
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(13);

	bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(socket_fd, 5);

	while (1)
	{
		int client_fd = accept(socket_fd, NULL, NULL);
		if (client_fd < 0)
		{
			perror("accept");
			continue;
		}
		char *time_str = get_time();
		write(client_fd, time_str, strlen(time_str));
		close(client_fd);
	}
	close(socket_fd);
	return (0);
}
