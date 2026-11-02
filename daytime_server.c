#include <stdio.h>
#include <unistd.h>
#include <time.h>

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
	
	return (0);
}
