#include "vm.h"

void	*ft_memcpy_sw_endian(void *dst, const void *src, size_t n, size_t m)
{
	unsigned const char	*usrc;
	unsigned char		*udst;
	size_t	i;

	udst = dst;
	usrc = src;
	i = 0;
	while (n--)
	{
		if (i < m)
			udst[n] = usrc[i];
		else
			udst[n] = 0;
		i++;
	}
	return (dst);
}
